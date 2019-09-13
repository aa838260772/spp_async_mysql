#include "micro_thread.h"
#include "mt_api.h"
#include <vector>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "mt_sys_hook.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;
using namespace NS_MICRO_THREAD;

int chek_func(void *buf, int len)
{
	return strlen((char*)buf);
}

void threadEntry(void * arg)
{
	cout << "EnterEntry " << MtFrame::Instance()->GetActiveThread() << endl;
	struct sockaddr_in svraddr;
	const char *sndmsg = "hello";
	char rcv[64] = {0};
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(8888);
	svraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	int ret = connect(fd,(struct sockaddr*)&svraddr,sizeof(svraddr));
	if(ret)
	{
		cout << "connect fail" << endl;
		return;
	}
	printf("connect\n");
	ret = write(fd, sndmsg, strlen(sndmsg));
	if(!ret)
	{
		cout << "write fail" << endl;
		return;
	}
	printf("write\n");
	//协程读写默认的超时时间是500ms  mt_sys_hook.cpp 94,现在改为10s测试
	ret = read(fd, rcv, sizeof(rcv));
	if(ret)
	{
		cout << "rsp:" << rcv << endl;
	}

	cout <<  MtFrame::Instance()->GetActiveThread() 
		<< " " << rcv 
		<< " ret:" << ret
		<< endl;
	return ;
}

int Process(void * arg)
{
	int cnt = *(int*)arg;
	if(MtFrame::Instance()->CreateThread(threadEntry, arg,true) == NULL)
	{
		return -1;
	}
	return 0;
}

int main()
{
	mt_set_hook_flag();
	if(!mt_init_frame())
	{
		cout << "init frame fail" << endl;
		return -1;
	}

	int i = 0;
	while(1)
	{
		if(i++ > 0)
		{
			break;
		}
		Process(&i);
	}
	while(1)
	{
		mt_sleep(100000);
	}
	return 0;
}
