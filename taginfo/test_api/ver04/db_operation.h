
#ifndef __DB_OPERATION_H__
#define __DB_OPERATION_H__
int	int_db();
int	close_db();
int	InsertFolderInfo(signed long i4FolderID,const char * dirname,signed long i4FolderPathID);
int	InsertFolderPathInfo(signed long i4FolderPathID, unsigned char* u1FolderPath);
int	InsertFileInfo(signed long gi4FileID,const char * filename,signed long i4FolderPathID);
int	GetFileCount();
int GetFileFullPath(char *filepath,char * filename,long index);
int updateFileInfo(long index,long artistid,long albumid,long genreid,const char *title);


#endif //__DB_OPERATION_H__