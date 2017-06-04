#include <string.h>

#include "log.h"
#include "tag_c.h"
#include "db_operation.h"
#include "media_tag_c.h"
#define	D_FOLDER_LAYER_MAX_SIZE					(8)

#define	        D_FILE_LENGTH_MAX           			( 256+64) 
#define			D_PATH_MAX_SIZE							(D_FILE_LENGTH_MAX *D_FOLDER_LAYER_MAX_SIZE )

unsigned char filefullpath[D_PATH_MAX_SIZE];
unsigned char filename[D_FILE_LENGTH_MAX];

int main()
{
	int_db();
	long file_count = GetFileCount();
	long index = 1;
	
	for (index = 1;index <= file_count; index++)
	{
		memset(filefullpath,0,sizeof(filefullpath));
		memset(filename,0,sizeof(filename));
		GetFileFullPath(filefullpath,filename,index);
		printf("index = [%ld]%s\n",index,filefullpath);
		const char * artist = getArtistTag(filefullpath);
		printf("artist = %s\n",artist);
		long artistid = get_tag_id("ARTIST", artist);
		const char * album = getAlbumTag(filefullpath);
		printf("album = %s\n",album);
		long albumid = get_tag_id("ALBUM", album);
		const char * genre = getGenreTag(filefullpath);
		printf("genre = %s\n",genre);
		long genreid = get_tag_id("GENRE", genre);
		const char * title = getArtistTag(filefullpath);
		//int titleid = get_tag_id("FILE", artist);
		if (strlen(title) == 0)
		{
			printf("filename = %s\n", filename);
			updateFileInfo(index,artistid,albumid,genreid,filename);
		}
		else
		{
			printf("title = %s\n",title);
			updateFileInfo(index,artistid,albumid,genreid,title);
		}
	}
	
	close_db();
	return 0;
}

