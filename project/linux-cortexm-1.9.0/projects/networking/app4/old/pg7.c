/*
 * pg7.c - The simplest loadable kernel module.
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
static int pg7_debug = 0;

/*
 * User can change verbosity of the driver
 */
module_param(pg7_debug, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(pg7_debug, "STM32F4 GPIOS driver verbosity level");

/*
 * Service to print debug messages
 */
#define d_printk(level, fmt, args...)				\
	if (pg7_debug >= level) printk(KERN_INFO "%s: " fmt,	\
					__func__, ## args)

/*
 * Device major number
 */
static uint pg7_major = 242;//

/*
 * User can change the major number
 */
module_param(pg7_major, uint, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(pg7_major, "pg7 driver major number");

/*
 * Device name
 */
static char * pg7_name = "usartShortLoopback";

/*
 * Device access lock. Only one process can access the driver at a time
 */
static int pg7_lock = 0;

/*
 * Device "data"
 */
//static char pg7_str[] = "This is the STM32F4 Status Gpio Device driver module\n";
//static char *pg7_end;

/*
 * Device open
 */
        struct stm32f2_gpio_dsc pg7 ={6,7};//PG7
static int pg7_open(struct inode *inode, struct file *file)
{
	int ret = 0;
	int rv;
	rv =stm32f2_gpio_config(&pg7,STM32F2_GPIO_ROLE_GPOUT);
	rv =stm32f2_gpout_set(&pg7,1);
	/*
	 * One process at a time
	 */
	if (pg7_lock ++ > 0) {
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
	
Done:
	return ret;
}

/*
 * Device close
 */
static int pg7_release(struct inode *inode, struct file *file)
{
	pg7_lock = 0;
	module_put(THIS_MODULE);
	return 0;
}

/* 
 * Device read
 */
static ssize_t pg7_read(struct file *filp, char *buffer,
			 size_t length, loff_t * offset)
{
	return 0;
}

/* 
 * Device write
 */
/*
static ssize_t pg7_write(struct file *filp, const char *buffer,
			  size_t length, loff_t * offset)
{
	if(buffer[0]==0x30){
		rv =stm32f2_gpout_set(&pg7,0);
	}else if(buffer[0]==0x31){
		rv =stm32f2_gpout_set(&pg7,1);
	}
	else
	{	
	return -EIO;
	}
}
*/

/*
 * Device operations
 */
static struct file_operations pg7_fops = {
	.open = pg7_open,
	.release = pg7_release,
	.write   =pg7_write
};

static int __init pg7_init_module(void)
{
	int ret = 0;

	/*
 	 * check that the user has supplied a correct major number
 	 */
	if (pg7_major == 0) {
		printk(KERN_ALERT "%s: pg7_major can't be 0\n", __func__);
		ret = -EINVAL;
		goto Done;
	}

	/*
 	 * Register device
 	 */
	ret = register_chrdev(pg7_major, pg7_name, &pg7_fops);
	if (ret < 0) {
		printk(KERN_ALERT "%s: registering device %s with major %d "
				  "failed with %d\n",
		       __func__, pg7_name, pg7_major, ret);
		goto Done;
	}
	
Done:
	return ret;
}
static void __exit pg7_cleanup_module(void)
{
	/*
	 * Unregister device
	 */
	unregister_chrdev(pg7_major, pg7_name);

}

module_init(pg7_init_module);
module_exit(pg7_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Choi,choi@nor.co.kr");
MODULE_DESCRIPTION("STM32F4 Status Gpio device driver");
