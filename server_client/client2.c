//客户端
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
int fd;
void* handler(void* arg)
{
	int real_write;
	int real_read;
	char buf[1024];
	while(1)
	{
		memset(buf,0,1024);
		real_read=read(0,buf,1024);
		if(real_read<0)
		{
			printf("handler read error\n");
			return NULL;
		}
		real_write=write(fd,buf,real_read-1);
		if(real_write>0)
		{
			printf("client send message to server:%s\n",buf);
		}
	}
return  NULL;
}

int main(int argc,char **argv)
{
	int ret;
	struct sockaddr_in clientAddr;
/*	if(argc!=3)
	{
		printf("para error\n");
		return -1;
	}*/
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd<0)
	{
		printf("socket error\n");
		return -1;
	}
	clientAddr.sin_family=AF_INET;
	clientAddr.sin_port=htons(atoi(argv[2]));//atoi字符串转换为整型
	clientAddr.sin_addr.s_addr=inet_addr(argv[1]);//inet_addr字符串转换为整型
	ret=connect(fd,(struct sockaddr *)&clientAddr,sizeof(clientAddr));
	if(ret<0)
	{
		printf("connect error\n");
		return -1;
	}
	pthread_t pth_id;
	ret=pthread_create(&pth_id,NULL,handler,NULL);
	if(ret<0)
	{
		printf("pthread_create error\n");
		close(fd);
		return -1;
	}
/*
	char buf[1024];
	while (1)
	{
		memset(buf,0,1024);
		ret=read(fd,buf,1024);
		if(ret>0)
		{
			printf("client recv message from server:%s\n",buf);	
		}
	}*/
	pthread_join(pth_id,NULL);
	close(fd);
	return 0;
}
