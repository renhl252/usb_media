#include <string.h>

#include "log.h"
#include "db.h"

int	int_db()
{
	db_open();
	return 0;
}

int	close_db()
{
	db_close();
	return 0;
}

int get_db_count(char* sql)
{
	return get_record_count(sql);
}

void get_item_text(char* sql)
{
	return 	db_get_item_text(sql);
}