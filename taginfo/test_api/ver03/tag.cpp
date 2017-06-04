extern "C" {
	#include "log.h"
}
#include "tag.h"
#include "fileref.h"
//#include <cstring>

//	#include <iostream> 
//	using namespace std;
static 	TagLib::String tag;
extern "C" const char* getTitleTag(const char *filename)
{
//	TagLib::FileRef f(filename);
//	TagLib::String title = f.tag()->title(); 	
//	TagLib::String artist = f.tag()->artist(); 	
//	TagLib::String album = f.tag()->album();	
//	TagLib::String genre = f.tag()->genre(); 	
//	unsigned int year = f.tag()->year();	 	
//	
//	cout << title << endl;
//	cout << artist << endl;
//	cout << album << endl;
//	cout << genre << endl;
//	cout << year << endl;
//	LOG_STR("title = %s",title.toCString(true));
//	LOG_BIN(title.toCString(true),strlen(title.toCString(true))+10);
//	LOG_STR("artist = %s",artist.toCString(true));	
//	LOG_STR("album = %s",album.toCString(true));	
//	LOG_STR("genre = %s",genre.toCString(true));	
	LOG_STR("filename = %s",filename);
//	TagLib::String str(filename,TagLib::String::UTF8);
	TagLib::FileRef f(filename);
	if (f.tag() == NULL)
	{
		return "";
	}
	tag = f.tag()->title();
	return tag.toCString(true);
}

extern "C" const char* getArtistTag(char *filename)
{
	TagLib::FileRef f(filename);
	if (f.tag() == NULL)
	{
		return "";
	}
	tag = f.tag()->artist();
	return tag.toCString(true);
}

extern "C" const char* getAlbumTag(char *filename)
{
	TagLib::FileRef f(filename);
	if (f.tag() == NULL)
	{
		return "";
	}
	tag = f.tag()->album();
	return tag.toCString(true);
	
}

extern "C" const char* getGenreTag(char *filename)
{
	TagLib::FileRef f(filename);
	if (f.tag() == NULL)
	{
		return "";
	}
	tag = f.tag()->genre();
	return tag.toCString(true);	
}