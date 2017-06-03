//g++ tag_test.cpp -I ../taglib/include/taglib/ -L ../taglib/lib/ -ltag -lz -o test_tag
//for p in ./*; do ../../test_tag "$p";done
#include "tag.h"
#include "fileref.h"
#include <string.h>

#include <iostream> 
using namespace std;

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


bool getTagInfo(const char *filename)
{
	TagLib::FileRef f(filename);
	if (NULL == f.tag())
	{
		return false;
	}
	
	TagLib::String title = f.tag()->title();
	TagLib::String artist = f.tag()->artist();
	TagLib::String album = f.tag()->album();
	TagLib::String genre = f.tag()->genre();
	
	cout << "Title :"  << title.toCString(true) << endl;
	cout << "artist :" << artist.toCString(true) << endl;
	cout << "album :" << album.toCString(true) << endl;
	cout << "genre :" << genre.toCString(true) << endl;
	
	return true;
}

TagLib::FileRef::FileTypeResolver *aacFileTypeResolver;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "!!!!!!!!!! error !!!!!"<< endl;
		return -1;
	}
	
	aacFileTypeResolver = new MyFileTypeResolver();
	TagLib::FileRef::addFileTypeResolver(aacFileTypeResolver);
	
	cout << "==File== :"<< argv[1] << endl;
	bool bRet = getTagInfo(argv[1]);
	if (bRet != true)
	{
		cout << "not support!!" << endl;
	}
	
	if (aacFileTypeResolver != NULL)
	{
		delete aacFileTypeResolver;
	}
	return 0;
}

