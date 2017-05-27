#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "db.h"

#include "code.h"
#include "log.h"

#define	 D_OK          							(0) 
#define	 D_NG          							(-1)

#define D_FILE_CNT_IN_FOLDER_MAX				(1024)
#define D_FOLDER_CNT_IN_FOLDER_MAX				(1024)
#define D_FILE_CNT_MAX							(5000)
#define D_FOLDER_CNT_MAX						(5000)
#define	D_FOLDER_LAYER_MAX_SIZE					(8)

#define	        D_FILE_LENGTH_MAX           			( 256 + 64 ) 
#define			D_PATH_MAX_SIZE							( ((D_FILE_LENGTH_MAX + 1)/4 + 1)*D_FOLDER_LAYER_MAX_SIZE )


static U1	gau1CurrentPath[ D_PATH_MAX_SIZE + 4 ];
static U1	gau1CurrentFilePath[ D_PATH_MAX_SIZE + 4 + 255 ];

static U4	gu4CurrentPathSize;
static I4	gi4FileID;
static I4	gi4FolderID;
static I4	gi4FolderIDUpdateFlg;
static I4	gi4TotalFileCount;
static I4	gi4FolderLayerCount;
static I4	gi4TotalFolderCount;

static U1	gau1SQLBuffer[1024];

static I4 indexing_scan_files(const char* dir_name);

typedef enum _EN_FILE_TYPE
{
	D_FILE_EXTENTION_MP3 = 1,
	D_FILE_EXTENTION_WMA,
	D_FILE_EXTENTION_AAC,
	D_FILE_EXTENTION_M4A,
	D_FILE_EXTENTION_OTHER
}EN_FILE_TYPE;
	
#define D_FILE_EXT_LENGTH 4
static I4 get_fileextensiontype( U1* pu1FileName, U4* pu4ExtType );

I4 start_indexing()
{
	memset(gau1CurrentPath, 0, sizeof(gau1CurrentPath));
	gu4CurrentPathSize = 0;
	
	gi4FileID = 0;
	gi4FolderID = 0;
	
	gi4TotalFileCount = 0;
	gi4FolderLayerCount = 1;
	gi4TotalFolderCount = 0;
	
	gi4FolderIDUpdateFlg = 0;
	
	return 0;
}

I4 end_indexing()
{
	
	return 0;
}

I4 indexing_scan_folder(const char* dir_name)
{
	U4 u4FolderSize = 0;
	DIR*				pstDir;
	struct dirent*		pstDirEnt;
	I4					i4Result = D_OK;
	U4					u4LoopCnt = 0;
	
	LOG_STR("Call");
	LOG_STR("gau1CurrentPath = %s", gau1CurrentPath);
	LOG_STR("gu4CurrentPathSize = %ld", gu4CurrentPathSize);

	//check of the max folder 
	if (D_FOLDER_CNT_MAX <= gi4TotalFolderCount)
	{
		return i4Result;
	}
	
	/////////////////////////////////
	//scan files in the current path
	indexing_scan_files(dir_name);
	
	/////////////////////////////////
	//scan folders in the current path
	
	//check of the max folder depth 
	if (gi4FolderLayerCount >= D_FOLDER_LAYER_MAX_SIZE)
	{
		LOG_STR_RES("@@ max layer : i4FolderLayerCount = %ld", gi4FolderLayerCount);
		return i4Result;
	}
	
	errno = 0;
	pstDir = opendir((const char*)gau1CurrentPath);
	if (NULL == pstDir)
	{
		LOG_STR("opendir() return error(%d): %s", errno, strerror(errno));
		i4Result = D_NG;
	}
	else
	{
		while (u4LoopCnt < D_FOLDER_CNT_IN_FOLDER_MAX)
		{
			//check cancel or not
			
			errno = 0;
			pstDirEnt = readdir( pstDir );
			if (NULL == pstDirEnt && 0 != errno)
			{
				LOG_STR("readdir() return error(%d): %s", errno, strerror(errno));
			}
	
			if (NULL == pstDirEnt)
			{
				i4Result = ( 0 == errno ) ? D_OK : D_NG;
				break;
			}
			
			if (DT_DIR != pstDirEnt->d_type)
			{
				LOG_STR("!! File is not directory !!");
				continue;
			}
			
			u4LoopCnt++;
			
			if ( NULL == pstDirEnt->d_name )
			{
				continue;
			}
#if 1

			if ( 0 == strcmp( pstDirEnt->d_name, ".") )
			{
				u4LoopCnt--;
				continue;
			}
			if ( 0 == strcmp( pstDirEnt->d_name, "..") )
			{
				u4LoopCnt--;
				continue;
			}
#endif
			u4FolderSize = strlen((const char*)pstDirEnt->d_name);
			if (D_FILE_LENGTH_MAX <= u4FolderSize + 1 + gu4CurrentPathSize)
			{
				continue;
			}
			
			strncat( (char*)gau1CurrentPath, "/", 1);	
			strncat( (char*)gau1CurrentPath, (const char*)pstDirEnt->d_name, u4FolderSize);	
			gu4CurrentPathSize += (u4FolderSize + 1 );

			gi4FolderLayerCount++;
			indexing_scan_folder((const char*)pstDirEnt->d_name);
			gi4FolderLayerCount--;

			gu4CurrentPathSize -= (u4FolderSize + 1 );
			gau1CurrentPath[gu4CurrentPathSize] = 0;
		}

		errno = 0;
		i4Result = closedir(pstDir);
		if ( 0 != i4Result )
		{
			LOG_STR("closedir() return error(%d): %s", errno, strerror(errno));
			i4Result = D_NG;
	 	}
	}
	
	return i4Result;
}

I4 indexing_scan_files(const char* dir_name)
{
	U4 u4FolderSize = 0;
	DIR*				pstDir;
	struct dirent*		pstDirEnt;
	I4					i4Result = D_OK;
	U4					u4LoopCnt = 0;
	I4					gi4TotalFileCount_back = 0;
	
	
	LOG_STR("Call");
	LOG_STR_RES("gi4TotalFolderCount = %ld gi4FolderLayerCount = %ld gau1CurrentPath = %s", gi4TotalFolderCount, gi4FolderLayerCount, gau1CurrentPath);
	LOG_STR("gu4CurrentPathSize = %ld", gu4CurrentPathSize);
	
	gi4TotalFileCount_back = gi4TotalFileCount;
//	LOG_STR_RES("### before gi4TotalFileCount_back = %ld",gi4TotalFileCount_back);
	
	errno = 0;
	pstDir = opendir((const char*)gau1CurrentPath);
	if (NULL == pstDir)
	{
		LOG_STR("opendir() return error(%d): %s", errno, strerror(errno));
		i4Result = D_NG;
	}
	else
	{
		while (u4LoopCnt < D_FILE_CNT_IN_FOLDER_MAX)
		{
			//check cancel or not  TODO
			
			errno = 0;
			pstDirEnt = readdir( pstDir );
			if (NULL == pstDirEnt && 0 != errno)
			{
				LOG_STR("readdir() return error(%d): %s", errno, strerror(errno));
			}
			
			//wait : sleep  TODO
			
			//abnormal check 
			if (NULL == pstDirEnt)
			{
				i4Result = ( 0 == errno ) ? D_OK : D_NG;
				LOG_STR("readdir() return pstDirEnt is NULL");
				break;
			}
			
			/*check type*/
			if (DT_REG != pstDirEnt->d_type)
			{
				LOG_STR("pstDirEnt->d_type is not DT_REG");
				continue;
			}
			
			u4LoopCnt++;
						//check of the max folder 
			if (D_FILE_CNT_MAX <= gi4TotalFileCount)
			{
				i4Result = D_NG;
				break;
			}
			
			/**check d-name ***/
			if ( NULL == pstDirEnt->d_name )
			{
				LOG_STR("pstDirEnt->d_name is NULL");
				continue;
			}

#if 0
			if ( 0 == strcmp( pstDirEnt->d_name, ".") )
			{
				LOG_STR("pstDirEnt->d_name is .");
				continue;
			}

			if ( 0 == strcmp( pstDirEnt->d_name, "..") )
			{
				LOG_STR("pstDirEnt->d_name is ..");
				continue;
			}
#endif
			U4 pu4ExtType = 0;
			memset(gau1CurrentFilePath,0,sizeof(gau1CurrentFilePath));
			memcpy(gau1CurrentFilePath,gau1CurrentPath,gi4TotalFolderCount);
			strcat(gau1CurrentFilePath,(const char*)pstDirEnt->d_name);
			
			//check extention to do 
			if (get_fileextensiontype(gau1CurrentFilePath,&pu4ExtType) == 0)
			{
				gi4FileID++;
				gi4TotalFileCount++;
				LOG_STR_RES("\t[File][gi4FileID = %ld][filetype=%ld][%s]",gi4FileID, pu4ExtType,(const char*)pstDirEnt->d_name);
				memset(gau1SQLBuffer,0,sizeof(gau1SQLBuffer));
				sprintf(gau1SQLBuffer,"insert into FILE(ID,NAME,PATHID) values(%ld,'%s',%ld)",gi4FileID,(const char*)pstDirEnt->d_name,gi4TotalFolderCount+1);
				//insert folder payh
				db_insertdata(gau1SQLBuffer);
			}

		}

		errno = 0;
		i4Result = closedir( pstDir );
		if ( 0 != i4Result )
		{
			LOG_STR("closedir() return error(%d): %s", errno, strerror(errno));
			i4Result = D_NG;
	 	}
	}
	
	//	LOG_STR_RES("### after  gi4TotalFileCount_back = %ld gi4TotalFileCount = %ld",gi4TotalFileCount_back, gi4TotalFileCount);
	if (gi4TotalFileCount_back == gi4TotalFileCount)
	{
		LOG_STR_RES("Empty Folder is %s",gau1CurrentPath);
	}
	else
	{
			gi4TotalFolderCount++;
			memset(gau1SQLBuffer,0,sizeof(gau1SQLBuffer));
			sprintf(gau1SQLBuffer,"insert into FOLDER(ID,NAME,PATHID) values(%ld,'%s',%ld)",gi4TotalFolderCount,dir_name,gi4TotalFolderCount);
			//insert folder payh
			db_insertdata(gau1SQLBuffer);
			sprintf(gau1SQLBuffer,"insert into FOLDERPATH values(%ld,'%s')",gi4TotalFolderCount,gau1CurrentPath);
			//insert folder payh
			db_insertdata(gau1SQLBuffer);		

	}
	
	return i4Result;
}


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
	    	"PATHID         INTEGER DEFAULT 0"\
			");";
	db_create_table(sql);
	return 0;
}


int	close_db()
{
	db_close();
	return 0;
}

U1* com_strtoupper(U1* pu1Str)
{
	U1 *pu1WorkStr;  
	if ( NULL == pu1Str )
	{
		return pu1Str;
	}
	for ( pu1WorkStr = pu1Str; *pu1WorkStr; pu1WorkStr++ )
	{
		*pu1WorkStr = toupper( *pu1WorkStr );  
	}        
	return ( pu1Str );       
}

I4 get_fileextensiontype( U1* pu1FileName, U4* pu4ExtType )
{
	U4		u4FileNameSize			= 0;
	U1*		pu1UpperExt			= NULL;
	I4		i4CmpRet			= -1;
	U1		au1Extention[ 4 ];

	if ( NULL == pu1FileName )
	{
		return -1;
	}
	if ( NULL == pu4ExtType )
	{
		return -1;
	}
	
	memset( (U1*)&au1Extention[ 0 ], 0, sizeof( au1Extention ) );

	u4FileNameSize = strlen( (char*)pu1FileName );
	strncpy( (char*)au1Extention , (char*)(pu1FileName + u4FileNameSize - D_FILE_EXT_LENGTH), D_FILE_EXT_LENGTH );

	au1Extention[ D_FILE_EXT_LENGTH ] = '\0';

	pu1UpperExt = com_strtoupper( au1Extention );
	
	i4CmpRet = strncmp( (char*)pu1UpperExt, ".MP3" , D_FILE_EXT_LENGTH );
	if ( 0 == i4CmpRet )
	{
		*pu4ExtType = D_FILE_EXTENTION_MP3;
	}
	else {
		i4CmpRet = strncmp( (char*)pu1UpperExt, ".WMA" , D_FILE_EXT_LENGTH );
		if ( 0 == i4CmpRet )
		{
			*pu4ExtType = D_FILE_EXTENTION_WMA;
		}
		else
		{
			i4CmpRet = strncmp( (char*)pu1UpperExt, ".AAC" , D_FILE_EXT_LENGTH );
			if ( 0 == i4CmpRet )
			{
				*pu4ExtType = D_FILE_EXTENTION_AAC;
			}
			else
			{
				i4CmpRet = strncmp( (char*)pu1UpperExt, ".M4A" , D_FILE_EXT_LENGTH );
				if ( 0 == i4CmpRet )
				{
					*pu4ExtType = D_FILE_EXTENTION_M4A;
				}
				else
				{
					*pu4ExtType = D_FILE_EXTENTION_OTHER;
					return -1;
				}			
				

			}
		}
	}

	return 0;
}




int main(int argc,char *argv[])  
{  	
	start_indexing();
	int_db();
	strcpy(gau1CurrentPath, argv[1]);
	gu4CurrentPathSize = strlen(argv[1]);
	indexing_scan_folder("Root");
	close_db();	
	return 0;
} 

