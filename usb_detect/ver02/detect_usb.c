#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <stddef.h>  
#include <string.h>  
#include <errno.h>  
#include <getopt.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <signal.h>  
#include <getopt.h>  
#include <sys/time.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <sys/select.h>  
#include <linux/types.h>  
#include <linux/netlink.h>  
#include <sys/types.h>
#include <libudev.h>
#include <pthread.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include "detect_usb.h"

#include <iconv.h>

F_DETECTFUN gDetectFunc;

static S_DEV_INFO_MNG gDevInfoMng;
static char cmd_buff[1024];
static char cmd_res_buff[1024];
static char au1strConv[1024];

static pthread_t monitor_thread;

static int code_convert(char *from_charset, char *to_charset, char *inbuf, int inlen, char *outbuf, int outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);

	if (cd == 0) 
	return 1;

	memset(outbuf, 0, outlen);

	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) 
	return 1;

	iconv_close(cd);
	return 0;
}
static char* shellcmd(char* cmd, char* buff, int size)
{
  char temp[256];
  FILE* fp = NULL;
  int offset = 0;
  int len;
   
  fp = popen(cmd, "r");
  if(fp == NULL)
  {
    return NULL;
  }
 
  while(fgets(temp, sizeof(temp), fp) != NULL)
  {
    len = strlen(temp);
    if(offset + len < size)
    {
      strcpy(buff+offset, temp);
      offset += len;
    }
    else
    {
      buff[offset] = 0;
      break;
    }
  }
   
  if(fp != NULL)
  {
    pclose(fp);
  }
 
  return buff;
}

static char * getDeviceName(char *devFileName)
{
	memset(cmd_buff, 0, sizeof(cmd_buff));
	memset(cmd_res_buff, 0, sizeof(cmd_res_buff));
	sprintf(cmd_buff, "blkid -o value -s LABEL %s", devFileName);
	char* pstrRes = shellcmd(cmd_buff, cmd_res_buff, sizeof(cmd_res_buff));
	if (NULL == pstrRes)
	{
		return "";
	}
	int len = 0;
	len = strlen(pstrRes);
	if (len > 0)
	{
		pstrRes[len - 1] = '\0'; //delete \n
	}
	
	memset(au1strConv,0,sizeof(au1strConv));
	code_convert("gb2312","utf-8",pstrRes,len,au1strConv,1024);
	return au1strConv;
}

static char * getDeviceFStype(char *devFileName)
{
	memset(cmd_buff, 0, sizeof(cmd_buff));
	memset(cmd_res_buff, 0, sizeof(cmd_res_buff));
	sprintf(cmd_buff, "blkid -o value -s TYPE %s", devFileName);
	char* pstrRes = shellcmd(cmd_buff, cmd_res_buff, sizeof(cmd_res_buff));
	if (NULL == pstrRes)
	{
		return "";
	}
	int len = 0;
	len = strlen(pstrRes);
	if (len > 0)
	{
		pstrRes[len - 1] = '\0'; //delete \n
	}
	
	return pstrRes;
}

static void Udev_Enumrate()
{
    struct udev* udev_ancestor=NULL;
    struct udev_enumerate* udev_enum=NULL;
    struct udev_list_entry* device_fistentry=NULL;
    struct udev_list_entry *dev_list_entry=NULL; 
    struct udev_device *dev=NULL;
    udev_ancestor=udev_new();
    udev_enum=udev_enumerate_new(udev_ancestor);
    if(udev_enumerate_add_match_subsystem (udev_enum, "block")==0)
    {
        //cout<<"add block device to match subsystem successful"<<endl;
    }
     if(udev_enumerate_add_match_property (udev_enum, "DEVTYPE","partition")==0)
    {
        //cout<<"add block device to match subsystem successful"<<endl;
    }   
    //Scan the system under /sys/
    udev_enumerate_scan_devices(udev_enum);
    
    //get the first entry of the device list
    device_fistentry=udev_enumerate_get_list_entry(udev_enum);

    udev_list_entry_foreach(dev_list_entry, device_fistentry)
    {
		const char *path;
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev_ancestor, path);
		const char* node = udev_device_get_devnode(dev);
		const char* sysPath = udev_device_get_syspath(dev);
		const char* Subsystem = udev_device_get_subsystem(dev); 
		const char* Devtype = udev_device_get_devtype(dev);   

       dev = udev_device_get_parent_with_subsystem_devtype(
                                dev,
                                "usb",
                                "usb_device");

       if (!dev) 
        {

        }
        else
        {   
			
//	printf("=========== add ====== \n");
//	printf("Got Device(Code):\n");      
//	printf("  Node: %s\n", node);
//	printf("  VID/PID: %s %s\n",udev_device_get_sysattr_value(dev,"idVendor"), udev_device_get_sysattr_value(dev, "idProduct"));
//	printf("  manufacturer : %s \n",udev_device_get_sysattr_value(dev,"manufacturer")); 
//	printf("  product : %s \n",udev_device_get_sysattr_value(dev,"product")); 
//	printf("  serial: %s\n",udev_device_get_sysattr_value(dev, "serial"));
//	printf("  Subsystem: %s\n", Subsystem);
//	printf("  Devtype: %s\n", Devtype);
//	printf("  sysPath: %s\n",sysPath);
		
		//
		//	typedef struct _S_DEV_INFO_MNG_
		//	{
		//		int i4DevInsState;//0 not : 1 inserted
		//		int i4ColdStart; //0 not :1 cold staet
		//		char au1DevVid[8];
		//		char au1DevPid[8];
		//		char au1DevNode[12];
		//		char au1Serial[256];
		//		char au1DevName[256];
		//		char au1DevType[8];
		//	}S_DEV_INFO_MNG;
		
			memset(&gDevInfoMng,0, sizeof(gDevInfoMng));
			gDevInfoMng.i4DevInsState = 1;
			gDevInfoMng.i4ColdStart = 1;
			strncpy(gDevInfoMng.au1DevVid, udev_device_get_sysattr_value(dev,"idVendor"), 5);
			strncpy(gDevInfoMng.au1DevPid, udev_device_get_sysattr_value(dev,"idProduct"), 5);
			strncpy(gDevInfoMng.au1DevNode, node, 10);// /dev/sda1
			strncpy(gDevInfoMng.au1Serial, udev_device_get_sysattr_value(dev, "serial"), 255);
			strncpy(gDevInfoMng.au1DevName, getDeviceName(gDevInfoMng.au1DevNode), 255);
			strncpy(gDevInfoMng.au1DevType, getDeviceFStype(gDevInfoMng.au1DevNode), 7);
			
			gDetectFunc(E_USB_EVT_DEVICE_INSERTED,&gDevInfoMng);
        }

        udev_device_unref(dev);
    }
    udev_enumerate_unref(udev_enum);
    udev_unref(udev_ancestor);

}

static void* udev_Monitor(void* para)
{
    struct udev* udev=NULL;
    struct udev_monitor * mon=NULL;
    struct udev_device *dev,*dev2;
    int fd;
    udev=udev_new();
    mon=udev_monitor_new_from_netlink(udev,"udev");
    
 //   udev_monitor_filter_add_match_subsystem_devtype(mon, "sound", "usb_device");
 //   udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", "usb_device");
    /*--(GM_Sha#3979)add for getting block event--*/
//    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", "disk");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", "partition");
    /*--(GM_Sha#3979)add for getting bInterfaceClass to detect usb mass storage device--*/
//    udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", "usb_interface");
    udev_monitor_enable_receiving(mon);
    fd = udev_monitor_get_fd(mon);
    while(1)
    {
                
        fd_set fds;
        struct timeval tv;
        int ret;
        
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        	
        ret = select(fd+1, &fds, NULL, NULL, &tv);
        if(ret>0 && FD_ISSET(fd,&fds))
        {
            dev = udev_monitor_receive_device(mon);
            if (dev) 
            {                    
                const char* action        = udev_device_get_action(dev);        
				memset(&gDevInfoMng,0, sizeof(gDevInfoMng));

		dev2 = udev_device_get_parent_with_subsystem_devtype(
				dev,
				"usb",
				"usb_device");
           //

//		printf("Got Device\n");
//		printf("  Node: %s\n", udev_device_get_devnode(dev));
//		printf("  VID/PID: %s %s\n",udev_device_get_sysattr_value(dev2,"idVendor"), udev_device_get_sysattr_value(dev2, "idProduct"));
//		printf("  manufacturer : %s \n",udev_device_get_sysattr_value(dev2,"manufacturer")); 
//		printf("  product : %s \n",udev_device_get_sysattr_value(dev2,"product")); 
//		printf("  serial: %s\n",udev_device_get_sysattr_value(dev2, "serial"));
//		printf("  Subsystem: %s\n", udev_device_get_subsystem(dev));
//		printf("  Devtype: %s\n", udev_device_get_devtype(dev));
//		printf("  sysPath: %s\n",udev_device_get_syspath(dev));						
//				printf("Got Device\n");
				gDevInfoMng.i4ColdStart = 0;
				strncpy(gDevInfoMng.au1DevVid, udev_device_get_sysattr_value(dev2,"idVendor"), 5);
				strncpy(gDevInfoMng.au1DevPid, udev_device_get_sysattr_value(dev2,"idProduct"), 5);
				strncpy(gDevInfoMng.au1DevNode, udev_device_get_devnode(dev), 10);// /dev/sda1
				strncpy(gDevInfoMng.au1Serial, udev_device_get_sysattr_value(dev2, "serial"), 255);
				strncpy(gDevInfoMng.au1DevName, getDeviceName(gDevInfoMng.au1DevNode), 255);
				strncpy(gDevInfoMng.au1DevType, getDeviceFStype(gDevInfoMng.au1DevNode), 7);

				if (0 == strncmp(action, "add", strlen("add")))
		        {
					gDevInfoMng.i4DevInsState = 1;
					gDetectFunc(E_USB_EVT_DEVICE_REMOVED ,&gDevInfoMng);
		        }
				else
				{
					gDevInfoMng.i4DevInsState = 0;
					gDetectFunc(E_USB_EVT_DEVICE_REMOVED ,&gDevInfoMng);
				}				
                udev_device_unref(dev);
            }
            else 
            {
                printf("No Device from receive_device(). An error occured.\n");
            }        
        }
    }
return NULL;
}

/////////////////////////////////////////////////////////////////////
int DevMngInit(F_DETECTFUN psDetectFunc)
{
	memset(&gDevInfoMng,0, sizeof(gDevInfoMng));
	monitor_thread = 0;
	gDetectFunc = psDetectFunc;
	return 0;
}

int DevMngStart()
{
    Udev_Enumrate();
	int err=0;
	err=pthread_create(&monitor_thread, NULL,udev_Monitor, NULL);
	
	return 0;
}

int DevMngDeInit()
{
    if(monitor_thread!=0)
    {
        pthread_join(monitor_thread,NULL);
    }
	gDetectFunc = NULL;
	
	return 0;
}
