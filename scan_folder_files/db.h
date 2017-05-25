#ifndef __DB_H__
#define __DB_H__
int db_open();
int db_close();
int db_create_table(char *sql);
int db_insertdata(char *sql);
#endif
