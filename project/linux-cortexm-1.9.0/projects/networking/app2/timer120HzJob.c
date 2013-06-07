#include "timer120HzJob.h"
#include "stm32f2_gpio_drv.h"
extern struct stm32f2_gpio_dsc led1;
int timer120HzJob(void)
{
	// timer alert.. 1ms 이상 대기하지 말것 
	
	stm32f2_gpout_toggle(&led1);
        // 레지스터 세팅 또는 비트 세틍 정도만 하고 빠져나올것
	return 0;
}

