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


#define DBHOST      "192.168.248.11"
#define DBUSER      "root"
#define DBPASS      "19901219"
#define DBPORT      3306
#define DBNAME      "test"
#define DBSOCK      NULL //"/var/lib/mysql/mysql.sock"
#define DBPCNT      0

extern unsigned int timeout;
extern const char *pStatement; ;
class MysqlPool{
		public:
				bool Init(int size);

				MYSQL* GetConn();

				bool Release(MYSQL* ptr);
				static MysqlPool& Inst(){
						static MysqlPool * _inst = new MysqlPool;
						return *_inst;
				}
		private:
				vector<pair<bool, MYSQL*> > _pool;
};

int chek_func(void *buf, int len);

void PrintTime();

void threadEntry(void * arg);

int Process(void * arg);
