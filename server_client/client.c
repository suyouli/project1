#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
int main(int argc,char **argv)
{
	int fd;
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
	clientAddr.sin_family=AF_INET;//地址族
	clientAddr.sin_port=htons(atoi(argv[2]));//端口号，atoi字符串转换为整型
	clientAddr.sin_addr.s_addr=inet_addr(argv[1]);//IPv4地址inet_addr字符串转换为整型
	ret=connect(fd,(struct sockaddr *)&clientAddr,sizeof(clientAddr));
	if(ret<0)
	{
		printf("connect error\n");
		return -1;
	}
	char buf[1024];
	memset(buf,0,1024);
	ret=read(fd,buf,1024);
	if(ret>0)
	{
		printf("client recv message from server:%s\n",buf);
		return -1;
	}
}
