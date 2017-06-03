
#include <stdio.h>
#include "media_tag.h"
	
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("=========ERROR: argc : %d==========\n",argc);
		return -1;
	}
	
	printf("=========file : %s==========\n",argv[1]);
	
	S_MEDIA_TAG_INFO stMediaTagInfo;
	
	initMediaTagInfo();
	
	int bRet = getMediaTagInfo(argv[1], &stMediaTagInfo);
	if (bRet != 0)
	{
		printf("!!!not support !!!\n");
	}
	else
	{
		printf("title = %s\n",stMediaTagInfo.au1Title);
		printf("artist = %s\n",stMediaTagInfo.au1Artist);
		printf("album = %s\n",stMediaTagInfo.au1Album);
		printf("genre = %s\n",stMediaTagInfo.au1Genre);
	
	}
	
	deinitMediaTagInfo();
	
	return 0;
}
