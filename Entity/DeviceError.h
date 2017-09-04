/****************************************file start****************************************************/
#ifndef _DEVICEERROR_E_H
#define	_DEVICEERROR_E_H

#include	"Define.h"
#include	"DateTime.h"
#include	"Operator.h"

#define		DeviceErrorRecordPageShowNum	8

#pragma pack(1)
typedef struct
{
	unsigned short errorCode;
	DateTime dateTime;
	Operator operator;
	char result[30];
	unsigned short crc;
}DeviceError;
#pragma pack()

#define	DeviceErrorStructSize		sizeof(DeviceError)							//最多保存的用户数目
#define	DeviceErrorStructCrcSize	DeviceErrorStructSize - 2						//最多保存的用户数目

#endif

/****************************************end of file************************************************/
