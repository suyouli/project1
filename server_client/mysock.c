#include "mysock.h"
#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000
//mmmmmmmmmmmmmmmmmmmmmm
void setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

int creatServer(int port)
{
    int i, maxi, listenfd, connfd, sockfd,epfd,nfds, portnumber;
    ssize_t n;
    char line[MAXLINE];
    socklen_t clilen;
//#define socklen_t int


    struct epoll_event ev,events[20];
    
	struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    setnonblocking(listenfd);

    epfd = epoll_create(256);
    ev.data.fd=listenfd;
//    ev.events=EPOLLIN|EPOLLLT;
    ev.events=EPOLLIN|EPOLLET;
    //ev.events=EPOLLIN;

    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
   // bzero(&serveraddr, sizeof(serveraddr));
	memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char *local_addr="127.0.0.1";
    inet_aton(local_addr,&(serveraddr.sin_addr));//htons(portnumber);

    serveraddr.sin_port=htons(port);
    bind(listenfd,(struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);
    maxi = 0;
    for ( ; ; ) {
        
		nfds=epoll_wait(epfd,events,20,500);
        //�����������������¼�
//       printf("epoll_wait\n");
        for(i=0;i<nfds;++i)
        {
			printf("@@@@@@@\n");
            if(events[i].data.fd==listenfd)//����¼�⵽һ��SOCKET�û����ӵ��˰󶨵�SOCKET�˿ڣ������µ����ӡ�

            {
                connfd = accept(listenfd,(struct sockaddr *)&clientaddr, &clilen);
                if(connfd<0){
                    perror("connfd<0");
                    exit(1);
                }
                setnonblocking(connfd);

                char *str = inet_ntoa(clientaddr.sin_addr);
				printf("accept a connection from %s\n", str);

                ev.data.fd=connfd;

                ev.events=EPOLLIN|EPOLLET;
                //ev.events=EPOLLIN;

                //ע��ev

                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
            }
            else if(events[i].events&EPOLLIN)//������Ѿ����ӵ��û��������յ����ݣ���ô���ж��롣

            {
				printf("EPOLLIN\n");
                //cout << "EPOLLIN" << endl;
                if ( (sockfd = events[i].data.fd) < 0)
                    continue;
                if ( (n = read(sockfd, line, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        events[i].data.fd = -1;
                    } else
						printf("readline error\n");
                        //std::cout<<"readline error"<<std::endl;
                } else if (n == 0) {
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                line[n] = '\0';
				printf("read:%s\n",line);

                ev.data.fd=sockfd;

               // ev.events=EPOLLOUT|EPOLLET;
                //�޸�sockfd��Ҫ������¼�ΪEPOLLOUT

                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);

            }
            else if(events[i].events&EPOLLOUT) // ��������ݷ���

            {
                sockfd = events[i].data.fd;
                write(sockfd, line, n);

                ev.data.fd=sockfd;

                ev.events=EPOLLIN|EPOLLET;

                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
            }
        }
    }
    return 0;
}
