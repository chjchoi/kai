//diagnostic
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "stm32f2_gpio.h"
#include <time.h>
static struct termios initial_settings,new_settings;
static int peek_character =-1;
void init_keyboard();
void close_keyboard();
int Usart_Test_init(void);

int doJob(char);
int kbhit();
int readch();
//
int do_LED(int ledN,int val);
int show_DI(void);
int sendUsart(int);
int etc_init(void);
int nor_120(void);
static int slb=1;
static long ccount=0;
//
extern struct stm32f2_gpio_dsc status_ind[];
extern struct stm32f2_gpio_dsc short_loopback;
extern struct stm32f2_gpio_dsc tp_sync;
extern struct stm32f2_gpio_dsc tp_ready;
extern struct stm32f2_gpio_dsc led1;//PF10
extern struct stm32f2_gpio_dsc led2;//PI4

//
#define INPUT_SIZE 20
#define SEND_BUF_SIZE 26
#define SEND_BUF_SHORT_SIZE 10
static int fd1,fd3,fd5,fd6;
static char ch=0;
static	int rv=0;
static struct timespec  MyTimespec;
static struct timespec  RecTimespec;
//----------------------------------------------
int main(void)
{
	struct timespec myTimespec;
	init_keyboard();
        nor_Show_Diag_Menu(slb);	
	Usart_Test_init();
	etc_init();
	while(1)
	{
		if(rv==0)
		{
			scanf("%c",&ch);
			if(ch=='z')
			{
				printf("quit!!\n");
				close_keyboard();
				close(fd1);close(fd3);close(fd5);close(fd6);
				return 0;
			}
		}
		rv=doJob(ch);
	//	ch=0x00;
	}//while
END:
	close_keyboard();
	close(fd1);close(fd3);close(fd5);close(fd6);
	return 0;
}//main
//-------------------------
int etc_init(void)
{
	stm32f2_usart_short_loopback_disable();
	stm32f2_status_gpio_init();
	stm32f2_tp_sync_ready_init();
	stm32f2_led_init();
	MyTimespec.tv_nsec =(long)8333333;
	MyTimespec.tv_sec=0;
	RecTimespec.tv_nsec =0;
	RecTimespec.tv_sec =0;	
	return 0;
}
	
//----------------------------
int Usart_Test_init(void)
{
	
	struct termios term;
	pid_t pid1=100;
	pid_t pid3=100;
	pid_t pid5=100;
	pid_t pid6=100;
	memset(&term,0,sizeof(term));
	term.c_iflag=0;
	term.c_oflag=0;
	term.c_cflag=CS8|CREAD|CLOCAL;
	term.c_lflag=0;
	term.c_cc[VMIN]=1;
	term.c_cc[VTIME]=5;

	fd1 =open("/dev/ttyS0",O_WRONLY|O_NONBLOCK);
        fd3 =open("/dev/ttyS2",O_WRONLY|O_NONBLOCK);
        fd5 =open("/dev/ttyS4",O_WRONLY|O_NONBLOCK);
        fd6 =open("/dev/ttyS5",O_WRONLY|O_NONBLOCK);

	cfsetospeed(&term,B115200);
	cfsetispeed(&term,B115200);
	tcsetattr(fd1,TCSANOW,&term);
	tcsetattr(fd3,TCSANOW,&term);
	tcsetattr(fd5,TCSANOW,&term);
	tcsetattr(fd6,TCSANOW,&term);
	pid1=vfork();
	switch(pid1)
	{
		case -1: //fork fail
			close(fd1);close(fd3);close(fd5);close(fd6);
			return -1;
		case 0://child
			execl("/root/ttyK","ttyK","1",0);
			break;
		default:
			pid3=vfork();
			if(pid3==0)
			{
				execl("/root/ttyK","ttyK","3",0);
			}
			else
			{
			}
			break;
	}
	pid5=vfork();
	switch(pid5)
	{
		case -1: //fork fail
			close(fd1);close(fd3);close(fd5);close(fd6);
			return -1;
		case 0://child
			execl("/root/ttyK","ttyK","5",0);
			break;
		default:
			pid6=vfork();
			if(pid6==0)
			{
				execl("/root/ttyK","ttyK","6",0);
			}
			else
			{
			}
			break;
	}
	return 0;
	
}
//--------------------------
int doJob(char ch)
{
		int ret=0;
		int rv=0;	
	   	switch(ch)// rv=0 .. 1time. else ..
	    	{
			case '0':   rv=0; ch=0; return 0;
			case 'a':   rv=0;do_LED(1,0);break;//led1 on
			case 'b':   rv=0;do_LED(1,1);break;//led1 off
			case 'c':   rv=0;do_LED(2,0);break;//led2 on
			case 'd':   rv=0;do_LED(2,1);break;//led2 off
			case 'e':   rv=show_DI();break;
			case 'f':              ;break;
			case 'g':
			case 'h':   rv=0;nor_Show_Diag_Menu();break;
			case 'i':             ;break;
			case 'j':  ret=nor_120();
				   if(ret<0)
				   { ch=0;}
				   else
				   {
					rv=1;
				   }
				   break;
			case 'k':             ;break;
			case 'm':  rv=0;slb=1;nor_Show_Diag_Menu(slb);break;
			case 'n':  rv=0;slb=0;nor_Show_Diag_Menu(slb);break;
			case 'q':  rv=0xFF;goto END;
			case 'r':  rv=0;tpReady(0);break;
			case 's':  rv=0;tpReady(1);break;
	      	        case '1':  rv=0;sendUsart(1);break; 
			case '2':               ;break;
			case '3':  rv=0;sendUsart(3);break; 
			case '4':               ;break;
			case '5':               ;break;
			case '6':		;break;
			case 'z': return 0xFF;
			default:
			break;
		}
END:
		return rv;
error:
		return -1;
}
//r
void tpReady(int val)
{
	stm32f2_gpout_set(&tp_ready,val);	
}
int nor_120(void)
{
	int rv;
	rv=nanosleep(&MyTimespec,&RecTimespec);
	stm32f2_gpout_toggle(&tp_sync);
	if(ccount++>700)
	{
		ccount=0;
		printf("quit 120Hz!!");
		return -1;
	}	
	if(rv<0)
	{
		printf("error sleep\n");
		return -1;
	}
			
//	printf("%d\n",RecTimespec.tv_nsec);
	return 0;
//	printf("%d\n",pRecTimespec->tv_nsec);
}
int nor_Show_Diag_Menu(int val)
{
	if(val=1){
	printf("\n\r-------HW test Program---[No short Usart LB]-----\n");
	}
	else{
	printf("\n\r-------HW test Program---[short Usart LB]-----\n");
	}
	printf("a) LED1 on   b) LED1 off  c) LED2 on  d) LEd2 off\n");
	printf("e) show D/I  f) show D/I LB   g) show D/I Mux \n");
	printf("h) menu      j) 120Hz      k) usart short lb test \n");
	printf("q) quit\n    m) ttyS0 echo n)ttyS2 echo \n");
	printf("r) tp_ready on s)tp_ready off \n");
	printf("1) usart1 test code send,  3) usart3 test code send \n");
	printf("z) quit\n");
	return 0;
} 
//
int sendUsart(int numN)
{
	int rv;
	int i;
	char tt='1';
	char send1Data[SEND_BUF_SIZE]	= {0xCA, 0x00, 0x05, 0x20, 0x00, 0x0a, 0x00, 0x02, 
			0x00, 0x00, 0x00, 0x1e, 0x14, 0xa8, 0x04, 0x80, 
			0xf0, 0xfb, 0x0a, 0xdf, 0x6d, 0xb6, 0xd8, 0x00,0x00};
	char send3Data[SEND_BUF_SHORT_SIZE]={0x46,0x72,0x6F,0x6D,0x3A,0x74,0x74,0x79,0x53,0x32};
	char send5Data[SEND_BUF_SHORT_SIZE]={0x46,0x72,0x6F,0x6D,0x3A,0x74,0x74,0x79,0x53,0x34};
	char send6Data[SEND_BUF_SHORT_SIZE]={0x46,0x72,0x6F,0x6D,0x3A,0x74,0x74,0x79,0x53,0x35};

	if(numN==1)
	{
		for(i=0;i<SEND_BUF_SIZE;++i)	
		{
			rv =write(fd1,&send1Data[i],1);
		}
		if(rv<0)
		{
			goto ERROR;
		}
		
	}
	else 
	{
		
		for(i=0;i<SEND_BUF_SHORT_SIZE;++i)	
		{
			switch(numN)
			{
				case 3:
					rv =write(fd3,&send3Data[i],1);
					break;
				case 5:
					rv =write(fd5,&send5Data[i],1);
					break;
			        case 6:	
					rv =write(fd6,&send6Data[i],1);
					break;
				default:
					return -1;
			}
	
		}
		if(rv<0)
		{
			goto ERROR;
		}
	}
	if(rv<0)
	{
		printf("write error\n");
		return -3;
	}
ERROR:
	return rv;	
}
//-----------------------------
int do_LED(int ledN,int val)//ledN= 1, or 2  val=0 or 1
{
	char ledNumber, ledValue;
	int fd,rv;
	int retV=-1;
	char data[2];
	if(ledN==1)
	{
		stm32f2_gpout_set(&led1,val);
	}
	else if(ledN==2)
	{
		stm32f2_gpout_set(&led2,val);
	}
	else{
		goto error;
	}
	return rv;
error:
	retV =-1;
	close(fd);
	return retV;
}
int show_DI()
{
	int i;
	int val;
	int retV;
	for(i=0;i<STATUS_GPIO_NUMBER;++i)
	{
		val =stm32f2_gpio_getValue(&status_ind[i],STM32F2_GPIO_ROLE_GPIN);
		if(val)
		{
			printf("1");
		}
		else if(val==0)
		{
			printf("0");
		}
		else
		{
			printf("X");
		}
		
	}
	if(kbhit())
	{
		return 0;	
	}
	printf("\r");
	
	return 1;
}
//-----------------------------------
void init_keyboard()
{
	tcgetattr(0,&initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_lflag &= ~ISIG;
	new_settings.c_cc[VMIN] =1;
	new_settings.c_cc[VTIME] =0;
	tcsetattr(0,TCSANOW, & new_settings);
}
void close_keyboard()
{
	tcsetattr(0,TCSANOW, &initial_settings);
}
int kbhit()
{
	char ch;
	int nread;
	if(peek_character !=-1)
	{
		return 1;
	}
	new_settings.c_cc[VMIN]=0;
	tcsetattr(0,TCSANOW, &new_settings);
	nread = read(0,&ch,1);
	new_settings.c_cc[VMIN]=1;
	tcsetattr(0,TCSANOW,& new_settings);
	if(nread ==1){
		peek_character =ch;
		return 1;
	}
	return 0;
}
int readch()
{
	char ch;
	if(peek_character !=-1)	{
		ch =peek_character;
		peek_character =-1;
		return ch;
	}
	read(0,&ch,1);
	return ch;
}
//-------------------------------------------------------------
