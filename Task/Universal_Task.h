#ifndef _UNIVERSAL_T_H__
#define _UNIVERSAL_T_H__

#include "Define.h"

#pragma pack(1)
typedef struct SD_TestStruct_Tag
{
	unsigned char testBuf[4096];
	unsigned short crc;
}SD_TestStruct;
#pragma pack()

#define	SD_TestStructSize		sizeof(SD_TestStruct)
#define	SD_TestStructCRCSize	sizeof(SD_TestStruct)-2

char StartvUniversalTask(void);
MyRes sd_test(unsigned int count);

#endif




