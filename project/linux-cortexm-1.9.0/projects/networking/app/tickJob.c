
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
extern struct stm32f2_gpio_dsc tp_sync;
int tickJob(void)
{
//	stm32f2_gpout_toggle(&led1);
	//tp_sync에 대한 초기화 
//stm32f2_gpio_config(&tp_sync, STM32F2_GPIO_ROLE_GPOUT); 은 .../arch/arm/mach-stm32/iomux.c에서 이미 이루어져있다.

	stm32f2_gpout_toggle(&tp_sync);
	return 0;
}



