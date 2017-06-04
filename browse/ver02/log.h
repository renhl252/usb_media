
#ifndef __LOG_H__

#define __LOG_H__


#include <stdarg.h>
#include <stdio.h>

void log_print_bin(const char *filename, const char *function, unsigned long lineno, char *buf, unsigned long length);

#if 1
#define LOG_STR(fmt,...)		printf("[%s] [%d]"fmt"\n", __FUNCTION__,__LINE__,##__VA_ARGS__)
#define LOG_STR(fmt,...)		printf("[%s] [%d]"fmt"\n", __FUNCTION__,__LINE__,##__VA_ARGS__)
#define LOG_BIN(buffer,length)  log_print_bin(__FILE__,__FUNCTION__,__LINE__,(char *)buffer,length)
#define LOG_STR_RES(fmt,...)	printf("[%s] [%d]"fmt"\n", __FUNCTION__,__LINE__,##__VA_ARGS__)

#else

#define LOG_STR_RES(fmt,...)		printf("[%s] [%d]"fmt"\n", __FUNCTION__,__LINE__,##__VA_ARGS__)
#define LOG_STR(fmt,...)		
#define LOG_BIN(buffer,length)  

#endif

#endif
