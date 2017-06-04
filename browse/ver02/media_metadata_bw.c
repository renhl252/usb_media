#include <string.h>
#include "log.h"
#include "media_metadata_bw.h"
#include "db_operation.h"

//
char * au1IDS[] ={
"",
"GENREID",
"ARTISTID",
"ALBUMID",
"ID"
};

char * au1TableNames[] ={
"",
"GENRE",
"ARTIST",
"ALBUM",
"AUDIO"
};

char * au1ItemNames[] ={
"",
"NAME",
"NAME",
"NAME",
"TITLE"
};

typedef struct _S_CONVERTID_RES
{
	UINT32 indexofIDS;//0 default
	UINT32 u32SelID;  //0 default
}S_CONVERTID_RES;

S_CONVERTID_RES astConvetIDRes[4];
UINT32 u32NumofConvertIDRes;

CHAR au1SQL[1024];
CHAR au1TmpSQL[1024];


static void convertID(
    EN_MetadataCategory eBaseCategory,
    UINT32 uiBaseCategoryOffset,
    EN_MetadataCategory eSubCategory,
    UINT32 uiSubCategoryOffset,
    EN_MetadataCategory eLowerSubCategory,
    UINT32 uiLowerSubCategoryOffset,
    EN_MetadataCategory eLowestSubCategory,
    UINT32 uiLowestSubCategoryOffset
   );

static void getConvertIDSQL(EN_MetadataCategory eCategory, UINT32 uiCategoryOffset);
static UINT32 getListItemCountSQL(EN_MetadataCategory eCategory);
static void getListUpdateSQL(EN_MetadataCategory eCategory, UINT32 uiCategoryOffset, UINT32 uiCategoryCount);

RetType get_metadata_category_info(
    EN_MetadataCategory eBaseCategory,
    UINT32 uiBaseCategoryOffset,
    EN_MetadataCategory eSubCategory,
    UINT32 uiSubCategoryOffset,
    EN_MetadataCategory eLowerSubCategory,
    UINT32 uiLowerSubCategoryOffset,
    EN_MetadataCategory eLowestSubCategory,
    UINT32 uiLowestSubCategoryOffset,
    EN_MetadataCategory eSpecificCategory)
{
	LOG_STR("===call===");
	LOG_STR("eBaseCategory = %d", eBaseCategory);
	LOG_STR("eSubCategory = %d", eSubCategory);
	LOG_STR("eLowerSubCategory = %d", eLowerSubCategory);
	LOG_STR("eLowestSubCategory = %d", eLowestSubCategory);
	LOG_STR("");
	LOG_STR("uiBaseCategoryOffset = %d", uiBaseCategoryOffset);
	LOG_STR("uiSubCategoryOffset = %d", uiSubCategoryOffset);
	LOG_STR("uiLowerSubCategoryOffset = %d", uiLowerSubCategoryOffset);
	LOG_STR("uiLowestSubCategoryOffset = %d", uiLowestSubCategoryOffset);
	LOG_STR("");

	memset(&astConvetIDRes, 0, sizeof(astConvetIDRes));
	u32NumofConvertIDRes = 0;
	//convert ID from filter index 
	LOG_STR("===convertID===");
	convertID(
	    eBaseCategory,
	    uiBaseCategoryOffset,
	    eSubCategory,
	    uiSubCategoryOffset,
	    eLowerSubCategory,
	    uiLowerSubCategoryOffset,
	    eLowestSubCategory,
	    uiLowestSubCategoryOffset
	);
	LOG_STR("===getListItemCountSQL===");
	UINT32 u32Count = getListItemCountSQL(eSpecificCategory);
	LOG_STR("===getListUpdateSQL===");
	getListUpdateSQL(eSpecificCategory,1,u32Count);
	
	return D_SUCCESS;
}

void convertID(
    EN_MetadataCategory eBaseCategory,
    UINT32 uiBaseCategoryOffset,
    EN_MetadataCategory eSubCategory,
    UINT32 uiSubCategoryOffset,
    EN_MetadataCategory eLowerSubCategory,
    UINT32 uiLowerSubCategoryOffset,
    EN_MetadataCategory eLowestSubCategory,
    UINT32 uiLowestSubCategoryOffset
   )
{
	if (eBaseCategory == E_METADATA_DEVICE
		|| eBaseCategory == E_METADATA_CATEGORY_NOT_IN_USE)
	{
		//LOG_STR("===not need convert ===");
		return ;
	}
	getConvertIDSQL(eBaseCategory, uiBaseCategoryOffset);
	
	if (eSubCategory == E_METADATA_CATEGORY_NOT_IN_USE)
	{
		//LOG_STR("=== convert eSubCategory end ===");
		return ;
	}
	getConvertIDSQL(eSubCategory, uiSubCategoryOffset);
	
	if (eLowerSubCategory == E_METADATA_CATEGORY_NOT_IN_USE)
	{
		//LOG_STR("=== convert eLowerSubCategory end ===");
		return ;
	}
	getConvertIDSQL(eLowerSubCategory, uiLowerSubCategoryOffset);

	if (eLowestSubCategory == E_METADATA_CATEGORY_NOT_IN_USE)
	{
		//LOG_STR("=== convert eLowestSubCategory end ===");
		return ;
	}
	getConvertIDSQL(eLowestSubCategory, uiLowestSubCategoryOffset);


}
UINT32 getCommonSQL(EN_MetadataCategory eCategory)
{
	UINT32 indexofIDS = 0;
	switch(eCategory)
	{
		case E_METADATA_GENRE:
			indexofIDS = 1;
		break;
		case E_METADATA_ARTIST:
			indexofIDS = 2;
		break;
		case E_METADATA_ALBUM:
			indexofIDS = 3;
		break;
		case E_METADATA_SONG:
			indexofIDS = 4;
		break;
		default:
		break;
	}
	
	if (0 == indexofIDS)
	{
		return indexofIDS;
	}
	
	//LOG_STR("indexofIDS = %d", indexofIDS);
	//select ID from ARTIST where ID in (select distinct ARTISTID from AUDIO where Genreid=11) order by sortid limit 0,1
	sprintf(au1TmpSQL," \"%s\"", au1TableNames[indexofIDS]);
	strcat(au1SQL,au1TmpSQL);
	
	UINT32 index;
	for(index = 0; index <u32NumofConvertIDRes; index++)
	{
		memset(au1TmpSQL,0,sizeof(au1TmpSQL));
		if (index == 0)
		{
			sprintf(au1TmpSQL," where ID in (select distinct \"%s\" from AUDIO where \"%s\"=%d", au1IDS[indexofIDS], au1IDS[astConvetIDRes[index].indexofIDS],
				astConvetIDRes[index].u32SelID);
		}
		else
		{
			sprintf(au1TmpSQL," and \"%s\"=%d ", au1IDS[astConvetIDRes[index].indexofIDS], astConvetIDRes[index].u32SelID);
		}
		strcat(au1SQL,au1TmpSQL);
	}
	
	if (index != 0)
	{
		strcat(au1SQL,") ");
	}
	
	return indexofIDS;
}


void getConvertIDSQL(EN_MetadataCategory eCategory, UINT32 uiCategoryOffset)
{
	memset(au1SQL,0,sizeof(au1SQL));
	memset(au1SQL,0,sizeof(au1SQL));
	strcpy(au1SQL,"select ID from");
	//select ID from ARTIST where ID in (select distinct ARTISTID from AUDIO where Genreid=11) order by sortid limit 0,1
	UINT32 indexofIDS = getCommonSQL(eCategory);
	sprintf(au1TmpSQL," order by sortid limit %d,1", uiCategoryOffset - 1);
	strcat(au1SQL,au1TmpSQL);
	
	LOG_STR("au1SQL = %s", au1SQL);
	
	UINT32 u32ID = get_db_count(au1SQL);

	astConvetIDRes[u32NumofConvertIDRes].indexofIDS=indexofIDS;
	astConvetIDRes[u32NumofConvertIDRes].u32SelID=u32ID;
	u32NumofConvertIDRes++;	
}

UINT32 getListItemCountSQL(EN_MetadataCategory eCategory)
{
    //select count(ID) from ALBUM where ID in (select distinct ALBUMID from AUDIO where Genreid=11 and ARTISTID=15)
	memset(au1SQL,0,sizeof(au1SQL));
	memset(au1SQL,0,sizeof(au1SQL));
	strcpy(au1SQL,"select count(ID) from");
	getCommonSQL(eCategory);
	LOG_STR("au1SQL = %s", au1SQL);
	UINT32 u32Count = get_db_count(au1SQL);
	LOG_STR("u32Count = %d", u32Count);
	
	return u32Count;

}

void getListUpdateSQL(EN_MetadataCategory eCategory, UINT32 uiCategoryOffset, UINT32 uiCategoryCount)
{
    //select NAME from ALBUM where ID in (select distinct ALBUMID from AUDIO where Genreid=11 and ARTISTID=15) order by sortid  limit start ,count
	memset(au1SQL,0,sizeof(au1SQL));
	UINT32 indexofIDS = getCommonSQL(eCategory);
	sprintf(au1TmpSQL, "select \"%s\" from ", au1ItemNames[indexofIDS]);
	strcat(au1TmpSQL,au1SQL);
	sprintf(au1SQL,"%s",au1TmpSQL);
	
	sprintf(au1TmpSQL," order by sortid limit %d,%d", uiCategoryOffset - 1,uiCategoryCount);
	strcat(au1SQL,au1TmpSQL);
	
	
	LOG_STR("au1SQL = %s", au1SQL);
	get_item_text(au1SQL);

}
