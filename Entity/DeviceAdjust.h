/****************************************file start****************************************************/
#ifndef _DEVICEADJUST_E_H
#define	_DEVICEADJUST_E_H

#include	"DateTime.h"
#include	"Operator.h"
#include	"Define.h"

#define		DeviceAdjustRecordPageShowNum	8

#pragma pack(1)
typedef struct
{
	float normalv;
	float measurev;
	DateTime dateTime;
	Operator operator;
	char result[20];
	unsigned short crc;
}DeviceAdjust;
#pragma pack()

#pragma pack(1)
typedef struct
{
	PageRequest pageRequest;
	DeviceRecordHeader deviceRecordHeader;
	DeviceAdjust deviceAdjust[DeviceAdjustRecordPageShowNum];
	unsigned char readTotalNum;							//读取到的有效数据数目
}DeviceAdjustReadPackge;
#pragma pack()

#define	DeviceAdjustStructSize		sizeof(DeviceAdjust)							//最多保存的用户数目
#define	DeviceAdjustStructCrcSize	DeviceAdjustStructSize - 2						//最多保存的用户数目



#define	DeviceAdjustReadPackgeStructSize		sizeof(DeviceAdjustReadPackge)								//最多保存的用户数目

#endif

/****************************************end of file************************************************/
