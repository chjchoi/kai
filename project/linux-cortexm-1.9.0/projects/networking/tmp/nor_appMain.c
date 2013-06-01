#include <stdio.h>
#include "nor_appMain.h"
#include <command.h>
#include <asm/arch/stm32f2_gpio.h>
//#include <common.h>
//out
//
#define SCL(n) stm32f2_gpout_set(&lb_scl,(n))
#define SCL_TOGGLE() stm32f2_gpout_toggle(&lb_scl)
#define GET_SCL() stm32f2_gpio_getValue(&lb_scl,STM32F2_GPIO_ROLE_GPOUT)
//
#define SDA(n) stm32f2_gpout_set(&lb_sda,(n))
#define SDA_TOGGLE() stm32f2_gpout_toggle(&lb_sda)
#define GET_SDA() stm32f2_gpio_getValue(&lb_sda, STM32F2_GPIO_ROLE_GPOUT)

//
#define GPIO(n) stm32f2_gpout_set(&gpio_lb,(n))
//
#define GET_HIC_READY() stm32f2_gpio_getValue(&dsp_hic_ready,STM32F2_GPIO_ROLE_GPIN);
#define LED1(n) stm32f2_gpout_set(&led1,(n))
#define LED2(n) stm32f2_gpout_set(&led2,(n))
#define LED1_TOGGLE() stm32f2_gpout_toggle(&led1);
#define LED2_TOGGLE() stm32f2_gpout_toggle(&led2);
#define LED1ALIAS(n) stm32f2_gpout_set(&led1Alias,(n))
#define LED2ALIAS(n) stm32f2_gpout_set(&led2Alias,(n))
//
#define LB_MUXA(n) stm32f2_gpout_set(&lb_mux_selA,(n))
#define LB_MUXB(n) stm32f2_gpout_set(&lb_mux_selB,(n))
#define LB_MUXC(n) stm32f2_gpout_set(&lb_mux_selC,(n))
//
#define TP_SYNC(n) stm32f2_gpout_set(&dsp_tp_sync,(n))
#define TP_READY(n) stm32f2_gpout_set(&dsp_tp_ready,(n))
#define TP_SYNC_TOGGLE() stm32f2_gpout_toggle(&dsp_tp_sync)
#define TP_READY_TOGGLE() stm32f2_gpout_toggle(&dsp_tp_ready)
#define GET_TP_READY() stm32f2_gpio_getValue(&dsp_tp_ready,STM32F2_GPIO_ROLE_GPOUT)
#define GET_TP_SYNC()  stm32f2_gpio_getValue(&dsp_tp_sync,STM32F2_GPIO_ROLE_GPOUT)
//in
#define GET_LB_IN() stm32f2_gpio_getValue(&lb_in,STM32F2_GPIO_ROLE_GPIN)
#define GET_STATUS_IND(n) stm32f2_gpio_getValue(&status_ind[n],STM32F2_GPIO_ROLE_GPIN)

//
extern char console_buffer[CONFIG_SYS_CBSIZE];
extern int runMode;
//in
extern struct stm32f2_gpio_dsc status_ind[9];
extern struct stm32f2_gpio_dsc lb_in;
extern struct stm32f2_gpio_dsc dsp_hic_ready;
//out
extern struct stm32f2_gpio_dsc led1;
extern struct stm32f2_gpio_dsc led2;
extern struct stm32f2_gpio_dsc led1Alias;
extern struct stm32f2_gpio_dsc led2Alias;
extern struct stm32f2_gpio_dsc lb_scl;
extern struct stm32f2_gpio_dsc lb_sda;
extern struct stm32f2_gpio_dsc gpio_lb;
extern struct stm32f2_gpio_dsc lb_mux_selA;
extern struct stm32f2_gpio_dsc lb_mux_selB;
extern struct stm32f2_gpio_dsc lb_mux_selC;
extern struct stm32f2_gpio_dsc lb_tty;

extern struct stm32f2_gpio_dsc dsp_tp_ready;
extern struct stm32f2_gpio_dsc dsp_tp_sync;
//
extern volatile struct stm32_usart_regs *nor_usart_regs[6];
extern struct stm32f2_gpio_dsc lb_tty;
int recievedTTy[6]={1,1,1,1,1,1};
//
int usart_port_mode=0;
int usart_test_total_count=0;
int nor_usart_test(int);
extern int nor_serial_putc(const char ,int);
//NOR end

//-------------------------------------------------------
int nor_Show_Diag_Menu()
{
	printf("\n\r 'z' for switch Diagnostic <-->uboot command\n");
        printf("a)LED1 on   b)LED1 off   c)LED2 on   d)LED2 off\n");
	printf("e)show D/I  f)show D/I LB   g)show D/I Mux \n");
	printf("h)menu      j)120Hz         k)usart test long loop\n");
	printf("l)usart test internal loop  m)A20 toggle\n");
	return 0;
}
//------------------------------------------------------
int nor_run_command(const char * cmd,int flag)
{
	int i,rv=0;
	int ll;
	if(runMode!=1)//DIAGNOSTIC아니면 
	{
		return 0;
	}	
	if(flag!=CMD_FLAG_REPEAT)
	{
		
              gpio_lb_init();
	}
	switch(cmd[0])
	{
                case 0x03: //ctrl-c 현재는 안됨 
			printf("                \r");
                     break;
		case 'a':
			if(flag!=CMD_FLAG_REPEAT)
			{
			  
			   LED1(1);
			   LED1ALIAS(1);
			   printf("Led1 On\r");
			}
			break;
		case 'b':

			if(flag!=CMD_FLAG_REPEAT)
			{
			   LED1(0);
			   LED1ALIAS(0);
			   printf("Led1 Off\r");
			}
			break;
		case 'c':
			if(flag!=CMD_FLAG_REPEAT)
			{
			   LED2(1);
			   LED2ALIAS(1);
			   printf("Led2 On\r");
			}
			break;
		case 'd':
			if(flag!=CMD_FLAG_REPEAT)
			{
				LED2ALIAS(0);
				LED2(0);
				printf("Led2 Off \r");
			}
			break;
		case 'e':
		//	nor_Gpio_Init();
			//char str[] 배열로 처리하고 printf("%s",str)하면 안됨
			
			for(i=0;i<9;++i)
			{
				rv=GET_STATUS_IND(i);		
				if(rv==1)
				{
					printf("1");
				}
				else
				{
					printf("0");
				}
			}
			 rv =GET_HIC_READY();
                                if(rv==1)
                                {
                                    printf("1");
                                }
                                else
                                {
                                   printf("0");
                                }

			printf("\r");
			
			break;
		case 'f':
			nor_gpio_lb();
			break;
		case 'g':
		        nor_gpio_mux();	
			break;
                case 'h':
		
			if(flag!=CMD_FLAG_REPEAT)
			{
				nor_Show_Diag_Menu();
			}
		
			break;	
		case 'i':
			//nor_usart_test();
			break;
		case 'j':
			nor_120Hz_test();
			break;
		case 'k':
			ll=1;
                
			if(flag!=CMD_FLAG_REPEAT)
			{
				recievedTTy[usart_port_mode] =1;
			}
			nor_usart_test(ll);
			break;
		case 'l':
			ll=0;
			if(flag!=CMD_FLAG_REPEAT)
			{
				recievedTTy[usart_port_mode] =1;
			}
			nor_usart_test(ll);
			break;
		case 'm':
			if(flag!=CMD_FLAG_REPEAT)
			{
				A20Toggle();
			}
			break;
		case 'n':
			if(flag!=CMD_FLAG_REPEAT)
			{
				AddrTest();
			}
			break;
		case 'z':
			 if(flag!=CMD_FLAG_REPEAT)
                        {
                                nor_Show_Diag_Menu();
                        }
			break;
		default:
			break;
	}
	return 0;
}
//------------------------------------------------------
void A20Toggle(void)
{
	//test
	static struct stm32f2_gpio_dsc a20 ={4,4};
	int rv;
	int i;
	rv = stm32f2_gpio_config(&a20,STM32F2_GPIO_ROLE_GPOUT);
	for(i=0;i<1000;++i)
	{
		stm32f2_gpout_set(&a20,1);
		udelay(100);
		stm32f2_gpout_set(&a20,0);
		udelay(100);
	}
	//recovery
	stm32f2_gpio_config(&a20,STM32F2_GPIO_ROLE_FSMC);

}

void AddrTest(void)
{
	int  i;
	volatile unsigned int * pStartAddr=0x64000000;
	for(i=(int)0x64000000 ;i<=(int)0x647FFFFC;i+=4,pStartAddr+=1)
	{
		*(pStartAddr)=i;
	
	}
	printf("done!!");
}
//-----------------------------------------------------
void LED2ON(void)
{
	LED2(1);
}	
//------------------------------------------------------
int test(void)
{
	return 0;
}
//--------------------------------------------------------
int nor_usart_test(int ll)
{
	int rv=0;
	int i;
	char TestCode[6]={0x11,0x22,0x33,0x44,0x55,0x66};
	
	rv= stm32f2_gpout_set(&lb_tty,ll);
	if(rv!=0)
		return -1;
        for(i=0;i<6;++i)
	{
//		if(i!=0&&i!=4/*||i==2*/)
		if(i!=0&&i!=5)
		{
			continue;
		}
		rv = nor_serial_putc(TestCode[i],0);
                if(rv>=0)
		{
			printf("USART%d:SEND %x",0,TestCode[i]);
		}
		rv=    nor_serial_putc(TestCode[i],5);
		if(rv>=0)
		{
			printf("USART%d:SEND %x",5,TestCode[i]);
		}
		

	}	
	__udelay(10000);
	for(i=0;i<6;++i)
	{
		if(i!=0&&i!=5)
		{
			continue;
		}
        	if(nor_serial_tstc(0))
		{	
			rv=nor_serial_getc(0);
			if(rv>=0)
			printf("USART%d:REC %x\r",0,rv&0xFF);
		}
		
        	if(nor_serial_tstc(0))
		{	
			rv=nor_serial_getc(5);
			if(rv>=0)
			printf("USART%d:REC %x\r",5,rv&0xFF);
		}
	}
	return 0;
/*
	int rv=0;
	int i;
	int v=0;
	int countUsartTxCheck=0;
	char TestCode[4] ={0x00,0x55,0xaa,0xff};
	if(usart_port_mode>=6)
	{
		usart_port_mode=0;
		usart_test_total_count++;
		if(usart_test_total_count>=4)
		{
			usart_test_total_count=0;
		}
	}
	v= stm32f2_gpout_set(&lb_tty,0);
	if(v!=0)
		return -1;
       	if(recievedTTy[usart_port_mode]==1)
	{
		if(usart_port_mode==1 ||usart_port_mode==2)// 2= console ,1= error
		{
			return;
		}
	        	nor_serial_putc(TestCode[usart_test_total_count],usart_port_mode);
		  	printf("USART%d:SEND %x",usart_port_mode,TestCode[usart_test_total_count]);
	           	recievedTTy[usart_port_mode]=0;
	}
	for(i=0;i<6;++i)
	{
	    if(recievedTTy[i]==0)
	    {
		while(countUsartTxCheck<10)
		{
			if(nor_serial_tstc(i))
			{
		        	rv=nor_serial_getc(i);
		        	printf("USART%d:REC %x\r",i,rv&0xFF);
		        	recievedTTy[i]=1;
                        }
	 		countUsartTxCheck++;
		}
	    }
	}
	usart_port_mode++;
	return rv;	
*/
}
//----------------------------------------
int countForGpioMux=0;
int smallCount=0;
int nor_gpio_mux()
{
	int i;
	int rv=0;
	long timeTick=0;
	int minTick=100;//100mS
	int countTime_100mS=0;
        //
	LB_MUXC(0);
	LB_MUXB(0);
	reset_timer();
	if(countForGpioMux>=10)
	{
		countForGpioMux=0;
	}
	else if(countForGpioMux==0)
	{
		printf("nor_gpio_mux(ready,sync)\n\r");
	}
	while(1)
	{
                timeTick= get_timer(0);//1mS Tick timer
		if(timeTick>=minTick)//100mS
		{
			reset_timer();
			countTime_100mS++;
		} 
		if(countTime_100mS >=10) //1000mS
		{
			countTime_100mS=0;
			// out data
			TP_SYNC_TOGGLE();
			smallCount++;
			if(smallCount==2)
			{
				smallCount=0;
				TP_READY_TOGGLE();

			}		
			__udelay(1000);
			printf("out:%d",GET_TP_READY());
			printf("%d",GET_TP_SYNC());
			//
			printf("  in:");
			LB_MUXA(0);
			__udelay(1000);
			if(GET_LB_IN())//ready
			{
				printf("0");//반전
			}
			else
			{
				printf("1");
			}
			LB_MUXA(1);
			__udelay(1000);
			printf("%d\n\r",GET_LB_IN());//sync
			countForGpioMux++;
			break;
		}
	}
	return 0;		
}
//--------------------------------------
int countForGpioLB=0;
int nor_gpio_lb()
{
	int i;
	//int sdaData[10]={0,1,1,1,1,1,1,1,1,1};
	int sdaData[10]={1,0,0,0,0,0,0,0,0,0,};
        int rv=-1;
        long timeTick=0;
        int minTick =100;//100mS?
	int countTime_100mS=0;
	SCL(0);
	GPIO(0);
	__udelay(1000);
        rv=timer_init();
        reset_timer();
	if(countForGpioLB >=10)
	{
		countForGpioLB =0;
	}
	else if(countForGpioLB==0)
	{
		printf("-------gpio loop back test ---\n\r");
	}
        while(1)//0 ~9 10bit
        {
                //1tick= 1mS teick
                timeTick= get_timer(0);//mS timer

		if(timeTick>=minTick)//100mS
		{
			reset_timer();
			countTime_100mS++;
		}
		if(countTime_100mS >=10)//1000mS
		{
		    countTime_100mS=0;
		    if(sdaData[countForGpioLB]==1)
		    {
		         SDA(1);  
		    }
		    else
		    {
			SDA(0);
		    }
		    SCL(1);
		    __udelay(1000);
                    SCL(0);
		    __udelay(1000);
		    GPIO(1);
		    __udelay(1000);
		    GPIO(0); 
                    __udelay(1000);
                    
                                for(i=0;i<9;++i)
                               {
                                     rv=GET_STATUS_IND(i);
                                     if(rv==1)
                                     {
                                        printf("1");
                                     }
                                     else
                                     {
                                        printf("0");
                                     }
                                }
				rv =GET_HIC_READY();
				if(rv==1)
				{
				    printf("1");
				}
				else
				{
				   printf("0");
				}
                                 printf("\r");
                  countForGpioLB++;
		  break;
               }//if
        }//while
	return 0;
}//function body
//---------------------------------------------
int gpio_lb_init()
{
	int i;
	SDA(1);	
	for(i=0;i<20;++i)
	{
		    SCL(1);
                    __udelay(10);
                    SCL(0);
                    __udelay(1000);
	}
        GPIO(1);
        __udelay(1000);
	return 0;
}
//----------------------------------------
int nor_120Hz_test()
{
	TP_SYNC(0);
	__udelay(8300);
        TP_SYNC(1);
	__udelay(33);	
}
