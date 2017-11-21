/****************************************file start****************************************************/
#ifndef _DEVICE_E_H
#define	_DEVICE_E_H

#include	"Operator.h"

#define	DeviceIdLen	20
#define	DeviceAddrLen	50

#pragma pack(1)
typedef struct Device_Tag
{
	char deviceId[DeviceIdLen];													//设备ID
	Operator operator;															//责任人
	char addr[DeviceAddrLen];													//设备地址
	unsigned short crc;
}Device;
#pragma pack()

#define	DeviceStructSize		sizeof(Device)
#define	DeviceStructCrcSize		DeviceStructSize - 2
	
#endif

/****************************************end of file************************************************/

