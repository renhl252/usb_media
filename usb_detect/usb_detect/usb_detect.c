#include <stdio.h>
#include <string.h>
#include "usb_detect.h"

extern int OpenMsgQueue();
extern int CloseMsgQueue();
extern int WriteData(char * buffer, int buf_size);


#define D_NO_EVENT 1
#define D_NO_DEVNAME 2
#define D_NO_FSTYPE 3
#define D_NO_FSTYPEVER 4
#define D_NO_UUID 5
#define D_NO_SERAIL 6
#define D_NO_LABEL 7
#define D_NO_VENDORID 8

// "/usr/bin/usb_detect '%E{ACTION}' '%N'  '%E{ID_FS_TYPE}' '%E{ID_FS_VERSION}' '%E{ID_FS_UUID}' '%E{ID_SERIAL_SHORT}' '%E{ID_FS_LABEL}' '%E{ID_VENDOR_ID}' "

int main(int argc,char *argv[])
{
	if (argc == 1)
	{
		return -1;
	}
	
    FILE* fstream;
    fstream=fopen("/tmp/test.txt","a+");
    if(fstream==NULL)
    {
        printf("open file test.txt failed!\n");
        return -1;
    }
   S_USB_DETECT_INFO usbinfo;
   memset(&usbinfo, 0, sizeof(S_USB_DETECT_INFO));
   int index = 1;
OpenMsgQueue();

 
   fprintf(fstream,"%s\n", "========== start=============");
   for (index = 1; index < argc; index++)
   {
   		fprintf(fstream,"@@ %s\n",(char *)argv[index]);
		strcpy(usbinfo.event, argv[index]);

   }
   fprintf(fstream,"%s\n", "=============");
	strcpy(usbinfo.event, argv[D_NO_EVENT]);
   		fprintf(fstream,"%s\n",(char *)usbinfo.event);
	strcpy(usbinfo.devname, argv[D_NO_DEVNAME]);
   		fprintf(fstream,"%s\n",(char *)usbinfo.devname);
	strcpy(usbinfo.fstype, argv[D_NO_FSTYPE]);
   		fprintf(fstream,"%s\n",(char *)usbinfo.fstype);
	strcpy(usbinfo.fstypever, argv[D_NO_FSTYPEVER]);
   		fprintf(fstream,"%s\n",(char *)usbinfo.fstypever);
	strcpy(usbinfo.uuid, argv[D_NO_UUID]);
   		fprintf(fstream,"%s\n",(char *)usbinfo.uuid);
	strcpy(usbinfo.serial, argv[D_NO_SERAIL]);
   		fprintf(fstream,"%s\n",(char *)usbinfo.serial);
	strcpy(usbinfo.label, argv[D_NO_LABEL]);
   		fprintf(fstream,"%s\n",(char *)usbinfo.label);
	strcpy(usbinfo.vendorid, argv[D_NO_VENDORID]);
   		fprintf(fstream,"%s\n",(char *)usbinfo.vendorid);

   fprintf(fstream,"%s\n", "========== end ==============");
   WriteData((char *)&usbinfo, sizeof(usbinfo));    
    fclose(fstream);



CloseMsgQueue();


    return 0;
    
}
