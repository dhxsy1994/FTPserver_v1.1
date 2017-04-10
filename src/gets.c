#include "fun.h"

int order_gets(int sfd,char* filename)
{
	//接收控制命令
	int ret;
	short ctl;
	ret = recv(sfd,&ctl,sizeof(short),0);
	if(-1 == ret)
	{
		perror("recv");
		return -1;
	}


	if (ctl == 0)
	{
	//接收到文件名验证服务器是否存在该文件
	train t;
	DIR *dir;
	dir = opendir(getcwd(NULL,0));
	if(NULL == dir)
	{
		perror("opendir");
		return -1;
	}
	struct dirent *p;
	int ret;
	bzero(&t,sizeof(t));
	while((p = readdir(dir))!=NULL)
	{
		ret = strcmp(filename,p->d_name);
		if(ret == 0)
		{
			//服务器文件存在
			ctl = 3;
			send(sfd,&ctl,sizeof(short),0);
			//开始发送
			printf("sending..\n");
			ret = send_file(sfd,filename);
			if(-1 == ret)
			{
				printf("send_file fail\n");
				return -1;
			}else{
				printf("send_file success\n");
				return 0;
			}
		}
	}
	//服务器文件不存在
	printf("no file 2\n");
	ctl = 2;
	send(sfd,&ctl,sizeof(short),0);
	return 0;
	}//ctl = 0

	//客户端文件存在
	if (ctl == 4)
	{
		printf("cli file exist 4\n");
		return 0;
	}

}
