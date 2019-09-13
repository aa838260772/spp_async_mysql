#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/epoll.h>
#define MAXLEN 1024*300

#include <iostream>
#include <map>

using namespace std;
#define ERR_EXIT(m) \
	do { \
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

int listenf();
static void do_epoll(int);

void getbufflen(int fd)
{

	// 接收缓冲区
	int nRecvBuf=0;//设置为32K
	socklen_t r_len;
	getsockopt(fd,SOL_SOCKET,SO_RCVBUF,(void*)&nRecvBuf,&r_len);
	//发送缓冲区
	int nSendBuf=0;//设置为32K
	socklen_t s_len;
	getsockopt(fd,SOL_SOCKET,SO_SNDBUF,(void*)&nSendBuf,&s_len);

	printf("recv:%d send:%d\n", nRecvBuf, nSendBuf);
}



int main(int argc, const char *argv[])
{
	int listenfd = listenf();
	do_epoll(listenfd);
	close(listenfd);
	return 0;
}

int listenf()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);//准备一个socketfd
	if(listenfd == -1 )
		ERR_EXIT("listen");

	int on = 1;
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)//setsockopt设置端口复用
	{
		close(listenfd);
		ERR_EXIT("setsockopt");
	}

	int nRecvBuf=4096*2;//设置为32K
	setsockopt(listenfd,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));


	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(8888);
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//seraddr.sin_addr.s_addr = inet_addr("192.168.202.54");
	socklen_t len = sizeof(seraddr);
	if(bind(listenfd, (struct sockaddr*)&seraddr, len) == -1)//监听socket端口，
	{
		close(listenfd);
		ERR_EXIT("bind");
	}

	if(listen(listenfd, 6) == -1)
	{
		close(listenfd);
		ERR_EXIT("listen");
	}
	return listenfd;
}

void do_epoll(int fd)
{
	char recvbuf[MAXLEN] = {0};
	int epollfd = epoll_create(2048);//设置的最大连接数
	if(epollfd == -1)
		ERR_EXIT("epoll_create");

	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)//加入轮询
		ERR_EXIT("epoll_ctl_add");

	struct epoll_event events[2048];//数组在epoll_wait返回结果的时候使用
	int ret;
	int i;//在下面while的for循环中遍历使用
	int rfd;
	int clientfd;
	int nread;
	std::map<int, int> map_req;
	while(1)
	{
		ret = epoll_wait(epollfd, events, 2048, 500);
		if(ret == -1)
			ERR_EXIT("epoll_wait");

		for(i = 0; i < ret; ++i )
		{
			rfd = events[i].data.fd;
			if(rfd == fd)
			{
				if((clientfd = accept(fd, NULL, NULL)) == -1)
					ERR_EXIT("accept");
				printf("client %d connect\n", clientfd);
				ev.data.fd = clientfd;
				ev.events = EPOLLIN;
				if(epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev) == -1)
					ERR_EXIT("epoll_ctl");
			}else
			{
				int nread = read(rfd, recvbuf, MAXLEN);
				if(nread == -1)
				{
					if(errno == EINTR || errno == EAGAIN)
						continue;
					else
					{
						close(rfd);
						printf("%d fd unnormal close %d\n", rfd, errno);
					}
					//ERR_EXIT("read");
				}else if( nread == 0)//客户端退出，从epoll轮询中删除
				{
					printf("%d fd normal close\n", rfd);
					ev.data.fd = rfd;
					ev.events = EPOLLIN;
					if(epoll_ctl(epollfd, EPOLL_CTL_DEL, rfd, &ev) == -1)
						ERR_EXIT("epoll_ctl");
					close(rfd);
				}else
				{
					getbufflen(rfd);
					printf("fd:%lu client %lu req:%d %s\n", rfd, time(NULL),nread, recvbuf);
					map_req[rfd] = time(NULL);
					//sleep(5);
					//int wret = write(rfd, recvbuf, strlen(recvbuf)); 
					//printf("client %lu rsp:%d\n", time(NULL),wret);
					//sleep(1);
					memset(recvbuf, 0, MAXLEN);
				}
			}
		}

		int iTime = time(NULL);
		for(map<int, int>::iterator itr = map_req.begin();
				itr != map_req.end();
		   )
		{
			if(iTime - itr->second >=5)
			{
				write(itr->first, "server response", 15);
				printf("fd:%d client %lu rsp:%s\n", itr->first, time(NULL),"server response");
				map_req.erase(itr++);
			}
			else
			{
				itr++;
			}
		}
	}
	close(epollfd);
}
