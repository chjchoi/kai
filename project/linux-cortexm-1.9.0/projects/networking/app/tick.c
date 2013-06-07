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
#include "tickJob.h"

//#define TIME_STEP (100*HZ/100) //HZ 100 , so 1초   
#define TIME_STEP (1*HZ/120) //HZ=120 1/120 초 간격   

typedef struct{
	struct timer_list timer;
	unsigned long led;
} __attribute__ ((packed)) TICK_MANAGER;

static TICK_MANAGER *ptrmng =NULL;
void tick_timeover(unsigned long arg);

void tick_registertimer(TICK_MANAGER * pdata,unsigned long timeover)
{
	init_timer(&(pdata->timer));
	pdata->timer.expires  = get_jiffies_64() + timeover;
	pdata->timer.data     = (unsigned long) pdata;
	pdata->timer.function =  tick_timeover;
	add_timer(&(pdata->timer));
}

void tick_timeover(unsigned long arg)
{
	TICK_MANAGER *pdata =NULL;
	if(arg)
	{
		/*  code here*/
		//
		pdata = (TICK_MANAGER *)  arg;
		//
			tickJob();
		//
		tick_registertimer(pdata,TIME_STEP);
	}
}

/*
 * Device open
 */
int tick_init(void)
{
	ptrmng =kmalloc(sizeof(TICK_MANAGER),GFP_KERNEL);
	if(ptrmng == NULL) return -ENOMEM;
	memset(ptrmng,0,sizeof(TICK_MANAGER));
	ptrmng->led=0;
	tick_registertimer(ptrmng,TIME_STEP);
	return 0;
}
void tick_exit(void)
{
	if(ptrmng != NULL)
	{
		del_timer(&(ptrmng->timer));
		kfree(ptrmng);
	}
}

module_init(tick_init);
module_exit(tick_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("choi, chjchoi@nor.co.kr");
MODULE_DESCRIPTION("Kernel Timer for 120Hz device driver");
