#include "fun.h"

int get_pwd(int sfd)
{
	train t;
	bzero(&t,sizeof(t));
	getcwd(t.buf,sizeof(t.buf));
	t.len = strlen(t.buf);

	int ret = send(sfd,&t,4+t.len,0);
	if(-1 == ret)
	{
		perror("send");
		return -1;
	}
	printf("%s\n",t.buf);
	return 0;
}

int get_ls(int sfd,char* path)
{
	char sbuf[200];
	train t;
	DIR *dir;
	dir = opendir(path);
	if(NULL == dir)
	{
		perror("opendir");
		return -1;
	}
	struct dirent *p;
	int ret;
	struct stat buf;
	char time[35] = {0};
	char buf_size[30] = {0};
	int len;
	bzero(&t,sizeof(t));
	while((p = readdir(dir))!= NULL)
	{	
		bzero(sbuf,sizeof(sbuf));
		bzero(buf_size,sizeof(buf_size));
		ret = stat(p->d_name,&buf);
		if(0 != ret)
		{
			perror("stat");
			return -1;
		}
		//int mode;
		//mode = buf.st_mode;
		if(p->d_type == 4)
		{
			sprintf(sbuf,"d      %-10ld  %-10s  %-10s\n",buf.st_size,p->d_name,ctime(&buf.st_mtime));
		}else{
			sprintf(sbuf,"-      %-10ld  %-10s  %-10s\n",buf.st_size,p->d_name,ctime(&buf.st_mtime));
		}
		len = strlen(sbuf);
		strncat(t.buf,sbuf,len-1);
	}	
	t.len = strlen(t.buf);
	puts(t.buf);
	ret = send_n(sfd,&t,t.len);
	if(ret == -1)
	{
		printf("error send\n");
	}

	closedir(dir);
	return 0;
}


int order_cd(int sfd,char* path)
{
	int len;
	int ret;	
	printf("%s",path);
	ret=chdir(path);
	if(-1 == ret)
	{
		perror("chdir");
		send(sfd,"no dir\n",7,0);
		return -1;
	}else{
		printf("chdir success\n");
		send(sfd,"ch ok\n",5,0);
		return 0;
	}

	return 0;		
}

int order_remove(int sfd,char* filename)
{
	int ret;
	train t;
	bzero(&t,sizeof(t));
	int len=strlen("remove suc");
	int len1=strlen("remove fail");
	ret = remove(filename);
	if(-1 == ret)
	{
		t.len = len1;
		printf("remove fail\n");
		strcpy(t.buf,"remove fail");
		t.len = len1;
			
		ret = send(sfd,&t,4+t.len,0);
		if(-1 == ret)
		{
			perror("send");
			return -1;
		}
	}else{
		t.len = len;
		strcpy(t.buf,"remove suc");
		printf("remove suc\n");
		ret = send(sfd,&t,4+t.len,0);
		if(-1 == ret)
		{
			perror("send1");
			return -1;
		}
	}
	return 0;
}


