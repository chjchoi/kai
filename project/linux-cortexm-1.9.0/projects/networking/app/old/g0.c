#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>

#include<sys/types.h>

#define MODEMDEVICE 	"/dev/ttyS0"
#define BUFFER_NUM 20
char x,y;
int main(){
        int  r_cnt;
	int count;
	int  MODEMFD;
	char chrs[BUFFER_NUM];
	int i;
	char ccc;
	for(i=0;i<BUFFER_NUM;++i)
	{
		chrs[i]=0x00;
	}
	MODEMFD = open(MODEMDEVICE, O_RDONLY | O_NOCTTY);
	
	if((MODEMFD < 0)){
		printf("%s:open error\n",MODEMDEVICE);
		return -1;
	}

	printf("%s:open success!\n", MODEMDEVICE);
	
	r_cnt = 0;
        
	while(1){
		//r_cnt = read(MODEMFD, chrs, 1);
		r_cnt = read(MODEMFD, &ccc, 1);
		/*
	 	count=0;	
		while(count<r_cnt ){
			printf("%c",MODEMDEVICE,chrs[count]);
			count++;
			if(count==r_cnt)
			{
				for(i=0;i<BUFFER_NUM;++i)
				{
					chrs[i]=0x00;
				}
			}
		}
		*/
		if(r_cnt==1)
		{
//			x=ccc/10;
//			y=ccc%10;
//			printf("%c",x+'0');
//			printf("%c",y+'0');
//			printf(" ");
		printf("ttyS0:%02x\n",ccc);
//		printf("%c ",ccc);
		}
		ccc=0;
	}
	
	return 0;
}


