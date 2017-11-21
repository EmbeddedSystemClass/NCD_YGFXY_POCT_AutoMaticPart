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
	char deviceId[DeviceIdLen];											//�豸ID,����Ϊ�豸id�ĸ���
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
#endif

/****************************************end of file************************************************/

