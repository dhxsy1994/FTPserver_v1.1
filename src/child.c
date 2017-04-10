#include "fun.h"

void make_child(pcdata p ,int num)
{
	//父子进程要传递描述符
	int fds[2];
	int pid;
	for(int i=0; i< num;i++)
	{
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		pid = fork();
		if(0 == pid)
		{
			close(fds[1]);//子进程关闭写
			child_handle(fds[0]);//读写判断
		}
		close(fds[0]);//父进程关闭读
		p[i].pid = pid;
		p[i].fdw = fds[1];
		p[i].busy = 0;//非忙碌
	}
}

void child_handle(int fdr)
{
	short flag = 1;//flag留用写入读出
	int new_fd;
	signal(SIGPIPE,SIG_IGN);
	while(1)
	{
		recv_fd(fdr,&new_fd);//这里如果没有收到fdr会卡住
		hand_request(new_fd);
		write(fdr,&flag,sizeof(flag));
	}
}
