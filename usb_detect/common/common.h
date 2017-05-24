
#ifndef __COMMON_H_H_H__

#define __COMMON_H_H_H__

#include <stdio.h>

#include <sys/un.h>

#include <sys/errno.h>

#include "log.h"

#define D_MQ_NAME "/mq_20120806"

#include <sys/stat.h>

#define D_RECV_BUFFER_SIZE 8192
#define D_SEND_BUFFER_SIZE 8192
	
#define AFC_OK (0)
#define AFC_NG (-1)	

extern int errno;

#endif //__COMMON_H_H_H__
