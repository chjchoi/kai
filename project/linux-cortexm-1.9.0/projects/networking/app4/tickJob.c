
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
extern struct stm32f2_gpio_dsc led1;
int tickJob(void)
{
	stm32f2_gpout_toggle(&led1);
	return 0;
}



