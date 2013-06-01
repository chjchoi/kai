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
static struct termios initial_settings,new_settings;
static int peek_character =-1;
void init_keyboard();
void close_keyboard();
int doJob(char);
int kbhit();
int readch();
//
int do_LED(int ledN,int val);
int show_DI(void);
int sendUsart(int);
static int slb=1;
//
#define INPUT_SIZE 20
#define SEND_BUF_SIZE 32
int fd1,fd3;
static char ch=0;
static	int rv=0;
int main(void)
{
	char str[INPUT_SIZE];
	pid_t pid=100;
	pid_t pid2=100;
	init_keyboard();
	struct termios term;
        nor_Show_Diag_Menu(slb);	

	memset(&term,0,sizeof(term));
	term.c_iflag=0;
	term.c_oflag=0;
	term.c_cflag=CS8|CREAD|CLOCAL;
	term.c_lflag=0;
	term.c_cc[VMIN]=1;
	term.c_cc[VTIME]=5;

	fd1 =open("/dev/ttyS0",O_WRONLY|O_NONBLOCK);
        fd3 =open("/dev/ttyS2",O_WRONLY|O_NONBLOCK);

	cfsetospeed(&term,B115200);
	cfsetispeed(&term,B115200);
	tcsetattr(fd1,TCSANOW,&term);
	tcsetattr(fd3,TCSANOW,&term);
	pid=vfork();
	switch(pid)
	{
		case -1: //fork fail
			close(fd1);close(fd3);
			return;
		case 0://child
			execl("/mnt/tty2","tty2","1",0);
			break;
		default:
			pid2=vfork();
			if(pid2==0)
			{
				execl("/mnt/tty2","tty2","3",0);
			}
			else
			{
			}
			break;
	}
	while(1)
	{
	if(rv==0)
	{
		scanf("%c",&ch);
		if(ch=='z')
		{
			printf("quit!!\n");
			return 0;
		}
	}
	rv=doJob(ch);
	ch=0x00;
	}//while
END:
	close_keyboard();
	close(fd1);close(fd3);
	return 0;
}//main

int doJob(char ch)
{
		int rv=0;	
	   	switch(ch)// rv=0 .. 1time. else ..
	    	{
			case 0:
				rv=0; ch=0; return 0;
			case 'a':   rv=0;do_LED(1,0);break;//led1 on
			case 'b':   rv=0;do_LED(1,1);break;//led1 off
			case 'c':   rv=0;do_LED(2,0);break;//led2 on
			case 'd':   rv=0;do_LED(2,1);break;//led2 off
			case 'e':   rv=show_DI();break;
			case 'f' :
			break;
			case 'g' :
			break;
			case 'h':   rv=0;nor_Show_Diag_Menu();break;
			case 'i':
			break;
			case 'j':
			break;
			case 'k':
			break;
			case 'm':  rv=0;slb=1;
        			nor_Show_Diag_Menu(slb);break;
			case 'n':  rv=0;slb=0;
        			nor_Show_Diag_Menu(slb);break;
			case 'q':  rv=0xFF;goto END;
	      	        case '1': rv=0;sendUsart(1);break; 
			case '2':
			break;
			case '3':rv=0;sendUsart(3);break; 
			case '4':
			break;
			case '5':
			break;
			case '6':
			break;
			case 'z':
				return 0xFF;
			default:
			break;
		}
END:
		return rv;
error:
		return -1;
}
//
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
	if(numN==1)
	{
		for(i=0;i<25;++i)	
			rv =write(fd1,&send1Data[i],1);
		//rv =fprintf(fd1,"%s\n",send1Data);
		if(rv<0)
		{
			goto ERROR;
		}
		else
		{
			return 0;
		}	
		
	}
	else if(numN==3)
	{
		
		for(i=0;i<25;++i)	
			rv =write(fd3,&send1Data[i],1);
	//	rv =fprintf(fd3,"%s\n",send1Data);
		if(rv<0)
		{
			goto ERROR;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		printf("wrong port");
		return 0;
	}	
	if(rv<0)
	{
		printf("write error\n");
	//	close(fd1);
	//	close(fd3);
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
		data[0]=0x30;
		if(val==0)
		{
			data[1]=0x30;// 
		}
		else if(val==1)
		{
			data[1]=0x31;// 
		}
		else
		{
			goto error;
		}
	}
	else if(ledN==2)
	{
		data[0]=0x31;
		if(val==0)
		{
			data[1]=0x30;//
		}
		else if(val==1)
		{
			data[1]=0x31;//
		}
		else
		{
			goto error;
		}
	}
	else{
		goto error;
	}
		
		
	if((fd =open("/dev/nor_led",O_WRONLY))<0)
	{
		printf("nor led open error");
		goto error;
	}
	if((rv=write(fd,&data,2))<0)
	{
		printf(" nor led write error");
		goto error;
	}	
	printf("led ok...\r");
	close(fd);
	return rv;
error:
	retV =-1;
	close(fd);
	return retV;
}
int show_DI()
{
	int fd;
	int x;
	int i;
	char c;
	int ch;
	int retV;
	if((fd =open("/dev/status_gpio",O_RDONLY))<0)
	{
		printf("status_gpio open error");
		retV=-1;
		goto Done;
	}
	while(1)
	{
		if(kbhit())
		{
			ch =readch();
			retV =ch;
			goto Done;		
		}
		for(i=0;i<9;++i)
		{
			c=(char)(i+0x30);
			if((x=read(fd,&c,1))<0)
			{
				printf("status_gpio read error:#%d..%d",i,x);
				goto Done;
			}
			
			if(c==0x31)//'1'
			{
				printf("1");
			}
			else if(c==0x30)//'0'
			{
				printf("0");
			}
			else
			{
				printf("X");
			}
		}
		printf("\r");
	}
Done:
	close(fd);
	return retV;
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
