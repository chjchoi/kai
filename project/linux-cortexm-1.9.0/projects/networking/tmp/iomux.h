/*
 * (C) Copyright 2011
 * Emcraft Systems, <www.emcraft.com>
 * Yuri Tikhonov <yur@emcraft.com>
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

#ifndef _MACH_STM32_IOMUX_H_
#define _MACH_STM32_IOMUX_H_

//#include <linux/init.h>
//
/*
 * GPIO registers bases
 */
#define STM32F2_GPIOA_BASE      (STM32_AHB1PERITH_BASE + 0x0000)
#define STM32F2_GPIOB_BASE      (STM32_AHB1PERITH_BASE + 0x0400)
#define STM32F2_GPIOC_BASE      (STM32_AHB1PERITH_BASE + 0x0800)
#define STM32F2_GPIOD_BASE      (STM32_AHB1PERITH_BASE + 0x0C00)
#define STM32F2_GPIOE_BASE      (STM32_AHB1PERITH_BASE + 0x1000)
#define STM32F2_GPIOF_BASE      (STM32_AHB1PERITH_BASE + 0x1400)
#define STM32F2_GPIOG_BASE      (STM32_AHB1PERITH_BASE + 0x1800)
#define STM32F2_GPIOH_BASE      (STM32_AHB1PERITH_BASE + 0x1C00)
#define STM32F2_GPIOI_BASE      (STM32_AHB1PERITH_BASE + 0x2000)

/*
 * GPIO configuration mode
 */
#define STM32F2_GPIO_MODE_IN    0x00
#define STM32F2_GPIO_MODE_OUT   0x01
#define STM32F2_GPIO_MODE_AF    0x02
#define STM32F2_GPIO_MODE_AN    0x03

/*
 * GPIO output type
 */
#define STM32F2_GPIO_OTYPE_PP   0x00
#define STM32F2_GPIO_OTYPE_OD   0x01

/*
 * GPIO output maximum frequency
 */
#define STM32F2_GPIO_SPEED_2M   0x00
#define STM32F2_GPIO_SPEED_25M  0x01
#define STM32F2_GPIO_SPEED_50M  0x02
#define STM32F2_GPIO_SPEED_100M 0x03

/*
 * GPIO pullup, pulldown configuration
 */
#define STM32F2_GPIO_PUPD_NO    0x00
#define STM32F2_GPIO_PUPD_UP    0x01
#define STM32F2_GPIO_PUPD_DOWN  0x02

/*
 * AF7 selection
 */
#define STM32F2_GPIO_AF_USART1  0x07
#define STM32F2_GPIO_AF_USART2  0x07
#define STM32F2_GPIO_AF_USART3  0x07

/*
 * AF8 selection
 */
#define STM32F2_GPIO_AF_USART4  0x08
#define STM32F2_GPIO_AF_USART5  0x08
#define STM32F2_GPIO_AF_USART6  0x08
/*
 * AF11 selection
 */
#define STM32F2_GPIO_AF_MAC     0x0B

/*
 * AF12 selection
 */
#define STM32F2_GPIO_AF_FSMC    0x0C
#define STM32F2_GPIO_AF_SDIO    0x0C
#define STM32F2_GPIO_AF_MC0     0x0D
/*
 * GPIO register map
 */
struct stm32f2_gpio_regs {
        unsigned int    moder;  /* GPIO port mode                             */
        unsigned int   otyper;  /* GPIO port output type                      */
        unsigned int  ospeedr;  /* GPIO port output speed                     */
        unsigned int  pupdr;    /* GPIO port pull-up/pull-down                */
        unsigned int     idr;   /* GPIO port input data                       */
        unsigned int     odr;   /* GPIO port output data                      */
        unsigned short     bsrrl;          /* GPIO port bit set/reset low                */
        unsigned short     bsrrh;          /* GPIO port bit set/reset high               */
        unsigned int     lckr;           /* GPIO port configuration lock               */
        unsigned int     afr[2];         /* GPIO alternate function                    */
};

/*
 * GPIO roles (alternative functions); role determines by whom GPIO is used
 */
enum stm32f2_gpio_role {
        STM32F2_GPIO_ROLE_USART1,       /* USART1                             */
        STM32F2_GPIO_ROLE_USART2,       /* USART2                             */
        STM32F2_GPIO_ROLE_USART3,       /* USART3                             */
        STM32F2_GPIO_ROLE_USART4,       /* USART4                             */
        STM32F2_GPIO_ROLE_USART5,       /* USART5                             */
        STM32F2_GPIO_ROLE_USART6,       /* USART6                             */
        STM32F2_GPIO_ROLE_ETHERNET,     /* MAC                                */
        STM32F2_GPIO_ROLE_SDIO,         /* SDIO                               */
        STM32F2_GPIO_ROLE_MCO,          /* MC external output clock           */
        STM32F2_GPIO_ROLE_GPOUT,
        STM32F2_GPIO_ROLE_GPIN,
};

/*
 * GPIO descriptor
 */
struct stm32f2_gpio_dsc {
        unsigned int             port;           /* GPIO port                          */
        unsigned int             pin;            /* GPIO pin                           */
};

//
enum stm32f2_gpio_port {
	STM32F2_GPIO_PORT_A	= 0,
	STM32F2_GPIO_PORT_B,
	STM32F2_GPIO_PORT_C,
	STM32F2_GPIO_PORT_D,
	STM32F2_GPIO_PORT_E,
	STM32F2_GPIO_PORT_F,
	STM32F2_GPIO_PORT_G,
	STM32F2_GPIO_PORT_H,
	STM32F2_GPIO_PORT_I
};

/*
 * GPIO port pins
 */
enum stm32f2_gpio_pin {
	STM32F2_GPIO_PIN_0	= 0,
	STM32F2_GPIO_PIN_1,
	STM32F2_GPIO_PIN_2,
	STM32F2_GPIO_PIN_3,
	STM32F2_GPIO_PIN_4,
	STM32F2_GPIO_PIN_5,
	STM32F2_GPIO_PIN_6,
	STM32F2_GPIO_PIN_7,
	STM32F2_GPIO_PIN_8,
	STM32F2_GPIO_PIN_9,
	STM32F2_GPIO_PIN_10,
	STM32F2_GPIO_PIN_11,
	STM32F2_GPIO_PIN_12,
	STM32F2_GPIO_PIN_13,
	STM32F2_GPIO_PIN_14,
	STM32F2_GPIO_PIN_15
};

void  stm32_iomux_init(void);
void  stm32_led_init(void);//NOR
int stm32f2_gpout_set(const struct stm32f2_gpio_dsc *dsc,int);
int stm32f2_status_gpio_init(void);
int stm32f2_gpout_toggle(const struct stm32f2_gpio_dsc *);
int stm32f2_gpio_getValue(const struct stm32f2_gpio_dsc *,enum stm32f2_gpio_role);
#endif
