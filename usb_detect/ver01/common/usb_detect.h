#ifndef __USB_DETECT_H__

#define  __USB_DETECT_H__
#define LEN_EVENT_MAX 10
#define LEN_DEVNAME_MAX 10
#define LEN_FSTYPE_MAX 10
#define LEN_FSTYPEVER_MAX 10
#define LEN_UUID_MAX 10
#define LEN_SERAIL_MAX 256
#define LEN_LABEL_MAX 256
#define LEN_VENDORID_MAX 10

typedef struct _S_USB_DETECT_INFO
{
	char event[LEN_EVENT_MAX];
	char devname[LEN_DEVNAME_MAX];
	char fstype[LEN_FSTYPE_MAX];
	char fstypever[LEN_FSTYPEVER_MAX];		
	char uuid[LEN_UUID_MAX];
	char serial[LEN_SERAIL_MAX];	
	char label[LEN_LABEL_MAX];
	char vendorid[LEN_VENDORID_MAX];	
}S_USB_DETECT_INFO;

#endif 
