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

#define	DeviceAdjustStructSize		sizeof(DeviceAdjust)							//��ౣ����û���Ŀ
#define	DeviceAdjustStructCrcSize	DeviceAdjustStructSize - 2						//��ౣ����û���Ŀ

#endif

/****************************************end of file************************************************/
