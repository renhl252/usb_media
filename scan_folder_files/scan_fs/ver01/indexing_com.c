
#include <string.h>

#include "code.h"
#include "log.h"

#define D_FILE_EXT_LENGTH 4

typedef enum _EN_FILE_TYPE
{
	D_FILE_EXTENTION_MP3 = 1,
	D_FILE_EXTENTION_AAC,
	D_FILE_EXTENTION_M4A,
	D_FILE_EXTENTION_M4B,
	D_FILE_EXTENTION_WAV,
	D_FILE_EXTENTION_WMA,
	D_FILE_EXTENTION_OGG,
	D_FILE_EXTENTION_FLAC,
	D_FILE_EXTENTION_OTHER
}EN_FILE_TYPE;


static U1* com_strtoupper(U1* pu1Str)
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
	U1		au1Extention[5] = {0};

	if (NULL == pu1FileName)
	{
	    return -1;
	}
	if (NULL == pu4ExtType)
	{
	    return -1;
	}

	u4FileNameSize = strlen( (char*)pu1FileName );
	if (u4FileNameSize <= 4)
	{
	    return -1;
	}

	strncpy( (char*)au1Extention , (char*)(pu1FileName + u4FileNameSize - D_FILE_EXT_LENGTH), D_FILE_EXT_LENGTH);

	pu1UpperExt = com_strtoupper(au1Extention);

        if (strncmp((char*)pu1UpperExt, ".MP3" ,D_FILE_EXT_LENGTH) == 0)
	{
		*pu4ExtType = D_FILE_EXTENTION_MP3;
	}
	else if (strncmp((char*)pu1UpperExt, ".AAC" ,D_FILE_EXT_LENGTH) == 0)
	{
		*pu4ExtType = D_FILE_EXTENTION_AAC;
	}
	else if (strncmp((char*)pu1UpperExt, ".M4A" ,D_FILE_EXT_LENGTH) == 0)
	{
		*pu4ExtType = D_FILE_EXTENTION_M4A;
	}
	else if (strncmp((char*)pu1UpperExt, ".M4B" ,D_FILE_EXT_LENGTH) == 0)
	{
		*pu4ExtType = D_FILE_EXTENTION_M4B;
	}
	else if (strncmp((char*)pu1UpperExt, ".WAV" ,D_FILE_EXT_LENGTH) == 0)
	{
		*pu4ExtType = D_FILE_EXTENTION_WAV;
	}
	else if (strncmp((char*)pu1UpperExt, ".WMA" ,D_FILE_EXT_LENGTH) == 0)
	{
		*pu4ExtType = D_FILE_EXTENTION_WMA;
	}
	else if (strncmp((char*)pu1UpperExt, ".OGG" ,D_FILE_EXT_LENGTH) == 0)
	{
		*pu4ExtType = D_FILE_EXTENTION_OGG;
	}
	else if (strncmp((char*)pu1UpperExt, "FLAC" ,D_FILE_EXT_LENGTH) == 0)
	{
		if (u4FileNameSize <= 5|| pu1FileName[u4FileNameSize-5] != '.')
		{
			*pu4ExtType = D_FILE_EXTENTION_OTHER;
			return -1;		
		}
		*pu4ExtType = D_FILE_EXTENTION_FLAC;
	}
	else
	{
		*pu4ExtType = D_FILE_EXTENTION_OTHER;
		return -1;
	}
	return 0;
}
