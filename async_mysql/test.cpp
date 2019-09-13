#include "micro_thread.h"
#include "mt_api.h"
#include "mt_sys_hook.h"
#include <vector>
#include <utility>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "mysql.h"

using namespace std;
using namespace NS_MICRO_THREAD;


#define DBHOST      "127.0.0.1"
#define DBUSER      "root"
#define DBPASS      ""
#define DBPORT      3306
#define DBNAME      "test"
#define DBSOCK      NULL //"/var/lib/mysql/mysql.sock"
#define DBPCNT      0

unsigned int timeout = 0;
const char *pStatement = "select * from innodb_lock where extra=";
class MysqlPool{
	public:
		bool Init(int size)
		{
			while(size--)
			{
				MYSQL *mysql = mysql_init(NULL);

				if (mysql == NULL)
				{
					printf("mysql init failed: %s\n", mysql_error(mysql));
					return false;
				}

				mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

				if (mysql_real_connect(mysql, DBHOST, DBUSER, DBPASS, DBNAME, DBPORT, DBSOCK, DBPCNT) == NULL)
				{
					printf("connect failed: %s\n", mysql_error(mysql));					  return false;
				}
				_pool.push_back(std::make_pair(0, mysql));
			}
			return true;
		}

		MYSQL* GetConn()
		{
			for(int i = 0; i < _pool.size(); i++)
			{
				if(_pool[i].first == 0)
				{
					_pool[i].first = 1;
					return _pool[i].second;
				}
			}
			return NULL;
		}
		
		bool Release(MYSQL* ptr)
		{
			for(int i = 0; i < _pool.size(); i++)
			{
				if(_pool[i].second == ptr)
					_pool[i].first = 0;
			}
			return NULL;
		}
	static MysqlPool& Inst(){
		static MysqlPool * _inst = new MysqlPool;
		return *_inst;
	}
	private:
		vector<pair<bool, MYSQL*> > _pool;
};

int chek_func(void *buf, int len)
{
	return strlen((char*)buf);
}

void PrintTime()
{
	struct timeval tv;
    gettimeofday(&tv,NULL);
   cout << tv.tv_sec*1000000000+tv.tv_usec << endl;
}

void threadEntry(void * arg)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	unsigned int num_fields;
	unsigned int i;

	MYSQL * mysql = MysqlPool::Inst().GetConn();
	//add retry cnt
	while(!mysql)
	{
		mt_sleep(2000);
		mysql = MysqlPool::Inst().GetConn();
		if(mysql)
			cout << "reget mysql" << endl;
	}
	
	cout << "req: " << MtFrame::Instance()->GetActiveThread() 
		<< endl;
	PrintTime();
	//通过mt异步化mysql查询
	stringstream ss;
	ss << pStatement;
	ss << 10000+ random()%90000;
	if (0 != mysql_real_query(mysql, ss.str().c_str(), ss.str().size()))
	{
		printf("query failed: %s\n",  mysql_error(mysql));
		mysql_close(mysql);
		return ;
	}

	result = mysql_store_result(mysql);

	if (result == NULL)
	{
		printf("fetch result failed: %s\n", mysql_error(mysql));
		mysql_close(mysql);
		return ;
	}

	num_fields = mysql_num_fields(result);
	//printf("numbers of result: %d\n", num_fields);

	while (NULL != (field = mysql_fetch_field(result)))
	{
	//	printf("field name: %s\n", field->name);
	}

	while (NULL != (row = mysql_fetch_row(result)))
	{
		unsigned long *lengths;
		lengths = mysql_fetch_lengths(result);

		for (i = 0; i < num_fields; i++)
		{
			printf("rsp {%.*s} ", (int) lengths[i], row[i] ? row[i] : "NULL");
		}

		printf("\n");
	}

	mysql_free_result(result);

	//release mysql 
	MysqlPool::Inst().Release(mysql);
	cout <<  MtFrame::Instance()->GetActiveThread() 
		<< endl;
	PrintTime();
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
