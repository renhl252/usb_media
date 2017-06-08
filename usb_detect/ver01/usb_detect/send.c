#include <mqueue.h>
#include "common.h"

int OpenMsgQueue();
int CloseMsgQueue();
int WriteData(char * buffer, int buf_size);

static mqd_t stMsgQueue;

int OpenMsgQueue()
{	
	LOG_STR();
	
	//打开或者创建mq
	stMsgQueue = mq_open(D_MQ_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO, NULL);
	if (-1 == stMsgQueue && errno != EEXIST)
	{
		LOG_STR("Error #### mq_open ####");
		return AFC_NG;
	}
	
	
	stMsgQueue = mq_open(D_MQ_NAME, O_RDWR);
	if (-1 == stMsgQueue)
	{
		LOG_STR("Error #### mq_open ####");
		return AFC_NG;
	}
	
	return AFC_OK;
}

int CloseMsgQueue()
{
	LOG_STR();
	int i4Ret = AFC_NG;

	//关闭mq
	//int mq_close(mqd_t mqdes);
	i4Ret = mq_close(stMsgQueue);
	if (-1 == i4Ret)
	{
		LOG_STR("Error #### mq_close ####");
		return AFC_NG;
	}
	
	//删除mq
	//mq_unlink(D_MQ_NAME);
	return AFC_OK;
}

int WriteData(char * buffer, int buf_size)
{
	int i4Ret = AFC_NG;
	LOG_STR();
	//TODO  send data size
	if (NULL == buffer || D_SEND_BUFFER_SIZE < buf_size
		|| buf_size == 0)
	{
		return i4Ret;
	}
	
	i4Ret = mq_send(stMsgQueue, buffer, buf_size, 0);
	if (-1 == i4Ret)
	{
		LOG_STR("Error #### mq_send ####");
		return AFC_NG;
	}	
	return AFC_OK;
}	

#if 0
char SendBuffer[D_SEND_BUFFER_SIZE] = {0};
int main()
{

	OpenMsgQueue();
	int i4Ret = 0;
	char *pStr =NULL;
	while(1)
	{
		printf("Input Send Data:");
		pStr = fgets(SendBuffer, D_SEND_BUFFER_SIZE, stdin);
		if (NULL == pStr)
		{
			LOG_STR("Error #### fgets ####");
			break;
		}

		if (!strncasecmp(SendBuffer, "quit", 4))
		{
			LOG_USR_STR("Send Exit!");
			break;
		}
		
		WriteData(pStr, D_SEND_BUFFER_SIZE);
	}


	CloseMsgQueue();
}
#endif

