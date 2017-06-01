#include <string.h>

#include "code.h"
#include "log.h"
#include "db.h"
static unsigned char gau1SQLBuffer[1024];

int	int_db()
{
	db_open();
	//create table
	char *sql = "CREATE TABLE FOLDER ("\
	    "ID           INTEGER PRIMARY KEY,"\
	    "SORTID       INTEGER DEFAULT 0,"\
	    "NAME         TEXT    NOT NULL,"\
	    "PATHID         INTEGER DEFAULT 0"\
		");";
	db_create_table(sql);
	sql = "CREATE TABLE FOLDERPATH ("\
			"ID           INTEGER PRIMARY KEY,"\
			"NAME         TEXT    NOT NULL"\
			");";
	db_create_table(sql);
	
	sql = "CREATE TABLE FILE ("\
			"ID           INTEGER PRIMARY KEY,"\
	   		"SORTID       INTEGER DEFAULT 0,"\
			"NAME         TEXT    NOT NULL,"\
	    	"PATHID         INTEGER DEFAULT 0,"\
	   		"TITLEID       INTEGER DEFAULT 0,"\
			"TITLE         TEXT    DEFAULT NULL,"\
	   		"ARTISTID       INTEGER DEFAULT 0,"\
	   		"ALBUMID       INTEGER DEFAULT 0,"\
	   		"GENREID       INTEGER DEFAULT 0"\
			");";
	db_create_table(sql);
	
	sql = "CREATE TABLE ARTIST ("\
		"ID      INTEGER PRIMARY KEY,"\
		"SORTID  INTEGER DEFAULT 0,"\
		"NAME    TEXT    NOT NULL "\
		"UNIQUE);";
	db_create_table(sql);
	sql = "CREATE TABLE ALBUM ("\
		"ID      INTEGER PRIMARY KEY,"\
		"SORTID  INTEGER DEFAULT 0,"\
		"NAME    TEXT    NOT NULL "\
		"UNIQUE);";
	db_create_table(sql);
	sql = "CREATE TABLE GENRE ("\
		"ID      INTEGER PRIMARY KEY,"\
		"SORTID  INTEGER DEFAULT 0,"\
		"NAME    TEXT    NOT NULL "\
		"UNIQUE);";
	db_create_table(sql);
	return 0;
}

int	close_db()
{
	db_close();
	return 0;
}

int	InsertFolderInfo(signed long i4FolderID,const char * dirname,signed long i4FolderPathID)
{
	memset(gau1SQLBuffer,0,sizeof(gau1SQLBuffer));
	sprintf(gau1SQLBuffer,"insert into FOLDER(ID,NAME,PATHID) values(%ld, \"%s\",%ld)",i4FolderID,dirname,i4FolderPathID);
	db_insertdata(gau1SQLBuffer);
	return 0;
}
int	InsertFolderPathInfo(signed long i4FolderPathID, unsigned char* u1FolderPath)
{
	memset(gau1SQLBuffer,0,sizeof(gau1SQLBuffer));
	sprintf(gau1SQLBuffer,"insert into FOLDERPATH(ID,NAME) values(%ld, \"%s\")",i4FolderPathID,u1FolderPath);
	db_insertdata(gau1SQLBuffer);
	return 0;
}

int	InsertFileInfo(signed long gi4FileID,const char * filename,signed long i4FolderPathID)
{
	memset(gau1SQLBuffer,0,sizeof(gau1SQLBuffer));
	sprintf(gau1SQLBuffer,"insert into FILE(ID,NAME,PATHID) values(%ld, \"%s\",%ld)",gi4FileID,filename,i4FolderPathID);
	db_insertdata(gau1SQLBuffer);
	
	return 0;
}



#if 0

int get_db_count(char* sql)
{
	return get_record_count(sql);
}

int	insert_data_db(char *sql)
{
	db_insertdata(sql);
	return 0;
}


I4 insert_tag_info(U4 pu4ExtType)
{
	if (pu4ExtType <= 3)
	{
		const char *artist = getArtistTag(gau1CurrentFilePath);
		gi4AristID = get_tag_id("ARTIST", artist);
		const char *album = getAlbumTag(gau1CurrentFilePath);
		gi4AlbumID = get_tag_id("ALBUM", album);
		const char *genre = getGenreTag(gau1CurrentFilePath);
		gi4GenreID = get_tag_id("GENRE", genre);
	}
	else
	{
		gi4AristID = get_tag_id("ARTIST", "");
		gi4AlbumID = get_tag_id("ALBUM", "");
		gi4GenreID = get_tag_id("GENRE", "");
	}
	return 0;
}

I4 get_tag_id(char* table_name, const char *tag)
{
	memset(gau1SQLBuffer,0,sizeof(gau1SQLBuffer));
	sprintf(gau1SQLBuffer,"select ID from '%s' where NAME='%s'",table_name,tag);
	int id = get_db_count(gau1SQLBuffer);
	if (id > 0)
	{
		return id;
	}
	memset(gau1SQLBuffer,0,sizeof(gau1SQLBuffer));
	sprintf(gau1SQLBuffer,"select count(ID) from '%s'",table_name);
	int count = get_db_count(gau1SQLBuffer);
	if (count >= 0)
	{
		count++;
		memset(gau1SQLBuffer,0,sizeof(gau1SQLBuffer));
		sprintf(gau1SQLBuffer,"insert into '%s' (ID,NAME) values(%d,'%s')",table_name,count,tag);
		insert_data_db(gau1SQLBuffer);
		return count;
	}
	LOG_STR_RES("Return ERROR: %s", gau1SQLBuffer);
	return -1;
}

#endif