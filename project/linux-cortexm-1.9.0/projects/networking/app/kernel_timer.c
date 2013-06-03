/*
 * kernel_timer.c - The simplest loadable kernel module.
 * Intended as a template for development of more
 * meaningful kernel modules.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
//
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/time.h>
#include <linux/timer.h>
#include "timer120HzJob.h"
//#define TIME_STEP (1*HZ/120) //1/120 초 
//#define TIME_STEP (120*HZ/120) //120/120  
#define TIME_STEP (100*HZ/100) //HZ 100 , so 1초   

typedef struct{
	struct timer_list timer;
	unsigned long led;
} __attribute__ ((packed)) KERNEL_TIMER_MANAGER;
static KERNEL_TIMER_MANAGER *ptrmng =NULL;
void kerneltimer_timeover(unsigned long arg);
//
void kerneltimer_registertimer(KERNEL_TIMER_MANAGER * pdata,unsigned long timeover)
{
	init_timer(&(pdata->timer));
	pdata->timer.expires  = get_jiffies_64() + timeover;
	pdata->timer.data     = (unsigned long) pdata;
	pdata->timer.function =  kerneltimer_timeover;
	add_timer(&(pdata->timer));
}
#define d_printk(level, fmt, args...)				\
	if (kernel_timer_debug >= level) printk(KERN_INFO "%s: " fmt,	\
					__func__, ## args)

void kerneltimer_timeover(unsigned long arg)
{
	KERNEL_TIMER_MANAGER *pdata =NULL;
	if(arg)
	{
		/*  code here*/
		//
		//	timer120HzJob();
		//
		kerneltimer_registertimer(pdata,TIME_STEP);
	}
}

/*
 * Device open
 */
int kerneltimer_init(void)
{
	ptrmng =kmalloc(sizeof(KERNEL_TIMER_MANAGER),GFP_KERNEL);
	if(ptrmng == NULL) return -ENOMEM;
	memset(ptrmng,0,sizeof(KERNEL_TIMER_MANAGER));
	ptrmng->led=0;
	kerneltimer_registertimer(ptrmng,TIME_STEP);
	return 0;
}
void kerneltimer_exit(vodi)
{
	if(ptrmng != NULL)
	{
		del_timer(&(ptrmng->timer));
		kfree(ptrmng);
	}
        // exit 관련 처리
}

module_init(kerneltimer_init);
module_exit(kerneltimer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("choi, chjchoi@nor.co.kr");
MODULE_DESCRIPTION("Kernel Timer for 120Hz device driver");
