#include "fun.h"

int main(int argc, char *argv[])
{
	if(argc != 4){
		printf("error args\n");
		return -1;
	}

	//客户端连接
	int num = atoi(argv[3]);//启动连接数
	pcdata p = (pcdata)calloc(num,sizeof(cdata));
	make_child(p,num);//进程池初始化
	int sfd;//创建套接字描述符
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if (-1 == sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;//套接字连接地址
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));//将端口号转换为网络字节序
	ser.sin_addr.s_addr=inet_addr(argv[1]);//将点分十进制IP地址转换为网络字节序
	int ret;
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));//端口绑定
	if(ret==-1)
	{

		perror("bind");
		return -1;

	}
	//epoll多路复用
	int epfd;
	epfd = epoll_create(1);
	struct epoll_event event,*evs;
	evs=(struct epoll_event*)calloc(num+1,sizeof(struct epoll_event));
	//将sfd添加至监控
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;
	event.data.fd=sfd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	if(-1 == ret){
		perror("epoll_ctl");
		return -1;
	}
	//将子进程的事件加入epoll
	int i;
	for(i=0;i<num;i++)
	{
		event.events = EPOLLIN;
		event.data.fd = p[i].fdw;
		epoll_ctl(epfd,EPOLL_CTL_ADD,p[i].fdw,&event);
	}
	//listen
	ret = listen(sfd,num);
	if(-1 == ret)
	{
		perror("listen");
		return -1;
	}
	struct sockaddr_in cli;
	int addrlen;
	//循环内部执行体
	int new_fd;//用于通信的描述符
	int j;
	short flag;
	while(1)
	{
		bzero(evs,(num+1)*sizeof(struct epoll_event));
		ret = epoll_wait(epfd,evs,num+1,0);//清空evs事件数组并等待响应
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
				if(sfd==evs[i].data.fd)
				//若是一个网络描述符accept
				{
					new_fd=accept(sfd,(struct sockaddr*)&cli,&addrlen);
					printf("accept client success\n");
					for(j=0;j<num;j++)
					{
						if(p[j].busy==0)
						{
							printf("process %d will busy\n",p[j].pid);
							send_fd(p[j].fdw,new_fd);//把new_fd传递给子进程
							p[j].busy=1;//这里置忙
							break;	

						}

					}
					close(new_fd);//关闭以后，因为引用计数为1，实际没有断开
				}
				for(j=0;j<num;j++)
				{
					if(p[j].fdw==evs[i].data.fd)
					{
						read(p[j].fdw,&flag,sizeof(flag));
						printf("process %d is not busy\n",p[j].pid);
						p[j].busy=0;//子进程变为非忙碌	

					}
				}	
			}	
		}
	}
}
