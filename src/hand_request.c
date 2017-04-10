#include "fun.h"

void hand_request(int fd){
	//子进程收到new_fd后开始工作
	//命令识别部分
	//char buf[200];
	char buf[10000];

	int ret;
	//recv
	ret = pre_hand();
	if(-1 == ret){
		printf("pre_hand error\n");
	}
	int len;
	char path[1000];
	while(1){
		len =0;
		bzero(path,sizeof(path));
		bzero(buf,sizeof(buf));
		ret = recv(fd,&len,sizeof(int),0);
		if(ret == -1 )
		{
			perror("recv");
			return;
		}
		ret = recv(fd,buf,len,0);
		if(ret == -1)
		{
			perror("recv1");
			return;
		}
		
		//设计字符串的操作
		
		if(strncmp(buf,"cd",2) == 0)
		{
			len = strlen(buf)-4;
			strncpy(path,buf+3,len);
			order_cd(fd,path);
			printf("cd\n");
			continue;
		}
		if(strncmp(buf,"ls",2) == 0 )
		{
			get_ls(fd,getcwd(NULL,0));
			printf("ls\n");
		}
		if(0 == strncmp(buf,"puts",4))
		{
			printf("puts\n");
		}
		if(0 == strncmp(buf,"gets",4))
		{
			len = strlen(buf)-6;
			strncpy(path,buf+5,len);
			printf("%s\n",path);
			order_gets(fd,path);
			printf("gets\n");
		}
		if(0 == strncmp(buf,"remove",6))
		{		
			len = strlen(buf)-8;
			strncpy(path,buf+7,len);
			order_remove(fd,path);
			printf("remove\n");
		}
		if(0 == strncmp(buf,"pwd",3))
		{
			get_pwd(fd);
		}
		if(0 == strncmp(buf,"exit",4))
		{
			printf("cli exit\n");
			return;
		}
	}

}
