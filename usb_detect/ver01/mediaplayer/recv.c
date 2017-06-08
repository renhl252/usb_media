// Error : Message too long
// 原因: Receive Msg Buffer 大小必须大于等于mq_open attr中的mq_msgsize

#include <mqueue.h>
#include "common.h"
#include "usb_detect.h"

char RecvBuffer[D_RECV_BUFFER_SIZE] = {0};


int OpenMsgQueue();
int CloseMsgQueue();
int GetData();
char *m_pu1Buffer; 
mqd_t stMsgQueue;



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



int GetData()
{
	LOG_STR();
	int i4Ret = AFC_NG;
	memset(RecvBuffer, 0, sizeof(RecvBuffer));	
	i4Ret = mq_receive(stMsgQueue, (char *)RecvBuffer, D_RECV_BUFFER_SIZE, NULL);
	if (-1 == i4Ret)
	{
		LOG_STR("Error #### mq_receive ####");
		return AFC_NG;
	}
	S_USB_DETECT_INFO *pusbInfo = (S_USB_DETECT_INFO *)RecvBuffer;
	LOG_STR("===========start===========\n");
	LOG_STR("event = %s",(char *)(pusbInfo->event));
	LOG_STR("devname = %s",(char *)pusbInfo->devname);
	LOG_STR("fstype = %s",(char *)pusbInfo->fstype);
	LOG_STR("fstypever = %s",(char *)pusbInfo->fstypever);
	LOG_STR("uuid = %s",(char *)pusbInfo->uuid);
	LOG_STR("serial = %s",(char *)pusbInfo->serial);
	LOG_STR("label = %s",(char *)pusbInfo->label);
	LOG_BIN((char *)pusbInfo->label, sizeof(pusbInfo->label));

	LOG_STR("vendorid = %s",(char *)pusbInfo->vendorid);
	LOG_STR("===========end  ==========\n");		
	return AFC_OK;
}	

int main()
{


	OpenMsgQueue();

	while(1)
	{
		GetData();
	}

	CloseMsgQueue();
	return 0;
}
