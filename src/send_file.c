#include "fun.h"

int send_file(int sfd,char* filename)
{
	train t;//定义小火车
	//第一步传输文件名
	bzero(&t,sizeof(t));
	t.len=strlen(filename);
	strcpy(t.buf,filename);
	send(sfd,&t,4+t.len,0);
	//发送文件内容
	int fd;
	int flag;
	fd=open(filename,O_RDWR);
	if(-1==fd)
	{
		perror("open");
		close(sfd);
		return -1;
	}
	int ret;
	//循环读取文件内容，并发送
	while((t.len=read(fd,t.buf,sizeof(t.buf)))>0)
	{

		ret=send_n(sfd,&t,4+t.len);
		if(-1==ret)
		{
			goto end;
		}
	}
end:
	flag=0;
	send(sfd,&flag,sizeof(int),0);//结束标记
	close(fd);
}
