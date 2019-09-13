#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mysql.h"
#include <unistd.h>

#define THREAD_NUM  4
#define DBHOST      "127.0.0.1"
#define DBUSER      "root"
#define DBPASS      ""
#define DBPORT      3306
#define DBNAME      "test"
#define DBSOCK      NULL //"/var/lib/mysql/mysql.sock"
#define DBPCNT      0


int main()
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	unsigned int num_fields;
	unsigned int i;
	unsigned int timeout = 3000;
	const char *pStatement = "select * from innodb_lock limit 2";
	MYSQL *mysql = mysql_init(NULL);

	if (mysql == NULL)
	{
		printf("mysql init failed: %s\n", mysql_error(mysql));
		return 0;
	}

	mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

	if (mysql_real_connect(mysql, DBHOST, DBUSER, DBPASS, DBNAME, DBPORT, DBSOCK, DBPCNT) == NULL)
	{
		printf("connect failed: %s\n", mysql_error(mysql));
		mysql_close(mysql);
		return 0;
	}

	printf("connect succssfully\n");

	if (0 != mysql_real_query(mysql, pStatement, strlen(pStatement)))
	{
		printf("query failed: %s\n",  mysql_error(mysql));
		mysql_close(mysql);
		return 0;
	}

	result = mysql_store_result(mysql);

	if (result == NULL)
	{
		printf("fetch result failed: %s\n", mysql_error(mysql));
		mysql_close(mysql);
		return 0;
	}

	num_fields = mysql_num_fields(result);
	printf("numbers of result: %d\n", num_fields);

	while (NULL != (field = mysql_fetch_field(result)))
	{
		printf("field name: %s\n", field->name);
	}

	while (NULL != (row = mysql_fetch_row(result)))
	{
		unsigned long *lengths;
		lengths = mysql_fetch_lengths(result);

		for (i = 0; i < num_fields; i++)
		{
			printf("{%.*s} ", (int) lengths[i], row[i] ? row[i] : "NULL");
		}

		printf("\n");
	}

	mysql_free_result(result);
	mysql_close(mysql);
	return 0;
}
