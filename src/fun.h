#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <dirent.h>
#include <sys/epoll.h>
#define FILENAME "file"
typedef struct{
	pid_t pid;
	int fdw;
	short busy;
	
}cdata,*pcdata;

typedef struct{
	int len;
	char buf[10000];
}train,*ptrain;

void child_handle(int fd);
void send_fd(int fdw,int fd);
void make_child(pcdata p ,int n);
void recv_fd(int fdr,int* fd);
void hand_request(int new_fd);
int send_n(int sfd,void *buf,int len);
int recv_n(int sfd,void *buf,int len);
int get_pwd(int sfd);
int pre_hand();
int get_ls(int sfd,char* path);
int order_cd(int sfd,char* path);
int order_remove(int sfd,char* filename);
int send_file(int sfd,char* filename);
int order_gets(int sfd,char* filename);
int order_puts(int sfd,char* filename);
