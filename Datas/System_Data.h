/****************************************file start****************************************************/
#ifndef	SYSTEM_D_H
#define	SYSTEM_D_H

#include	"DateTime.h"
#include	"NetWork.h"
#include	"Device.h"

#pragma pack(1)
typedef struct
{
	DateTime systemDateTime;
	WireNetInfo wireNetInfo;
	char deviceId[DeviceIdLen];											//设备ID,仅作为设备id的副本
	unsigned short testLedValue;										//发光LED DA值
	unsigned short baseLineValue;										//基线DA值
}SystemData;
#pragma pack()


const SystemData * getSystemRunTimeData(void);
void setSystemTime(DateTime * dateTime);
DateTime * getSystemTime(DateTime * dateTime);
void setSystemWireIP(unsigned int ip);
void setSystemWireMac(unsigned char * mac);
void setSystemWireLinkStatus(LinkStatus linkStatus);
void setSystemTimeIsRead(bool isNew);
bool getSystemTimeIsRead(void);
void setSystemDeviceId(char * device);
void getSystemDeviceId(char * device);
void setTestLedValue(unsigned short value);
void setBaseLineValue(unsigned short value);
#endif

/****************************************end of file************************************************/

