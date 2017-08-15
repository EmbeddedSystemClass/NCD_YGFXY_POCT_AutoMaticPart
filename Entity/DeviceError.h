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

#pragma pack(1)
typedef struct
{
	PageRequest pageRequest;
	DeviceRecordHeader deviceRecordHeader;
	DeviceError deviceError[DeviceErrorRecordPageShowNum];
	unsigned char readTotalNum;							//��ȡ������Ч������Ŀ
}DeviceErrorReadPackge;
#pragma pack()

#define	DeviceErrorStructSize		sizeof(DeviceError)							//��ౣ����û���Ŀ
#define	DeviceErrorStructCrcSize	DeviceErrorStructSize - 2						//��ౣ����û���Ŀ



#define	DeviceErrorReadPackgeStructSize		sizeof(DeviceErrorReadPackge)								//��ౣ����û���Ŀ

#endif

/****************************************end of file************************************************/
