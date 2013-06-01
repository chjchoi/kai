/*
 * usartShortLoopBackRelease.c - The simplest loadable kernel module.
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
static int usartShortLoopBackRelease_debug = 0;

/*
 * User can change verbosity of the driver
 */
module_param(usartShortLoopBackRelease_debug, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(usartShortLoopBackRelease_debug, "USART short loop back release");

/*
 * Service to print debug messages
 */
#define d_printk(level, fmt, args...)				\
	if (usartShortLoopBackRelease_debug >= level) printk(KERN_INFO "%s: " fmt,	\
					__func__, ## args)

/*
 * Device major number
 */
static uint usartShortLoopBackRelease_major = 240;//

/*
 * User can change the major number
 */
module_param(usartShortLoopBackRelease_major, uint, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(usartShortLoopBackRelease_major, "usartShortLoopBackRelease driver major number");

/*
 * Device name
 */
static char * usartShortLoopBackRelease_name = "usart_slb_releaseo";

/*
 * Device access lock. Only one process can access the driver at a time
 */
static int usartShortLoopBackRelease_lock = 0;

/*
 * Device "data"
 */
//static char usartShortLoopBackRelease_str[] = "This is the STM32F4 Status Gpio Device driver module\n";
//static char *usartShortLoopBackRelease_end;

/*
 * Device open
 */
struct stm32f2_gpio_dsc uslb =
static int usartShortLoopBackRelease_open(struct inode *inode, struct file *file)
{
	int ret = 0;
	int rv;
        struct stm32f2_gpio_dsc pg7 ={6,7};//PG7
	rv =stm32f2_gpio_config(&pg7,STM32F2_GPIO_ROLE_GPOUT);
//	rv =stm32f2_gpout_set(&pg7,1);
	/*
	 * One process at a time
	 */
	if (usartShortLoopBackRelease_lock ++ > 0) {
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
//	usartShortLoopBackRelease_end = usartShortLoopBackRelease_str + strlen(usartShortLoopBackRelease_str);
	//
	stm32f2_status_gpio_init();
	
Done:
//	d_printk(2, "lock=%d\n", usartShortLoopBackRelease_lock);
	return ret;
}

/*
 * Device close
 */
static int usartShortLoopBackRelease_release(struct inode *inode, struct file *file)
{
	/*
 	 * Release device
 	 */
	usartShortLoopBackRelease_lock = 0;

	/*
 	 * Decrement module use counter
 	 */
	module_put(THIS_MODULE);

//	d_printk(2, "lock=%d\n", usartShortLoopBackRelease_lock);
	return 0;
}

/* 
 * Device read
 */
static ssize_t usartShortLoopBackRelease_read(struct file *filp, char *buffer,
			 size_t length, loff_t * offset)
{
	//char * addr;
	//unsigned int len = 0;
	int ret = 0;
	int rv=0;
	struct stm32f2_gpio_dsc *dsc=        kmalloc(sizeof(struct stm32f2_gpio_dsc),GFP_KERNEL);
	volatile struct stm32f2_gpio_regs *gpio_regs;
	dsc->port =7; dsc->pin=8;	
 	// Check that the user has supplied a valid buffer
	if (! access_ok(0, buffer, length)) {
		ret = -EINVAL;
		goto Done;
	}
//
	switch(buffer[0])
	{
		case '0':
			dsc->port =7; dsc->pin =8;
			break;
		case '1':
			dsc->port =7; dsc->pin =9;
			break;
		case '2':
			dsc->port =7; dsc->pin =10;
			break;
		case '3':
			dsc->port =7; dsc->pin =11;
			break;
		case '4':
			dsc->port =7; dsc->pin =12;
			break;
		case '5':
			dsc->port =8; dsc->pin =0;
			break;
		case '6':
			dsc->port =8; dsc->pin =1;
			break;
		case '7':
			dsc->port =8; dsc->pin =2;
			break;
		case '8':
			dsc->port =8; dsc->pin =3;
			break;
		default:
			printk(KERN_ALERT "wrong %d ",buffer[0]);
			goto error;
			break;
	}
	if(dsc->port ==7)
	{
			gpio_regs =(volatile struct stm32f2_gpio_regs*)STM32F2_GPIOH_BASE;
	}
	else
	{
			gpio_regs =(volatile struct stm32f2_gpio_regs*)STM32F2_GPIOI_BASE;
	}		
		
	rv =(int)((gpio_regs->idr)&0xFFFF);
	if(rv &(1<<(dsc->pin)))
	{
		buffer[0]=0x31;
	}
	else
	{
		buffer[0] =0x30;
	}
Done:
	ret =0;
	kfree(dsc);
	return ret;
error:
	kfree(dsc);
	ret =-1;
	return ret;
}

/* 
 * Device write
 */
/*
static ssize_t usartShortLoopBackRelease_write(struct file *filp, const char *buffer,
			  size_t length, loff_t * offset)
{
	d_printk(3, "length=%d\n", length);
	return -EIO;
}
*/

/*
 * Device operations
 */
static struct file_operations usartShortLoopBackRelease_fops = {
	.read = usartShortLoopBackRelease_read,
	.open = usartShortLoopBackRelease_open,
	.release = usartShortLoopBackRelease_release
};

static int __init usartShortLoopBackRelease_init_module(void)
{
	int ret = 0;

	/*
 	 * check that the user has supplied a correct major number
 	 */
	if (usartShortLoopBackRelease_major == 0) {
		printk(KERN_ALERT "%s: usartShortLoopBackRelease_major can't be 0\n", __func__);
		ret = -EINVAL;
		goto Done;
	}

	/*
 	 * Register device
 	 */
	ret = register_chrdev(usartShortLoopBackRelease_major, usartShortLoopBackRelease_name, &usartShortLoopBackRelease_fops);
	if (ret < 0) {
		printk(KERN_ALERT "%s: registering device %s with major %d "
				  "failed with %d\n",
		       __func__, usartShortLoopBackRelease_name, usartShortLoopBackRelease_major, ret);
		goto Done;
	}
	
Done:
	d_printk(1, "name=%s,major=%d\n", usartShortLoopBackRelease_name, usartShortLoopBackRelease_major);

	return ret;
}
static void __exit usartShortLoopBackRelease_cleanup_module(void)
{
	/*
	 * Unregister device
	 */
	unregister_chrdev(usartShortLoopBackRelease_major, usartShortLoopBackRelease_name);

	d_printk(1, "%s\n", "clean-up successful");
}

module_init(usartShortLoopBackRelease_init_module);
module_exit(usartShortLoopBackRelease_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Choi,choi@nor.co.kr");
MODULE_DESCRIPTION("STM32F4 Status Gpio device driver");
