#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include <errno.h>

#include "code.h"
#include "log.h"

#define	 D_OK          						(0) 
#define	 D_NG          						(-1)

#define D_FILE_CNT_MAX						(5000)
#define D_FOLDER_CNT_MAX					(5000)
#define D_FILE_CNT_IN_FOLDER_MAX				(1024)
#define D_FOLDER_CNT_IN_FOLDER_MAX				(1024)
#define	D_FOLDER_LAYER_MAX_SIZE					(8)

#define	D_FILE_LENGTH_MAX           			( 256 + 64 ) 
#define	D_PATH_MAX_SIZE					( ((D_FILE_LENGTH_MAX + 1)/4 + 1)*D_FOLDER_LAYER_MAX_SIZE )


static U1	gau1CurrentPath[ D_PATH_MAX_SIZE + 4 ];
static U4	gu4CurrentPathSize;

static U1	gau1CurrentFilePath[ D_PATH_MAX_SIZE + 4 + 255 ];


static I4	gi4FileID;
static I4	gi4FolderID;
static I4	gi4TotalFileCount;
static I4	gi4TotalFolderCount;
static I4	gi4FolderLayerCount;

static I4       indexing_scan_files();
extern I4       get_fileextensiontype( U1* pu1FileName, U4* pu4ExtType );


static I4 start_indexing()
{
	memset(gau1CurrentPath, 0, sizeof(gau1CurrentPath));
	gu4CurrentPathSize = 0;
	
	gi4FileID = 0;
	gi4FolderID = 0;
	
	gi4TotalFileCount = 0;
	gi4FolderLayerCount = 0;
	gi4TotalFolderCount = 0;
	
	return 0;
}

static I4 end_indexing()
{
	return 0;
}

static I4 indexing_scan_folder(const char *dirname)
{
	U4 		u4FolderSize = 0;
	DIR*		pstDir;
	struct dirent*	pstDirEnt;
	I4		i4Result = D_OK;
	U4		u4LoopCnt = 0;
	I4		i4TotalFileCount_back = 0;

	//check of the max folder 
	if (D_FOLDER_CNT_MAX <= gi4TotalFolderCount)
	{
		return i4Result;
	}

	if (D_FILE_CNT_MAX <= gi4TotalFileCount)
	{
	  	return i4Result;
	}
	i4TotalFileCount_back = gi4TotalFileCount;

	indexing_scan_files();

	if (i4TotalFileCount_back == gi4TotalFileCount)
	{
		//empty folder
	}
	else
	{
		gi4TotalFolderCount++;
		gi4FolderID++;
		
		LOG_STR_RES("\tPath  = [%s]", gau1CurrentPath);
		LOG_STR_RES("\tFolder= [%ld] %s", gi4FolderID,dirname);
		LOG_STR_RES("=================================================================");
		if (D_FOLDER_CNT_MAX <= gi4TotalFolderCount)
		{
			LOG_STR_RES("@ max FolderCount: %ld", gi4TotalFolderCount);
			return i4Result;
		}
	}
	
	//check of the max folder depth 
	if (gi4FolderLayerCount >= D_FOLDER_LAYER_MAX_SIZE)
	{
		LOG_STR_RES("@ max layer: i4FolderLayerCount = %ld, Path  = [%s]", gi4FolderLayerCount,gau1CurrentPath);
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
			errno = 0;
			pstDirEnt = readdir( pstDir );	
			if (NULL == pstDirEnt)
			{
				i4Result = D_OK;
				break;
			}
			
			if (DT_DIR != pstDirEnt->d_type)
			{
				continue;
			}
			
#if 0			
			if ( NULL == pstDirEnt->d_name )
			{
				continue;
			}
#endif

			if ( 0 == strcmp( pstDirEnt->d_name, ".") )
			{
				continue;
			}
			if ( 0 == strcmp( pstDirEnt->d_name, "..") )
			{
				continue;
			}

			u4LoopCnt++;
			u4FolderSize = strlen((const char*)pstDirEnt->d_name);
			strncat( (char*)gau1CurrentPath, "/", 1);	
			strncat( (char*)gau1CurrentPath, (const char*)pstDirEnt->d_name, u4FolderSize);	
			gu4CurrentPathSize += (u4FolderSize + 1 );
			gau1CurrentPath[gu4CurrentPathSize] = 0;

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

static I4 indexing_scan_files()
{
	DIR*		pstDir;
	struct dirent*	pstDirEnt;
	I4		i4Result = D_OK;
	U4		u4LoopCnt = 0;

	if (D_FILE_CNT_MAX <= gi4TotalFileCount)
	{
		LOG_STR("!!! not need search file, because of gi4TotalFileCount is (%ld)", gi4TotalFileCount);
		i4Result = D_OK;
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
		while (u4LoopCnt < D_FILE_CNT_IN_FOLDER_MAX)
		{	
			errno = 0;
			pstDirEnt = readdir( pstDir );
			if (NULL == pstDirEnt)
			{
				i4Result = D_OK;
				break;
			}

			/*check type*/
			if (DT_REG != pstDirEnt->d_type)
			{
				continue;
			}
					
			if ( NULL == pstDirEnt->d_name )
			{
				LOG_STR("pstDirEnt->d_name is NULL");
				continue;
			}

			u4LoopCnt++;

			U4 pu4ExtType = 0;
			if (get_fileextensiontype((U1*)pstDirEnt->d_name,&pu4ExtType) == 0)
			{
				gi4FileID++;
				gi4TotalFileCount++;
				//LOG_STR_RES(" %s", gau1CurrentPath);
				LOG_STR_RES("File [%ld]%s", gi4FileID, (const char*)pstDirEnt->d_name);
				if (D_FILE_CNT_MAX <= gi4TotalFileCount)
				{
					i4Result = D_OK;
					break;
				}
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
	
	return i4Result;
}

int scan_folder_files(char * path)  
{
	start_indexing();

	strcpy(gau1CurrentPath, path);
	gu4CurrentPathSize = strlen(path);
	gi4FolderLayerCount++;
	indexing_scan_folder("Root");
	gi4FolderLayerCount--;
	
	end_indexing();
	return 0;
} 


