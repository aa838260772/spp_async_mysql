#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>

#include <unistd.h>
#include <fcntl.h>


#define ERR_EXIT(m) \
	do { \
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

#define MAXLINE 20 
#define SENDSIZE 20

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
static void do_client(int fd)
{
	char recvbuf[SENDSIZE + 1] = {0};
	char sendbuf[MAXLINE  + 1] = {0};

	char sendserverbuf[SENDSIZE+1] = {0};
	memset(sendserverbuf, 49, SENDSIZE*sizeof(char));

	printf("%s\n", sendbuf);
	fd_set reade, ready;
	FD_ZERO(&reade);
	int fd_stdin = fileno(stdin);
	FD_SET(fd_stdin, &reade);
	FD_SET(fd, &reade);
	int fd_max = (fd_stdin > fd) ? fd_stdin : fd;

	int ret;
	while(1)
	{
		ready = reade;
		ret = select( fd_max+1, &ready, NULL, NULL, NULL);//轮询
		if(ret == -1)
		{
			if(errno == EINTR)
				continue;
			ERR_EXIT("select");
		}else if(ret ==  0)
		{
			continue;
		}

		if(FD_ISSET(fd_stdin, &ready))
		{
			if(fgets(sendbuf, sizeof(sendbuf), stdin) == NULL)
			{
				close(fd);
				break;
			}else
			{
				int icnt = 0;
				while(icnt < SENDSIZE)
				{
					int wret = write(fd, sendserverbuf, strlen(sendserverbuf));
					if(wret > 0)
					{
						getbufflen(fd);
						icnt += wret;
						printf("send %d errno:%d size:%d %d\n ", wret,errno, icnt, SENDSIZE);
					}					
					if(errno = EAGAIN)
						continue;
				}
			}
		}


		if(FD_ISSET(fd, &ready))
		{
			int nread = read(fd, recvbuf, sizeof(recvbuf));
			if(nread < 0)
				ERR_EXIT("read");
			if(nread == 0)//如果没接收到消息，打印关闭描述符，退出循环
			{
				fprintf(stdout, "fd close\n");
				break;
			}
			getbufflen(fd);
			fprintf(stdout, "receive:%d %s\n", nread, recvbuf);
		}
		memset(sendbuf, 0, sizeof sendbuf);
		memset(recvbuf, 0, sizeof recvbuf);
	}
}
void handle(int signum)
{
	printf("sigpipe\n");
}

int main(int argc, const char *argv[])
{
	signal(SIGPIPE, SIG_IGN);
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
		ERR_EXIT("socket");

	struct sockaddr_in cliaddr;
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(8888);
	cliaddr.sin_addr.s_addr = inet_addr("192.168.202.54");
	socklen_t len = sizeof cliaddr;

	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger=1;
	//设置延迟关闭
	setsockopt(fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
	// 接收缓冲区
	//int nRecvBuf=4096*1024;//设置为32K
	//setsockopt(fd,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
	//发送缓冲区
	int nSendBuf=4096*3;//设置为32K
	setsockopt(fd,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
	
	getbufflen(fd);

	int ret ;
	if((ret = connect(fd, (struct sockaddr*)&cliaddr, len)) == -1)
	{
		close(fd);
		ERR_EXIT("connect");
	}
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
	do_client(fd);
	close(fd);
	return 0;
}
