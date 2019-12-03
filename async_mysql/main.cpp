#include "test.h"

int main()
{
	srandom(time(NULL));
	mt_set_hook_flag();	
	if(!mt_init_frame())
	{
		cout << "init frame fail" << endl;
		return -1;
	}

	if(!MysqlPool::Inst().Init(1))
	{
		printf("init mysql pool fail!\n");
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
		mt_sleep(10000);
	}
	return 0;
}

