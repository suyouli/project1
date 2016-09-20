/*使用客户端和服务器的模型，编一个程序要求如下：
 *（1）客户端给服务器端发送信息
 *（2）服务器有两个线程一个线程专门接收数据并保存到链表中，另一个线程用来打印链表的数据，然后删除打印过信息的节点，把它从链表中删除*/
//server
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
typedef struct server_list
{
	char data[1024];
    struct server_list* next;
}serverlist,*slist;
int cliFd;
slist h;
//创建链表
slist server_Createlist(void)
{
	slist p=(slist)malloc(sizeof(serverlist));
	if(p==NULL)
		return NULL;
	p->next=NULL;
	return p;

}
//接收数据并保存到链表中，
void* handler1(void *arg)
{
	int ret;
	char buf[1024];
	while(1)
	{
		memset(buf,0,1024);
		ret=read(cliFd,buf,1024);
		if(ret>0)
		{
			printf("server recv msg from client:%s\n",buf);
		}
		slist p=(slist)malloc(sizeof(serverlist));
		slist q=NULL;
		if(p==NULL)
			return NULL;
		strcpy(p->data,buf);
		for(q=h;q->next!=NULL;q=q->next)
			;
		p->next=q->next;
		q->next=p;
//		printf("%s\n",h->next->data);
		slist h1=NULL;
/*		for(h1=h->next;h1!=NULL;h1=h1->next)
				printf("%s\n",h1->data);*/

	}
}
//用来打印链表的数据，然后删除打印过信息的节点，把它从链表中删除
void* handler2(void *arg)
{
	while(1)
	{
		slist q=NULL;
		q=h->next;
		for(;q!=NULL;q=q->next)
		{
			printf("%s\n",q->data);
			h->next=q->next;
			free(q);
		}
	}
}

int main(int argc,char **argv)
{
    h=server_Createlist();
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
	pthread_t pth_id1;
	ret=pthread_create(&pth_id1,NULL,handler1,NULL);
	if(ret<0)
	{
		printf("pthread_create error1\n");
		close(cliFd);
		close(fd);
		return -1;
	}
	pthread_t pth_id2;
	ret=pthread_create(&pth_id2,NULL,handler2,NULL);
	if(ret<0)
	{
		printf("pthread_create error1\n");
		close(cliFd);
		close(fd);
		return -1;
	}
	pthread_join(pth_id1,NULL);
	pthread_join(pth_id2,NULL);
	close(fd);
	close(cliFd);
	return 0;
}
