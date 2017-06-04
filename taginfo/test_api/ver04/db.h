#ifndef __DB_H__
#define __DB_H__
int db_open();
int db_close();
int db_create_table(char *sql);
int db_insertdata(char *sql);
int get_record_count(char *sql);
int get_record_text(char *sql,char *buffer);

#endif
