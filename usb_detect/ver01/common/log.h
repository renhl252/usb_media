
#ifndef __LOG_H__

#define __LOG_H__

typedef unsigned char U08;
typedef unsigned long U32;
//typedef unsigned char UCHAR_TYPE;

#define UCHAR_TYPE unsigned char

#include <stdarg.h>
#include <stdio.h>

void log_print_bin(const char *filename, const char *function, unsigned long lineno, char *buf, unsigned long length);


#define LOG_STR(fmt,...)		printf("[%s] [%d]"fmt"\n", __FUNCTION__,__LINE__,##__VA_ARGS__)
#define LOG_BIN(buffer,length)  log_print_bin(__FILE__,__FUNCTION__,__LINE__,(char *)buffer,length)

#endif