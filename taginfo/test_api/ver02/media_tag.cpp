//g++ tag_test.cpp -I ../taglib/include/taglib/ -L ../taglib/lib/ -ltag -lz -o test_tag
//for p in ./*; do ../../test_tag "$p";done
#include <string.h>
#include <stdio.h>

#include "tag.h"
#include "fileref.h"
#include "media_tag.h"

////////////////////////////////////////////////////////////////////////////////////////////
#include "fileref.h"
#include "mpegfile.h"
#include <string.h>
class MyFileTypeResolver : public TagLib::FileRef::FileTypeResolver
{
	TagLib::File *createFile(const char *fileName,
	        bool readProperties,
	        TagLib::AudioProperties::ReadStyle propertiesStyle) const
	{
	    const char *ext = strrchr(fileName, '.');
	    if(ext && !strcasecmp(ext, ".aac"))
	    {
	        return new TagLib::MPEG::File(fileName, readProperties, propertiesStyle);
	    }
	    return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////
static TagLib::FileRef::FileTypeResolver *aacFileTypeResolver  = NULL;

int initMediaTagInfo()
{
	aacFileTypeResolver = new MyFileTypeResolver();
	TagLib::FileRef::addFileTypeResolver(aacFileTypeResolver);
	return 0;
}

int deinitMediaTagInfo()
{
	if(NULL != aacFileTypeResolver)
	{
		delete aacFileTypeResolver;
		aacFileTypeResolver = NULL;
	}
	return 0;
}

int getMediaTagInfo(const char *pu1FileName, S_MEDIA_TAG_INFO *pstMediaTagInfo)
{
	
	if (NULL == pu1FileName ||
		NULL ==  pstMediaTagInfo)
	{
		return -1;
	}
	
	TagLib::FileRef f(pu1FileName);
	if (NULL == f.tag())
	{
		return -1;
	}
	
	TagLib::String titleStr = f.tag()->title();
	TagLib::String artistStr = f.tag()->artist();
	TagLib::String albumStr = f.tag()->album();
	TagLib::String genreStr = f.tag()->genre();
	
	const char* title = titleStr.toCString(true);
	const char* artist = artistStr.toCString(true);
	const char* album = albumStr.toCString(true);
	const char* genre = genreStr.toCString(true);
	
	memset(pstMediaTagInfo,0,sizeof(S_MEDIA_TAG_INFO));
	int len = 0;
	
	len = strlen(title);
	if (len >= D_MEDIA_TAG_MAX_LEN)
	{
		len = D_MEDIA_TAG_MAX_LEN - 1;
	}
	strncpy(pstMediaTagInfo->au1Title, title,len);
	len = strlen(artist);
	if (len >= D_MEDIA_TAG_MAX_LEN)
	{
		len = D_MEDIA_TAG_MAX_LEN - 1;
	}
	strncpy(pstMediaTagInfo->au1Artist, artist,len);
	len = strlen(album);
	if (len >= D_MEDIA_TAG_MAX_LEN)
	{
		len = D_MEDIA_TAG_MAX_LEN - 1;
	}
	strncpy(pstMediaTagInfo->au1Album, album,len);
	len = strlen(genre);
	if (len >= D_MEDIA_TAG_MAX_LEN)
	{
		len = D_MEDIA_TAG_MAX_LEN - 1;
	}
	strncpy(pstMediaTagInfo->au1Genre, genre,len);

	return 0;
}

