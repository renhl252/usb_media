//http://www.runoob.com/sqlite/sqlite-c-cpp.html
//创建表
//下面的 C 代码段将用于在先前创建的数据库中创建一个表：

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include "log.h"

static   sqlite3 *db;
int db_open()
{
   int  rc;
   rc = sqlite3_open("/tmp/test.db", &db);
   if(rc)
   {
      	return -1;
   }
   
   return 1;
}

int db_close()
{
   sqlite3_close(db);
   db = NULL;
   return 1;
}


int db_create_table(char *sql)
{
	int  rc;
	char *zErrMsg = 0;
	LOG_STR("sql: %s\n", sql);
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		LOG_STR("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	return 1;
}

int db_insertdata(char *sql)
{
	int  rc;
	char *zErrMsg = 0;
	LOG_STR("sql: %s\n", sql);
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		LOG_STR("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	return 1;
}

