/*使用客户端和服务器的模型，模拟聊天通信：
 *（1）客户端给服务器端发送信息，服务器端把它打印到屏幕
 *（2）服务器端给客户端发送信息，客户端把信打印到屏幕*/
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int cliFd;
void* handler(void *arg)
{
	while(1)
	{
		int real_read;
		int real_write;
		char buf[1024];
		memset(buf,0,1024);
		real_read=read(0,buf,1024);
		if(real_read<0)
		{
			printf("handler read error\n");
			return NULL;
		}
		real_write=write(cliFd,buf,real_read-1);
		if(real_write>0)
		{
			printf("server send message to client:%s\n",buf);
			
		}
	}
}
int main(int argc,char **argv)
{
	int fd;
	int ret;
	struct sockaddr_in serAddr,cliAddr;
	socklen_t cliaddr_size=sizeof(cliAddr);
/*	if(argc!=3)
	{
		printf("para error\n");
		return -1;
	}*/
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd<0)
	{
		printf("socket error\n");
		return 0;
	}
	memset(&serAddr,0,sizeof(serAddr));
	serAddr.sin_family=AF_INET;
	serAddr.sin_port=htons(atoi(argv[1]));//atoi字符串转换为整型,htons:主机序转为网络序
	//htonl(INADDR_ANY)
	serAddr.sin_addr.s_addr=inet_addr("127.0.0.1");//inet_addr字符串转换为整型
	
	ret=bind(fd,(struct sockaddr *)&serAddr,sizeof(serAddr));
	if(ret<0)
	{
		printf("bind error\n");
		close(fd);
		return -1;
	}
	ret=listen(fd,800);
	if(ret<0)
	{
		printf("listen error\n");
		close(fd);
		return -1;
	}
	memset(&cliAddr,0,sizeof(cliAddr));
	cliFd=accept(fd,(struct sockaddr *)&cliAddr,&cliaddr_size);
	if(cliFd<0)
	{
		printf("accept error\n");
		close(fd);
		return -1;
	}
	pthread_t pth_id;
	ret=pthread_create(&pth_id,NULL,handler,NULL);
	if(ret<0)
	{
		printf("pthread_create error\n");
		close(cliFd);
		close(fd);
		return -1;
	}
	char buf[1024];
	while(1)
	{
		memset(buf,0,1024);
		ret=read(cliFd,buf,1024);
		if(ret>0)
		{
			printf("server recv msg from client:%s\n",buf);
		}
	}
	pthread_join(pth_id,NULL);
	close(fd);
	close(cliFd);
	return 0;
}
