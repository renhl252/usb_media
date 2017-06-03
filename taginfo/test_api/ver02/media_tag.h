#ifndef _MEDIA_TAG_H_
#define _MEDIA_TAG_H_
#ifdef __cplusplus
extern "C"
{
#endif

#define D_MEDIA_TAG_MAX_LEN 256
typedef struct _S_MEDIA_TAG_INFO_
{
	 char au1Title[D_MEDIA_TAG_MAX_LEN];
	 char au1Artist[D_MEDIA_TAG_MAX_LEN];
	 char au1Album[D_MEDIA_TAG_MAX_LEN];
	 char au1Genre[D_MEDIA_TAG_MAX_LEN];
}S_MEDIA_TAG_INFO;

int getMediaTagInfo(const char *pu1FileName, S_MEDIA_TAG_INFO *pstMediaTagInfo);

int initMediaTagInfo();

int deinitMediaTagInfo();

#ifdef __cplusplus
}
#endif

#endif