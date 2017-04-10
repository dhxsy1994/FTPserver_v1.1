#include "fun.h"

int order_puts(int sfd,char* filename)
{
	//接受控制信息
	int flag;
	short ctl ;
	int ret;
	int fd;
	char buf[10000];
	int len;
	ret = recv(sfd,&ctl,sizeof(short),0);
	//控制信息判断
	if(ctl == -1)
	{
		printf("cli no file\n");
		return -1;
	}
	if(ctl == 0)
	{
		fd = open(filename,O_RDWR|O_CREAT);
		if(-1 == fd)
		{
			perror("open");
			return -1;
		}
		while(1)
		{
			ret = recv(sfd,&len,sizeof(int),0);
			if (len >0)
			{
				bzero(buf,sizeof(buf));
				recv_n(sfd,buf,len);
				write(fd,buf,len);
			}else if(len == 0)
			{
				printf("puts file suc\n");
				break;
			}
		}
		close(fd);
		return 0;

	}



}
