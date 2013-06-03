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

#include <mach/iomux.h>
#include <mach/platform.h>
#include <mach/stm32.h>

//NOR
#define STATUS_GPIO_NUMBER 9
int stm32f2_led_init(void);
int stm32f2_tp_sync_ready_init(void);
int stm32f2_usart_short_loopback_disable(void);
int stm32f2_status_gpio_init(void);
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
	STM32F2_GPIO_AF_MAC, STM32F2_GPIO_AF_SDIO,STM32F2_GPIO_AF_MC0,(unsigned int) -1,(unsigned int) -1,//NOR
	0
};

/*
 * Configure the specified GPIO for the specified role
 */
#ifndef CONFIG_ARCH_STM32F1
static int stm32f2_gpio_config(struct stm32f2_gpio_dsc *dsc,
			       enum stm32f2_gpio_role role)
{
	volatile struct stm32f2_gpio_regs	*gpio_regs;

	u32	otype, ospeed, pupd, i,mode;//NOR
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
		//pupd = STM32F2_GPIO_PUPD_NO;
		pupd = STM32F2_GPIO_PUPD_UP;
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
#endif /* !CONFIG_ARCH_STM32F1 */

/*
 * Initialize the GPIO Alternative Functions of the STM32.
 */
void __init stm32_iomux_init(void)
{
	struct stm32f2_gpio_dsc		gpio_dsc;
	int				platform;

	/*
	 * Configure IOs depending on the board we're running on, and
	 * the configuration options we're using.
	 * Let's control platform strictly: if some of it does not need to
	 * play with iomux, it must be present in switch below (otherwise,
	 * the warning message will be printed-out)
	 */
	platform = stm32_platform_get();
	switch (platform) {
#ifndef CONFIG_ARCH_STM32F1
	/* STM32F2-based platforms */
	case PLATFORM_STM32_STM3220G_EVAL:
	case PLATFORM_STM32_STM3240G_EVAL:
// pin change -> stm32_usart.c 의 DMA channel 바꾸어주어야 하는가?.

#if defined(CONFIG_STM32_USART1)
		gpio_dsc.port = 0;
		gpio_dsc.pin  = 9;//PA9
		stm32f2_gpio_config(&gpio_dsc, STM32F2_GPIO_ROLE_USART1);

		gpio_dsc.port = 0;
		gpio_dsc.pin  = 10;//PA10
		stm32f2_gpio_config(&gpio_dsc, STM32F2_GPIO_ROLE_USART1);
#endif

#if defined(CONFIG_STM32_USART3)
		gpio_dsc.port = 1;// 
		gpio_dsc.pin  = 10;//PB10
		stm32f2_gpio_config(&gpio_dsc, STM32F2_GPIO_ROLE_USART3);

		gpio_dsc.port = 1; // 
		gpio_dsc.pin  = 11; //PB11
		stm32f2_gpio_config(&gpio_dsc, STM32F2_GPIO_ROLE_USART3);
#endif
#if defined(CONFIG_STM32_USART4)
		gpio_dsc.port = 2;// 
		gpio_dsc.pin  = 10;//PC10
		stm32f2_gpio_config(&gpio_dsc, STM32F2_GPIO_ROLE_USART4);

		gpio_dsc.port = 2; // 
		gpio_dsc.pin  = 11; //PC11
		stm32f2_gpio_config(&gpio_dsc, STM32F2_GPIO_ROLE_USART4);
#endif

//		
#if defined(CONFIG_STM32_MAC)
		do {
			static struct stm32f2_gpio_dsc mac_gpio[] = {
				{0,  1}, {0,  2}, {0,  7},
				{1,  5}, {1,  8},
				{2,  1}, {2,  2}, {2,  3}, {2,  4}, {2,  5},
				{6, 11}, {6, 13}, {6, 14},
				{7,  2}, {7,  3}, {7,  6}, {7,  7},
				{8, 10}
			};
			int	i;

			for (i = 0; i < ARRAY_SIZE(mac_gpio); i++) {
				stm32f2_gpio_config(&mac_gpio[i],
						    STM32F2_GPIO_ROLE_ETHERNET);
			}
		} while (0);
#endif
#if defined(CONFIG_MMC_ARMMMCI) || defined(CONFIG_MMC_ARMMMCI_MODULE)
		do {
			static struct stm32f2_gpio_dsc sdcard_gpio[] = {
				{2,  8}, {2,  9}, {2, 10}, {2, 11},
				{2, 12}, {3,  2}
	struct stm32f2_gpio_dsc pg7={6,7};
	};
			int	i;

			for (i = 0; i < ARRAY_SIZE(sdcard_gpio); i++) {
				stm32f2_gpio_config(&sdcard_gpio[i],
						    STM32F2_GPIO_ROLE_SDIO);
			}
		} while (0);
#endif /* defined(CONFIG_MMC_ARMMMCI) || defined(CONFIG_MMC_ARMMMCI_MODULE) */
		break;
#else
	/* STM32F1-based platforms */
	case PLATFORM_STM32_SWISSEMBEDDED_COMM:
		/*
		 * Rely on the IOMUX configuration initialized by the bootloader
		 */
		break;
#endif
	default:
		printk(KERN_WARNING "%s: unsupported platform %d\n", __func__,
			platform);
		/*
		 * Just to avoid compilation warns in case of configuration
		 * which doesn't need iomux, 'use' gpio_dsc var
		 */
		gpio_dsc.port = gpio_dsc.pin = 0;
		break;
	}
//NOR
	stm32f2_led_init();	
	stm32f2_status_gpio_init();
	stm32f2_usart_short_loopback_disable();
	stm32f2_tp_sync_ready_init();
}
//NOR
//------------------------------------------------------
int stm32f2_tp_sync_ready_init(void)
{
			
		struct stm32f2_gpio_dsc tp_sync={8,7};	
		struct stm32f2_gpio_dsc tp_ready={8,6};	
		stm32f2_gpio_config(&tp_sync,STM32F2_GPIO_ROLE_GPOUT);
		stm32f2_gpio_config(&tp_ready,STM32F2_GPIO_ROLE_GPOUT);
		stm32f2_gpout_set(&tp_sync,1);
		stm32f2_gpout_set(&tp_ready,1);
		return 0;
}

int stm32f2_usart_short_loopback_disable(void)
{
			
		struct stm32f2_gpio_dsc pg7={6,7};	
		stm32f2_gpio_config(&pg7,STM32F2_GPIO_ROLE_GPOUT);
		stm32f2_gpout_set(&pg7,1);
		return 0;
}
int stm32f2_status_gpio_init(void)
{
		struct stm32f2_gpio_dsc status_ind[STATUS_GPIO_NUMBER] = {
                {STM32F2_GPIO_PORT_H,STM32F2_GPIO_PIN_8},
                {STM32F2_GPIO_PORT_H,STM32F2_GPIO_PIN_9},
                {STM32F2_GPIO_PORT_H,STM32F2_GPIO_PIN_10},
                {STM32F2_GPIO_PORT_H,STM32F2_GPIO_PIN_11},
                {STM32F2_GPIO_PORT_H,STM32F2_GPIO_PIN_12},
                {STM32F2_GPIO_PORT_I,STM32F2_GPIO_PIN_0},
                {STM32F2_GPIO_PORT_I,STM32F2_GPIO_PIN_1},
              //  {STM32F2_GPIO_PORT_I,STM32F2_GPIO_PIN_2},
              //  {STM32F2_GPIO_PORT_I,STM32F2_GPIO_PIN_3},
		};
		int i;
		int rv=0;

		for(i=0;i<STATUS_GPIO_NUMBER;++i)
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
int stm32f2_led_init(void)
{
	struct stm32f2_gpio_dsc led1={5,10};//PF10
	struct stm32f2_gpio_dsc led2={8,4};//PI4
	stm32f2_gpio_config(&led1,STM32F2_GPIO_ROLE_GPOUT);
	stm32f2_gpio_config(&led2,STM32F2_GPIO_ROLE_GPOUT);
	return 0;
}

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

