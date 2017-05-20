#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

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
static U4	gu4CurrentPathSize;
static I4	gi4FileID;
static I4	gi4FolderID;
static I4	gi4FolderIDUpdateFlg;
static I4	gi4TotalFileCount;
static I4	gi4FolderLayerCount;
static I4	gi4TotalFolderCount;


static I4 indexing_scan_files();

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

I4 indexing_scan_folder()
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
	indexing_scan_files();
	
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
			
			gi4TotalFolderCount++;
			strncat( (char*)gau1CurrentPath, "/", 1);	
			strncat( (char*)gau1CurrentPath, (const char*)pstDirEnt->d_name, u4FolderSize);	
			gu4CurrentPathSize += (u4FolderSize + 1 );

			gi4FolderLayerCount++;
			indexing_scan_folder();
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

I4 indexing_scan_files()
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

			//check extention to do 
			
			gi4FileID++;
			gi4TotalFileCount++;
			//out put file name
			LOG_STR_RES("\t\t[File][gi4FileID = %ld][%s]",gi4FileID, (const char*)pstDirEnt->d_name);
			
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
	//	LOG_STR_RES("Empty Folder is %s",gau1CurrentPath);
	}
	
	return i4Result;
}




int main(int argc,char *argv[])  
{  	
	start_indexing();
	strcpy(gau1CurrentPath, argv[1]);
	gu4CurrentPathSize = strlen(argv[1]);
	indexing_scan_folder();
	
	return 0;
} 

