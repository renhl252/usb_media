#include <stdio.h>
#include <string.h>
#include "detect_usb.h"
#include "usb_mount.h"

static char au1MountPath[1024];

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

void HandleDetect(S_DEV_INFO_MNG *psDevInfoMng)
{
	print_dev(psDevInfoMng);
	memset(au1MountPath,0,sizeof(au1MountPath));
	snprintf(au1MountPath, sizeof(au1MountPath)-1,"/tmp/%s_%s_%s",psDevInfoMng->au1DevVid,psDevInfoMng->au1DevPid,psDevInfoMng->au1Serial);
	printf("au1MountPath = %s\n",au1MountPath);
	if (psDevInfoMng->i4DevInsState == 1)
	{
		MountFS(psDevInfoMng->au1DevNode, au1MountPath);
	}
	else
	{
		umountFS(au1MountPath);
	}
}

void DetectCBFunc(USB_CallbackEvent eEvent, void *pvEventData)
{
	
	switch(eEvent)
	{
		case E_USB_EVT_DEVICE_INSERTED:
			HandleDetect(pvEventData);
		break;
		case E_USB_EVT_DEVICE_REMOVED:
			HandleDetect(pvEventData);
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