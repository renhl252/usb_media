//http://www.runoob.com/sqlite/sqlite-c-cpp.html
//创建表
//下面的 C 代码段将用于在先前创建的数据库中创建一个表：

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>
#include "log.h"

static   sqlite3 *db;
int db_open()
{
   int  rc;
 //  rc = sqlite3_open("/tmp/test.db", &db);
 rc = sqlite3_open("test.db", &db);
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
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		LOG_STR("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	return 1;
}


int get_record_count(char *sql)
{
    int rc;
	 int i4Ret = -1;
   sqlite3_stmt  *stmt = NULL;
    rc = sqlite3_prepare_v2(db , sql , strlen(sql) , &stmt , NULL);
    if(rc != SQLITE_OK)
    {
        if(stmt)
        {
            sqlite3_finalize(stmt);
        }
        return i4Ret;
    }
    int nColumn = sqlite3_column_count(stmt);
    int vtype , i;
    do{ 
        rc = sqlite3_step(stmt);
        if(rc == SQLITE_ROW)
        {
             
            for(i = 0 ; i < nColumn ; i++ )
            {
                vtype = sqlite3_column_type(stmt , i);
                if(vtype == SQLITE_INTEGER)
                {
                    //printf("%d \n" ,sqlite3_column_int(stmt , i));
                    i4Ret = sqlite3_column_int(stmt , i);
                    break;
                }
            }
        }
        else if(rc == SQLITE_DONE)
        {
            break;
        }
        else
        {
            printf("Select faile\n");
            sqlite3_finalize(stmt);
            break;
        }
    }while(1);
    sqlite3_finalize(stmt);
    
    return i4Ret;
}


int get_record_text(char *sql,char *buffer)
{
    int rc;
	 int i4Ret = -1;
   sqlite3_stmt  *stmt = NULL;
    rc = sqlite3_prepare_v2(db , sql , strlen(sql) , &stmt , NULL);
    if(rc != SQLITE_OK)
    {
        if(stmt)
        {
            sqlite3_finalize(stmt);
        }
        return i4Ret;
    }
    int nColumn = sqlite3_column_count(stmt);
    int vtype , i;
    do{ 
        rc = sqlite3_step(stmt);
        if(rc == SQLITE_ROW)
        {
            for(i = 0 ; i < nColumn ; i++ )
            {
                vtype = sqlite3_column_type(stmt , 0);
                if(vtype == SQLITE_TEXT)
                {
                    strcpy(buffer,(const char *)sqlite3_column_text(stmt , 0));
                    break;
                }
            }
        }
        else if(rc == SQLITE_DONE)
        {
            break;
        }
        else
        {
            printf("Select faile\n");
            sqlite3_finalize(stmt);
            break;
        }
    }while(1);
    sqlite3_finalize(stmt);
    
    return i4Ret;
}

