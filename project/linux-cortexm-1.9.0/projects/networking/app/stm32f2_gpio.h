#ifndef __STM32F4_GPIO_H
#define __STM32F4_GPIO_H

#define STM32_PERIPH_BASE	0x40000000
#define STM32_AHB1PERITH_BASE	(STM32_PERIPH_BASE + 0x00020000)
#define STM32F2_GPIOA_BASE	(STM32_AHB1PERITH_BASE + 0x0000)
#define STM32F2_GPIOB_BASE	(STM32_AHB1PERITH_BASE + 0x0400)
#define STM32F2_GPIOC_BASE	(STM32_AHB1PERITH_BASE + 0x0800)
#define STM32F2_GPIOD_BASE	(STM32_AHB1PERITH_BASE + 0x0C00)
#define STM32F2_GPIOE_BASE	(STM32_AHB1PERITH_BASE + 0x1000)
#define STM32F2_GPIOF_BASE	(STM32_AHB1PERITH_BASE + 0x1400)
#define STM32F2_GPIOG_BASE	(STM32_AHB1PERITH_BASE + 0x1800)
#define STM32F2_GPIOH_BASE	(STM32_AHB1PERITH_BASE + 0x1C00)
#define STM32F2_GPIOI_BASE	(STM32_AHB1PERITH_BASE + 0x2000)
#ifdef CONFIG_ARCH_STM32F1
#define STM32_RCC_BASE		(STM32_AHB1PERITH_BASE + 0x1000) /* STM32F1 */
#else
#define STM32_RCC_BASE		(STM32_AHB1PERITH_BASE + 0x3800) /* STM32F2 */
#endif
#define STM32_RCC	((volatile struct stm32_rcc_regs *)STM32_RCC_BASE)


/*
 * GPIO configuration mode
 */
#define STM32F2_GPIO_MODE_IN	0x00
#define STM32F2_GPIO_MODE_OUT	0x01
#define STM32F2_GPIO_MODE_AF	0x02
#define STM32F2_GPIO_MODE_AN	0x03

/*
 * GPIO output type
 */
#define STM32F2_GPIO_OTYPE_PP	0x00
#define STM32F2_GPIO_OTYPE_OD	0x01

/*
 * GPIO output maximum frequency
 */
#define STM32F2_GPIO_SPEED_2M	0x00
#define STM32F2_GPIO_SPEED_25M	0x01
#define STM32F2_GPIO_SPEED_50M	0x02
#define STM32F2_GPIO_SPEED_100M	0x03

/*
 * GPIO pullup, pulldown configuration
 */
#define STM32F2_GPIO_PUPD_NO	0x00
#define STM32F2_GPIO_PUPD_UP	0x01
#define STM32F2_GPIO_PUPD_DOWN	0x02

/*
 * AF7 selection
 */
#define STM32F2_GPIO_AF_USART1	0x07
#define STM32F2_GPIO_AF_USART2	0x07
#define STM32F2_GPIO_AF_USART3	0x07

/*
 * AF8 selection
 */
#define STM32F2_GPIO_AF_USART4	0x08
#define STM32F2_GPIO_AF_USART5	0x08
#define STM32F2_GPIO_AF_USART6	0x08

/*
 * AF11 selection
 */
#define STM32F2_GPIO_AF_MAC	0x0B

/*
 * AF12 selection
 */
#define STM32F2_GPIO_AF_FSMC	0x0C
#define STM32F2_GPIO_AF_SDIO	0x0C
#define STM32F2_GPIO_AF_MC0     0x0D 
/*
 * GPIO register map
 */
struct stm32f2_gpio_regs {
	unsigned int	moder;		/* GPIO port mode			      */
	unsigned int	otyper;		/* GPIO port output type		      */
	unsigned int	ospeedr;	/* GPIO port output speed		      */
	unsigned int	pupdr;		/* GPIO port pull-up/pull-down		      */
	unsigned int	idr;		/* GPIO port input data			      */
	unsigned int	odr;		/* GPIO port output data		      */
	unsigned short	bsrrl;		/* GPIO port bit set/reset low		      */
	unsigned short	bsrrh;		/* GPIO port bit set/reset high		      */
	unsigned int	lckr;		/* GPIO port configuration lock		      */
	unsigned int	afr[2];		/* GPIO alternate function		      */
};
/*
 * GPIO ports
 */
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
/* Gpio roles (alternative functions); role determines by whom GPIO is used
 */
enum stm32f2_gpio_role {
	STM32F2_GPIO_ROLE_USART1,	/* USART1			      */
	STM32F2_GPIO_ROLE_USART2,	/* USART2			      */
	STM32F2_GPIO_ROLE_USART3,	/* USART3			      */
	STM32F2_GPIO_ROLE_USART4,	/* USART4			      */
	STM32F2_GPIO_ROLE_USART5,	/* USART5			      */
	STM32F2_GPIO_ROLE_USART6,	/* USART6			      */
	STM32F2_GPIO_ROLE_ETHERNET,	/* MAC				      */
	STM32F2_GPIO_ROLE_SDIO,		/* SDIO				      */
	STM32F2_GPIO_ROLE_MCO,		/* MC external output clock	      */
	//
	STM32F2_GPIO_ROLE_GPOUT,	/* GPOUT			      */ //NOR
	STM32F2_GPIO_ROLE_GPIN,
	STM32F2_GPIO_ROLE_LAST		/* for internal usage, must be last   *///NOR
	
};

//
/*
 * RCC register map
 */
struct stm32_rcc_regs {
	unsigned int	cr;		/* RCC clock control			      */
#ifndef CONFIG_ARCH_STM32F1
	unsigned int	pllcfgr;	/* RCC PLL configuration		      */
#endif
	unsigned int	cfgr;		/* RCC clock configuration		      */
	unsigned int	cir;		/* RCC clock interrupt			      */
#ifndef CONFIG_ARCH_STM32F1
	unsigned int	ahb1rstr;	/* RCC AHB1 peripheral reset		      */
	unsigned int	ahb2rstr;	/* RCC AHB2 peripheral reset		      */
	unsigned int	ahb3rstr;	/* RCC AHB3 peripheral reset		      */
	unsigned int	rsv0;
	unsigned int	apb1rstr;	/* RCC APB1 peripheral reset		      */
	unsigned int	apb2rstr;	/* RCC APB2 peripheral reset		      */
	unsigned int	rsv1[2];
#else
	unsigned int	apb2rstr;	/* RCC APB2 peripheral reset		      */
	unsigned int	apb1rstr;	/* RCC APB1 peripheral reset		      */
#endif

	unsigned int	ahb1enr;	/* RCC AHB1 peripheral clock enable	      */
#ifndef CONFIG_ARCH_STM32F1
	unsigned int	ahb2enr;	/* RCC AHB2 peripheral clock enable	      */
	unsigned int	ahb3enr;	/* RCC AHB3 peripheral clock enable	      */
	unsigned int	rsv2;
	unsigned int	apb1enr;	/* RCC APB1 peripheral clock enable	      */
	unsigned int	apb2enr;	/* RCC APB2 peripheral clock enable	      */
#else
	unsigned int	apb2enr;	/* RCC APB2 peripheral clock enable	      */
	unsigned int	apb1enr;	/* RCC APB1 peripheral clock enable	      */
#endif

#ifndef CONFIG_ARCH_STM32F1
	unsigned int	rsv3[2];
	unsigned int	ahb1lpenr;	/* RCC AHB1 periph clk enable in low pwr mode */
	unsigned int	ahb2lpenr;	/* RCC AHB2 periph clk enable in low pwr mode */
	unsigned int	ahb3lpenr;	/* RCC AHB3 periph clk enable in low pwr mode */
	unsigned int	rsv4;
	unsigned int	apb1lpenr;	/* RCC APB1 periph clk enable in low pwr mode */
	unsigned int	apb2lpenr;	/* RCC APB2 periph clk enable in low pwr mode */
	unsigned int	rsv5[2];
#endif
	unsigned int	bdcr;		/* RCC Backup domain control		      */
	unsigned int	csr;		/* RCC clock control & status		      */
#ifndef CONFIG_ARCH_STM32F1
	unsigned int	rsv6[2];
	unsigned int	sscgr;		/* RCC spread spectrum clock generation	      */
	unsigned int	plli2scfgr;	/* RCC PLLI2S configuration		      */
#endif
};

/*
 * GPIO descriptor
 */
struct stm32f2_gpio_dsc {
	unsigned int		port;		/* GPIO port			      */
	unsigned int		pin;		/* GPIO pin		0 ~15	      */
};
//
#define STATUS_GPIO_NUMBER 7

//------------------------------------
int stm32f2_gpio_config(struct stm32f2_gpio_dsc *dsc,enum stm32f2_gpio_role role);
int stm32f2_gpout_set(const struct stm32f2_gpio_dsc *dsc,int state);
int stm32f2_gpout_toggle(const struct stm32f2_gpio_dsc *dsc);
int stm32f2_gpio_getValue(const struct stm32f2_gpio_dsc*dsc,
                        enum stm32f2_gpio_role role);
//------------------------------------
int stm32f2_status_gpio_init(void);
int stm32f2_led_init(void);
int stm32f2_usart_short_loopback_disable(int);
int stm32f2_tp_sync_ready_init(void);



#endif// __STM32F4_GPIO_H
