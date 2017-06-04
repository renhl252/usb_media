#include <string.h>
#include "media_metadata_bw.h"
#include "log.h"
#include "db_operation.h"

void print()
{
	LOG_STR("=== device(0) Genre(1) Atist(2) Album(3) Song(4)  ====");
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
		print();
		return -1;
    }	
	int_db();
    int index = 0;
    for(index = 0; index < argc; index++)
    {
		LOG_STR(" argv[%d] = %s", index, argv[index]);
    }
    
	EN_MetadataCategory eSpecificCategory = atoi(argv[1]);
	EN_MetadataCategory eBaseCategory = E_METADATA_DEVICE;
    UINT32 uiBaseCategoryOffset = D_DEFAULT_CATEGORY_OFFSET;
    EN_MetadataCategory eSubCategory = E_METADATA_CATEGORY_NOT_IN_USE;
    UINT32 uiSubCategoryOffset = D_DEFAULT_CATEGORY_OFFSET;
    EN_MetadataCategory eLowerSubCategory = E_METADATA_CATEGORY_NOT_IN_USE;
    UINT32 uiLowerSubCategoryOffset =D_DEFAULT_CATEGORY_OFFSET;
    EN_MetadataCategory eLowestSubCategory = E_METADATA_CATEGORY_NOT_IN_USE;
    UINT32 uiLowestSubCategoryOffset = D_DEFAULT_CATEGORY_OFFSET;
	if (argc >= 3)
	{
		eBaseCategory = atoi(argv[2]);
	}
	if (argc >= 4)
	{
		uiBaseCategoryOffset = atoi(argv[3]);
	}

	if (argc >= 5)
	{
		eSubCategory = atoi(argv[4]);
	}
	if (argc >= 6)
	{
		uiSubCategoryOffset = atoi(argv[5]);
	}

	if (argc >= 7)
	{
		eLowerSubCategory = atoi(argv[6]);
	}
	if (argc >= 8)
	{
		uiLowerSubCategoryOffset = atoi(argv[7]);
	}
	

	RetType ret = D_SUCCESS;
	
	ret = get_metadata_category_info(
	eBaseCategory,
	uiBaseCategoryOffset,

	eSubCategory,
	uiSubCategoryOffset,

	eLowerSubCategory,
	uiLowerSubCategoryOffset,

	eLowestSubCategory,
	uiLowestSubCategoryOffset,
	
	eSpecificCategory
	);
	close_db();
	return 0;
}