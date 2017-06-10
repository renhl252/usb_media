#ifndef _DETECT_USB_H_
#define _DETECT_USB_H_

typedef struct _S_DEV_INFO_MNG_
{
	int i4DevInsState;//0 not : 1 inserted
	int i4ColdStart; //0 not :1 cold staet
	char au1DevVid[8];
	char au1DevPid[8];
	char au1DevType[8];
	char au1DevNode[12];
	char au1Serial[256];
	char au1DevName[256];
}S_DEV_INFO_MNG;

typedef enum _USB_CallbackEvent_
{
	E_USB_EVT_DEVICE_INSERTED,
	E_USB_EVT_DEVICE_REMOVED 
}USB_CallbackEvent;

typedef void (*F_DETECTFUN)(USB_CallbackEvent eEvent, void *pvEventData);


int DevMngInit(F_DETECTFUN psDetectFunc);
int DevMngStart();
int DevMngDeInit();


#endif //_DETECT_USB_H_