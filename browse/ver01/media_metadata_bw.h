#ifndef _MEDIA_METADATA_BW_H_
#define _MEDIA_METADATA_BW_H_

#define METADATATYPE_MAX_TYPES             10

typedef unsigned int UINT32;

typedef signed int INT32;
typedef UINT32 RetType;
typedef char CHAR;

#define D_SUCCESS                                                  0x0000

#define D_DEFAULT_CATEGORY_OFFSET 0xFFFFFFFF

typedef enum {
    E_METADATA_DEVICE = 0x0,
	E_METADATA_GENRE = 0x1,
    E_METADATA_ARTIST = 0x2,
    E_METADATA_ALBUM = 0x3,
    E_METADATA_SONG = 0x4,
    E_METADATA_CATEGORY_NOT_IN_USE = 0x800
}EN_MetadataCategory;

RetType get_metadata_category_info(
    EN_MetadataCategory eBaseCategory,
    UINT32 uiBaseCategoryOffset,
    EN_MetadataCategory eSubCategory,
    UINT32 uiSubCategoryOffset,
    EN_MetadataCategory eLowerSubCategory,
    UINT32 uiLowerSubCategoryOffset,
    EN_MetadataCategory eLowestSubCategory,
    UINT32 uiLowestSubCategoryOffset,
    EN_MetadataCategory eSpecificCategory);

#endif