#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<termios.h>

#define BUFFER_NUM 25
char x,y;
int main(int argc,char** argv){
        int  r_cnt;
	int count;
	int  MODEMFD;
	char chrs[BUFFER_NUM];
	int i;
	char ccc;
	struct termios term;
	memset(&term,0,sizeof(term));

        term.c_iflag=0;
        term.c_oflag=0;
        term.c_cflag=CS8|CREAD|CLOCAL;  
	// 8n1, see termios.h for more information
        term.c_lflag=0;
        term.c_cc[VMIN]=1;
        term.c_cc[VTIME]=5;

	for(i=0;i<BUFFER_NUM;++i)
	{
		chrs[i]=0x00;
	}
	if(argv[1][0]=='1')
	{
		MODEMFD = open("/dev/ttyS0",O_RDONLY);
		cfsetispeed(&term,B115200);
		tcsetattr(MODEMFD,TCSANOW,&term);
	}
	else if(argv[1][0]=='3')
	{
		MODEMFD = open("/dev/ttyS2",O_RDONLY);
		cfsetispeed(&term,B115200);
		tcsetattr(MODEMFD,TCSANOW,&term);
	}	
	else
	{
		printf("wrong number");
		return -100;
	}	
	if((MODEMFD < 0)){
		if(argv[1][0]=='1')
		{	
			printf("%s:open error\n","ttyS0");
		}
		else
		{
			printf("%s:open error\n","ttyS2");
		}
		return -1;
	}
	if(argv[1][0]=='1')
	{
		printf("%s:open success!\n", "ttyS0");
	}
	else
	{
		printf("%s:open success!\n","ttyS2");
	}
	
	r_cnt = 0;
        
	while(1){
		r_cnt = read(MODEMFD, chrs, 1);
		if(r_cnt>=1)
		{
			if(argv[1][0]=='1')
			{
				printf("ttyS0:%02x\n",chrs[0]);
			}
			else
			{
				printf("tty$2:%02x\n",chrs[0]);
			}
		}
		ccc=0;
	}
	
	return 0;
}


