#include "micro_thread.h"
#include "mt_api.h"
#include <vector>
#include <iostream>
#include <unistd.h>
#include <string.h>

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
	struct sockaddr_in cliaddr;
	char str[64] = {"hello"};
	char rcv[64] = {0};
	int size = 64;
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(8888);
	cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t len = sizeof cliaddr;
	int ret = mt_tcpsendrcv(&cliaddr, (void*)str, strlen(str), (void*)rcv, size, 10000, chek_func);
	cout <<  MtFrame::Instance()->GetActiveThread() 
		<< " " << rcv 
		<< " ret:" << ret
		<< endl;
	return ;
}

int Process(void * arg)
{
	int cnt = *(int*)arg;
	cout << cnt << "begin" << endl;
	if(MtFrame::Instance()->CreateThread(threadEntry, arg,true) == NULL)
	{
		return -1;
	}
	cout << cnt << "end " << endl;
	return 0;
}

int main()
{
	if(!mt_init_frame())
	{
		cout << "init frame fail" << endl;
		return -1;
	}

	int i = 0;
	while(1)
	{
		if(i++ > 1)
		{
			break;
		}
		Process(&i);
		mt_sleep(2);
	}
	while(1)
	{
		mt_sleep(10);
	}
	return 0;
}
