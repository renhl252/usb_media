#include <stdio.h>
#include <string.h>
#include "detect_usb.h"
void print_dev(S_DEV_INFO_MNG *psDevInfoMng)
{
	printf("====================\n");
	printf("i4DevInsState = %d\n", psDevInfoMng->i4DevInsState);
	printf("i4ColdStart = %d\n", psDevInfoMng->i4ColdStart);
	printf("au1DevVid = %s\n", psDevInfoMng->au1DevVid);
	printf("au1DevPid = %s\n", psDevInfoMng->au1DevPid);
	printf("au1DevNode = %s\n", psDevInfoMng->au1DevNode);
	printf("au1Serial = %s\n", psDevInfoMng->au1Serial);
	printf("au1DevName = %s\n", psDevInfoMng->au1DevName);
	printf("au1DevType = %s\n", psDevInfoMng->au1DevType);

}
void DetectCBFunc(USB_CallbackEvent eEvent, void *pvEventData)
{
	
	switch(eEvent)
	{
		case E_USB_EVT_DEVICE_INSERTED:
			print_dev(pvEventData);
		break;
		case E_USB_EVT_DEVICE_REMOVED:
			print_dev(pvEventData);
		break;
	}
}

int main(void)
{
	DevMngInit(DetectCBFunc);
	
	DevMngStart();
	
	DevMngDeInit();
	
  return 0;
}