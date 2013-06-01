/*
 * (C) Copyright 2011
 * Emcraft Systems, <www.emcraft.com>
 * Yuri Tikhonov <yur@emcraft.com>
 *
 * Add SDIO pin configuration for STM3220G-EVAL
 * (C) Copyright 2012
 * Emcraft Systems, <www.emcraft.com>
 * Alexander Potashev <aspotashev@emcraft.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>

//#include <mach/iomux.h>
#include <mach/platform.h>
#include <mach/stm32.h>
#include "stm32f2_gpio.h"
//NOR

struct stm32f2_gpio_dsc status_ind[9] = {
                {STM32F2_GPIO_PORT_H,STM32F2_GPIO_PIN_8},
                {STM32F2_GPIO_PORT_H,STM32F2_GPIO_PIN_9},
                {STM32F2_GPIO_PORT_H,STM32F2_GPIO_PIN_10},
                {STM32F2_GPIO_PORT_H,STM32F2_GPIO_PIN_11},
                {STM32F2_GPIO_PORT_H,STM32F2_GPIO_PIN_12},
                {STM32F2_GPIO_PORT_I,STM32F2_GPIO_PIN_0},
                {STM32F2_GPIO_PORT_I,STM32F2_GPIO_PIN_1},
                {STM32F2_GPIO_PORT_I,STM32F2_GPIO_PIN_2},
                {STM32F2_GPIO_PORT_I,STM32F2_GPIO_PIN_3},
};
/*
 * Register map bases
 */
static const unsigned long io_base[] = {
	STM32F2_GPIOA_BASE, STM32F2_GPIOB_BASE, STM32F2_GPIOC_BASE,
	STM32F2_GPIOD_BASE, STM32F2_GPIOE_BASE, STM32F2_GPIOF_BASE,
	STM32F2_GPIOG_BASE, STM32F2_GPIOH_BASE, STM32F2_GPIOI_BASE
};

/*
 * AF values (note, indexed by enum stm32f2_gpio_role)
 */
static const u32 af_val[] = {
	STM32F2_GPIO_AF_USART1, STM32F2_GPIO_AF_USART2, STM32F2_GPIO_AF_USART3,
	STM32F2_GPIO_AF_USART4, STM32F2_GPIO_AF_USART5, STM32F2_GPIO_AF_USART6,
	STM32F2_GPIO_AF_MAC, STM32F2_GPIO_AF_SDIO,STM32F2_GPIO_AF_MC0,//NOR
	0
};

/*
 * Configure the specified GPIO for the specified role
 */
int stm32f2_gpio_config(struct stm32f2_gpio_dsc *dsc,enum stm32f2_gpio_role role)
{
	volatile struct stm32f2_gpio_regs	*gpio_regs;

	u32	otype, ospeed=0, pupd, i,mode;//NOR
	int	rv;

	/*
	 * Check params
	 */
	if (!dsc || dsc->port > 8 || dsc->pin > 15) {
		rv = -EINVAL;
		goto out;
	}

	/*
	 * Depending on the role, select the appropriate io params
	 */
	switch (role) {
	case STM32F2_GPIO_ROLE_USART1:
	case STM32F2_GPIO_ROLE_USART2:
	case STM32F2_GPIO_ROLE_USART3:
	case STM32F2_GPIO_ROLE_USART4:
	case STM32F2_GPIO_ROLE_USART5:
	case STM32F2_GPIO_ROLE_USART6:
		otype  = STM32F2_GPIO_OTYPE_PP;
		ospeed = STM32F2_GPIO_SPEED_50M;
		pupd   = STM32F2_GPIO_PUPD_UP;
		mode   = STM32F2_GPIO_MODE_AF;//NOR
		break;
	case STM32F2_GPIO_ROLE_ETHERNET:
	case STM32F2_GPIO_ROLE_MCO:
		otype  = STM32F2_GPIO_OTYPE_PP;
		ospeed = STM32F2_GPIO_SPEED_100M;
		pupd   = STM32F2_GPIO_PUPD_NO;
		mode   = STM32F2_GPIO_MODE_AF;//NOR
		break;
	case STM32F2_GPIO_ROLE_SDIO:
		otype  = STM32F2_GPIO_OTYPE_PP;
		ospeed = STM32F2_GPIO_SPEED_50M;
		pupd   = STM32F2_GPIO_PUPD_NO;
		mode   = STM32F2_GPIO_MODE_AF;//NOR
		break;
	case STM32F2_GPIO_ROLE_GPOUT: //NORT start
		otype = STM32F2_GPIO_OTYPE_PP;
		ospeed = STM32F2_GPIO_SPEED_50M;
		pupd   = STM32F2_GPIO_PUPD_NO;
		mode   = STM32F2_GPIO_MODE_OUT; //NORT end
		break;
	case STM32F2_GPIO_ROLE_GPIN:
		pupd = STM32F2_GPIO_PUPD_NO;
		mode = STM32F2_GPIO_MODE_IN;
		break;
	default:
		rv = -EINVAL;
		goto out;
	}

	/*
	 * Get reg base
	 */
	gpio_regs = (struct stm32f2_gpio_regs *)io_base[dsc->port];

	/*
	 * Enable GPIO clocks
	 */
	STM32_RCC->ahb1enr |= 1 << dsc->port;

//	if (role != STM32F2_GPIO_ROLE_MCO) {
	if (role < STM32F2_GPIO_ROLE_MCO) {
		/*
		 * Connect PXy to the specified controller (role)
		 */
		i = (dsc->pin & 0x07) * 4;
		gpio_regs->afr[dsc->pin >> 3] &= ~(0xF << i);
		gpio_regs->afr[dsc->pin >> 3] |= af_val[role] << i;
	}

	i = dsc->pin * 2;

	/*
	 * Set Alternative function mode
	 */
	gpio_regs->moder &= ~(0x3 << i);
//	gpio_regs->moder |= STM32F2_GPIO_MODE_AF << i;//NOR
	gpio_regs->moder |= mode << i;//NOR:

	/*
	 * Output mode configuration
	 */
	gpio_regs->otyper &= ~(0x3 << i);
	gpio_regs->otyper |= otype << i;

	/*
	 * Speed mode configuration
	 */
	gpio_regs->ospeedr &= ~(0x3 << i);
	gpio_regs->ospeedr |= ospeed << i;

	/*
	 * Pull-up, pull-down resistor configuration
	 */
	gpio_regs->pupdr &= ~(0x3 << i);
	gpio_regs->pupdr |= pupd << i;

	rv = 0;
out:
	return rv;
}
//------------------------------------------------------
int stm32f2_status_gpio_init(void)
{
		int i;
		int rv=0;
		for(i=0;i<9;++i)
		{
			rv = stm32f2_gpio_config(&status_ind[i],STM32F2_GPIO_ROLE_GPIN);
			if(rv)
			{
				goto error;
			}
		}
		goto out;
	error:
		if(rv!=0)
		{
			printk(KERN_INFO "\n\r gpio init error\n\r");
			return rv;
		}
	out:
		printk(KERN_INFO"gpio init OK !!\n\r");
		return 0;
}
//-----------------------------------------------------
/*
void stm32_led_init(void)
{
	int rv=-1;
	struct stm32f2_gpio_dsc led1={5,10};//PF10
	struct stm32f2_gpio_dsc led2={8,4};//PI4
	if((rv=stm32f2_gpio_config(&led1,STM32F2_GPIO_ROLE_GPOUT))<0)
	{
		printk(KERN_ALERT "led1 init error:open:%d\n",rv);
	}
	if((rv=stm32f2_gpio_config(&led2,STM32F2_GPIO_ROLE_GPOUT))<0);
	{
		printk(KERN_ALERT "led2 init error:open:%d\n",rv);
	}		
}
*/
//------------------------------------------------------
int  stm32f2_gpout_set(const struct stm32f2_gpio_dsc *dsc,int state)
{
	volatile struct stm32f2_gpio_regs	*gpio_regs;
	s32					rv;
	
	if (!dsc || dsc->port > 8 || dsc->pin > 15) {
			printk(KERN_WARNING "%s: incorrect params %d.%d.\n", __func__,
				dsc ? dsc->port : -1,
				dsc ? dsc->pin  : -1);
		rv = -EINVAL;
		goto out;
	}

	gpio_regs = (struct stm32f2_gpio_regs *)io_base[dsc->port];

	if (state) {
		/* Set */
		gpio_regs->bsrrl = 1 << dsc->pin;
	} else {
		/* Reset */
		gpio_regs->bsrrh = 1 << dsc->pin ;
	}
	rv = 0;
out:
	return rv;
}
	
//------------------------------------------------
int stm32f2_gpout_toggle(const struct stm32f2_gpio_dsc *dsc)
{
        volatile struct stm32f2_gpio_regs       *gpio_regs;
        int                                     rv=0;
        if (!dsc || dsc->port > 8 || dsc->pin > 15) {
                        printk(KERN_WARNING "%s: incorrect params %d.%d.\n", __func__,
                                dsc ? dsc->port : -1,
                                dsc ? dsc->pin  : -1);
                rv = -EINVAL;
                return rv;
        }

        gpio_regs = (struct stm32f2_gpio_regs *)io_base[dsc->port];
        if(gpio_regs->odr&(1<<(dsc->pin)))
        {
                gpio_regs->bsrrh = 1<<dsc->pin;
        }
        else
        {
                gpio_regs->bsrrl = 1<<dsc->pin;
        }

        return rv;
}
//----------------------------------------------------
//----------------------------------------------------------------
int stm32f2_gpio_getValue(const struct stm32f2_gpio_dsc*dsc,
                        enum stm32f2_gpio_role role)
{
        int rv=0;
        volatile struct stm32f2_gpio_regs       *gpio_regs;
        /*
         * Check params
         */
        if (!dsc || dsc->port > 8 || dsc->pin > 15) {
                        printk(KERN_WARNING "%s: incorrect params %d.%d.\n", __func__,dsc ? dsc->port : -1, dsc ? dsc->pin :-1);
        rv = -EINVAL;
                goto out;
        }
        /* Get reg base*/
        gpio_regs = (struct stm32f2_gpio_regs *)io_base[dsc->port];
        /* GPIO clock enable*/
        STM32_RCC->ahb1enr |=1<<dsc->port;
        if(af_val[role]!= (unsigned int)-1)
        {
                printk(KERN_WARNING "just for gpio read...\n\r");
                rv= -EINVAL;
                goto out;
        }
         /*
         * Depending on the role, select the appropriate io params
         */
        switch (role) {
                 case STM32F2_GPIO_ROLE_GPIN:
                        rv =(int)(gpio_regs->idr&0xFFFF);
                        break;
                 case STM32F2_GPIO_ROLE_GPOUT:
                        rv =(int)(gpio_regs->odr&0xFFFF);
                        break;
        default:
                rv = -EINVAL;
                goto out;
        }
 if(rv & (1<<(dsc->pin)))
        {
              rv=1;
        }
        else
        {
              rv=0;
        }


out:
        return rv;
}

