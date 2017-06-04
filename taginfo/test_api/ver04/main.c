#include <string.h>

#include "log.h"
#include "tag_c.h"
#include "db_operation.h"
#include "media_tag.h"
#define	D_FOLDER_LAYER_MAX_SIZE					(8)

#define	        D_FILE_LENGTH_MAX           			( 256+64) 
#define			D_PATH_MAX_SIZE							(D_FILE_LENGTH_MAX *D_FOLDER_LAYER_MAX_SIZE )

unsigned char filefullpath[D_PATH_MAX_SIZE];
unsigned char filename[D_FILE_LENGTH_MAX];

int main()
{
	int_db();
	initMediaTagInfo();
	
	long file_count = GetFileCount();
	long index = 1;
	S_MEDIA_TAG_INFO stMediaTagInfo;
	
	for (index = 1;index <= file_count; index++)
	{
		memset(filefullpath,0,sizeof(filefullpath));
		memset(filename,0,sizeof(filename));
		GetFileFullPath(filefullpath,filename,index);
		printf("index = [%ld]%s\n",index,filefullpath);
		
		memset(&stMediaTagInfo,0,sizeof(S_MEDIA_TAG_INFO));
		int bRet = getMediaTagInfo(filefullpath, &stMediaTagInfo);
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

		long artistid = get_tag_id("ARTIST", stMediaTagInfo.au1Artist);
		long albumid = get_tag_id("ALBUM", stMediaTagInfo.au1Album);
		long genreid = get_tag_id("GENRE", stMediaTagInfo.au1Genre);

		if (strlen(stMediaTagInfo.au1Title) == 0)
		{
			printf("filename = %s\n", filename);
			updateFileInfo(index,artistid,albumid,genreid,filename);
		}
		else
		{
			printf("title = %s\n",stMediaTagInfo.au1Title);
			updateFileInfo(index,artistid,albumid,genreid,stMediaTagInfo.au1Title);
		}
	}
	
	close_db();
	deinitMediaTagInfo();
	return 0;
}

