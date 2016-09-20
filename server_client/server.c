#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(int argc,char **argv)
{
	int fd;
	int ret;
	struct sockaddr_in serAddr,cliAddr;
	socklen_t cliaddr_size=sizeof(cliAddr);//
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
		return -1;
	}
	ret=listen(fd,800);
	if(ret<0)
	{
		printf("listen error\n");
		return -1;
	}
	int cliFd;
	memset(&cliAddr,0,sizeof(cliAddr));
	cliFd=accept(fd,(struct sockaddr *)&cliAddr,&cliaddr_size);
	if(cliFd<0)
	{
		printf("accept error\n");
	}
	char buf[32];
	strcpy(buf,"hello client");
	ret=write(cliFd,buf,strlen(buf));
	if(ret>0)
	{
		printf("server send msg to client:%s\n",buf);
	}
	
	return 0;
}
