

#include <string.h>
#include "log.h"


void log_print_bin(const char *filename, const char *function, unsigned long lineno, char *buf, unsigned long length)
{
	U08 len = strlen(filename) - 1;

	while(len > 0)
	{
		if(filename[len] != '/')
		{
			len--;
			continue;
		}
		break;		
	}
	if (filename[len] == '/')
	{
		len++;
	}

	printf("[ %s ][ %s ][ %ld ]", 	&filename[len], function, lineno);
	
	U32 index = 0;
	printf("[Hexdump]");
	printf("\n");
	for (index = 0; index < length; index++)
	{
		if (index != 0 && index % 32 == 0)
		{
			printf("\n");
		}
		printf("%02x ", (unsigned char)buf[index]);
	}
	printf("\n");

	return;
}


