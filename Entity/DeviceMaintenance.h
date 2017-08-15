/****************************************file start****************************************************/
#ifndef _DEVICEMAINTENANCE_E_H
#define	_DEVICEMAINTENANCE_E_H

#include	"DateTime.h"
#include	"Operator.h"
#include	"Define.h"

#define		DeviceMaintenanceRecordPageShowNum	8

#pragma pack(1)
typedef struct
{
	DateTime dateTime;
	Operator operator;
	bool isOk;
	char desc[50];
	unsigned short crc;
}DeviceMaintenance;
#pragma pack()

#pragma pack(1)
typedef struct
{
	PageRequest pageRequest;
	DeviceRecordHeader deviceRecordHeader;
	DeviceMaintenance deviceMaintenance[DeviceMaintenanceRecordPageShowNum];
	unsigned char readTotalNum;							//��ȡ������Ч������Ŀ
}DeviceMaintenanceReadPackge;
#pragma pack()

#define	DeviceMaintenanceStructSize		sizeof(DeviceMaintenance)							//��ౣ����û���Ŀ
#define	DeviceMaintenanceStructCrcSize	DeviceMaintenanceStructSize - 2						//��ౣ����û���Ŀ



#define	DeviceMaintenanceReadPackgeStructSize		sizeof(DeviceMaintenanceReadPackge)								//��ౣ����û���Ŀ

#endif

/****************************************end of file************************************************/
