/*
 * tp_sync.c - The simplest loadable kernel module.
 * Intended as a template for development of more
 * meaningful kernel modules.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "stm32f2_gpio_drv.h"

extern struct stm32f2_gpio_dsc tp_sync;
/*
 * Driver verbosity level: 0->silent; >0->verbose
 */
static int tp_sync_debug = 0;

/*
 * User can change verbosity of the driver
 */
module_param(tp_sync_debug, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(tp_sync_debug, "tp_sync driver verbosity level");

/*
 * Service to print debug messages
 */
#define d_printk(level, fmt, args...)				\
	if (tp_sync_debug >= level) printk(KERN_INFO "%s: " fmt,	\
					__func__, ## args)

/*
 * Device major number
 */
static uint tp_sync_major = 240;

/*
 * User can change the major number
 */
module_param(tp_sync_major, uint, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(tp_sync_major, "tp_sync driver major number");

/*
 * Device name
 */
static char * tp_sync_name = "tp_sync";

/*
 * Device access lock. Only one process can access the driver at a time
 */
static int tp_sync_lock = 0;

/*
 * Device "data"
 */
static char tp_sync_str[] = "This is the simplest loadable kernel module\n";
static char *tp_sync_end;

/*
 * Device open
 */
static int tp_sync_open(struct inode *inode, struct file *file)
{
	int ret = 0;

	/*
	 * One process at a time
	 */
	if (tp_sync_lock ++ > 0) {
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
	tp_sync_end = tp_sync_str + strlen(tp_sync_str);

	stm32f2_gpio_config(&tp_sync,STM32F2_GPIO_ROLE_GPOUT);
Done:
	return ret;
}

/*
 * Device close
 */
static int tp_sync_release(struct inode *inode, struct file *file)
{
	/*
 	 * Release device
 	 */
	tp_sync_lock = 0;

	/*
 	 * Decrement module use counter
 	 */
	module_put(THIS_MODULE);

	d_printk(2, "lock=%d\n", tp_sync_lock);
	return 0;
}

/* 
 * Device read
 */
static ssize_t tp_sync_read(struct file *filp, char *buffer,
			 size_t length, loff_t * offset)
{
//	char * addr;
	unsigned int len = 0;
	int ret = 0;

	/*
 	 * Check that the user has supplied a valid buffer
 	 */
	if (! access_ok(0, buffer, length)) {
		ret = -EINVAL;
		goto Done;
	}

	/*
 	 * Get access to the device "data"
 	 */
//	addr = tp_sync_str + *offset;

	/*
	 * Check for an EOF condition.
	 */
//j	if (addr >= tp_sync_end) {
//		ret = 0;
//		goto Done;
//	}

	/*
 	 * Read in the required or remaining number of bytes into
 	 * the user buffer
 	 */
//	len = addr + length < tp_sync_end ? length : tp_sync_end - addr;

	strncpy(buffer, addr, len);
	*offset += len;
	ret = len;

Done:
	d_printk(3, "length=%d,len=%d,ret=%d\n", length, len, ret);
	return ret;
}

/* 
 * Device write
 */
static ssize_t tp_sync_write(struct file *filp, const char *buffer,
			  size_t length, loff_t * offset)
{
//	d_printk(3, "length=%d\n", length);
	return -EIO;
}

/*
 * Device operations
 */
static struct file_operations tp_sync_fops = {
	.read = tp_sync_read,
//	.write = tp_sync_write,
	.open = tp_sync_open,
	.release = tp_sync_release
};

static int __init tp_sync_init_module(void)
{
	int ret = 0;

	/*
 	 * check that the user has supplied a correct major number
 	 */
	if (tp_sync_major == 0) {
		printk(KERN_ALERT "%s: tp_sync_major can't be 0\n", __func__);
		ret = -EINVAL;
		goto Done;
	}

	/*
 	 * Register device
 	 */
	ret = register_chrdev(tp_sync_major, tp_sync_name, &tp_sync_fops);
	if (ret < 0) {
		printk(KERN_ALERT "%s: registering device %s with major %d "
				  "failed with %d\n",
		       __func__, tp_sync_name, tp_sync_major, ret);
		goto Done;
	}
	
Done:
	d_printk(1, "name=%s,major=%d\n", tp_sync_name, tp_sync_major);

	return ret;
}
static void __exit tp_sync_cleanup_module(void)
{
	/*
	 * Unregister device
	 */
	unregister_chrdev(tp_sync_major, tp_sync_name);

	d_printk(1, "%s\n", "clean-up successful");
}

module_init(tp_sync_init_module);
module_exit(tp_sync_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Choi, chjchoi@nor.co.kr");
MODULE_DESCRIPTION("tp _sync device driver");
