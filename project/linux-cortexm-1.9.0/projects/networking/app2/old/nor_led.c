/*
 * nor_led.c - The simplest loadable kernel module.
 * Intended as a template for development of more
 * meaningful kernel modules.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
//
#include "stm32f2_gpio.h"

/*
 * Driver verbosity level: 0->silent; >0->verbose
 */
static int nor_led_debug = 0;

/*
 * User can change verbosity of the driver
 */
module_param(nor_led_debug, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(nor_led_debug, "NOR LED driver verbosity level");

/*
 * Service to print debug messages
 */
#define d_printk(level, fmt, args...)				\
	if (nor_led_debug >= level) printk(KERN_INFO "%s: " fmt,	\
					__func__, ## args)

/*
 * Device major number
 */
static uint nor_led_major = 241;//

/*
 * User can change the major number
 */
module_param(nor_led_major, uint, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(nor_led_major, "NOR LED driver major number");

/*
 * Device name
 */
static char * nor_led_name = "nor_led";

/*
 * Device access lock. Only one process can access the driver at a time
 */
static int nor_led_lock = 0;

/*
 * Device "data"
 */
//static char nor_led_str[] = "This is the NOR LED Device driver module\n";
//static char *nor_led_end;

/*
 * Device open
 */
static int nor_led_open(struct inode *inode, struct file *file)
{
	int ret = 0;
	/*
	 * One process at a time
	 */
	if (nor_led_lock ++ > 0) {
		ret = -EBUSY;
		goto Done;
	}

	/*
 	 * Increment the module use counter
 	 */
	try_module_get(THIS_MODULE);

	/*
 	 * Do open-time calculations
 	 */
	//
	//stm32_led_init();
Done:
	return ret;
}

/*
 * Device close
 */
static int nor_led_release(struct inode *inode, struct file *file)
{
	/*
 	 * Release device
 	 */
	nor_led_lock = 0;

	/*
 	 * Decrement module use counter
 	 */
	module_put(THIS_MODULE);

	return 0;
}
/* 
 * Device write
 */
static ssize_t nor_led_write(struct file *filp, const char *buffer,
			  size_t length, loff_t * offset)
{
	
	char buffer0,buffer1;
	struct stm32f2_gpio_dsc *pled1=kmalloc(sizeof(struct stm32f2_gpio_dsc),GFP_KERNEL);
//        struct stm32f2_gpio_dsc *pled2=kmalloc(sizeof(struct stm32f2_gpio_dsc),GFP_KERNEL);
	pled1->port=5;pled1->pin=10;
	//pled2->port=9;pled2->pin=4;
	buffer0=buffer[0];
	buffer1=buffer[1];
	switch(buffer0)
	{
		case 0x30:
			if(buffer1==0x30)
			{
				stm32f2_gpout_set(pled1,0);
			}
			else if(buffer1==0x31)
			{
				stm32f2_gpout_set(pled1,1);
			}
			else
			{
				goto error;
			}				
			break;
		case 0x31:
			/*
			if(buffer1==0x30)
			{
				stm32f2_gpout_set(pled2,0);
			}
			else if(buffer1==0x31)
			{
				stm32f2_gpout_set(pled2,1);
			}
			else
			{
				goto error;
			}				
			*/	
			break;
		default:
			goto error;	
	}
	kfree(pled1);
	//kfree(pled2);
	return 0;
error:
	kfree(pled1);
	//kfree(pled2);	
	return -EIO;
}

/*
 * Device operations
 */
static struct file_operations nor_led_fops = {
//	.read = nor_led_read,
	.write = nor_led_write,
	.open = nor_led_open,
	.release = nor_led_release
};

static int __init nor_led_init_module(void)
{
	int ret = 0;

	/*
 	 * check that the user has supplied a correct major number
 	 */
	if (nor_led_major == 0) {
		printk(KERN_ALERT "%s: nor_led_major can't be 0\n", __func__);
		ret = -EINVAL;
		goto Done;
	}

	/*
 	 * Register device
 	 */
	ret = register_chrdev(nor_led_major, nor_led_name, &nor_led_fops);
	if (ret < 0) {
		printk(KERN_ALERT "%s: registering device %s with major %d "
				  "failed with %d\n",
		       __func__, nor_led_name, nor_led_major, ret);
		goto Done;
	}
	
Done:
	d_printk(1, "name=%s,major=%d\n", nor_led_name, nor_led_major);

	return ret;
}
static void __exit nor_led_cleanup_module(void)
{
	/*
	 * Unregister device
	 */
	unregister_chrdev(nor_led_major, nor_led_name);

	d_printk(1, "%s\n", "clean-up successful");
}

module_init(nor_led_init_module);
module_exit(nor_led_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Choi,choi@nor.co.kr");
MODULE_DESCRIPTION("NOR LED device driver");
