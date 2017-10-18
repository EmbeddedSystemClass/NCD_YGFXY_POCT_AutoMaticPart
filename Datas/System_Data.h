/****************************************file start****************************************************/
#ifndef	SYSTEM_D_H
#define	SYSTEM_D_H

#include	"DateTime.h"
#include	"NetWork.h"

#pragma pack(1)
typedef struct
{
	DateTime systemDateTime;
	WireNetInfo wireNetInfo;
	bool deviceInfoIsNew;								//设备信息是否有更新。是否需要上传设备信息到纽康度服务器的标识
	bool timeIsReaded;									//一台设备一次开机只读取一次时间，用于校准设备的时间
	bool isUsartMotorOnline;							//串口夹爪是否在线，否则使用io控制的夹爪
}SystemData;
#pragma pack()


const SystemData * getSystemRunTimeData(void);
void setSystemTime(DateTime * dateTime);
DateTime * getSystemTime(DateTime * dateTime);
void setSystemWireIP(unsigned int ip);
void setSystemWireMac(unsigned char * mac);
void setSystemWireLinkStatus(LinkStatus linkStatus);
void setSystemDeviceInfoStatus(bool isNew);
bool getSystemDeviceInfoStatus(void);
void setSystemTimeIsRead(bool isNew);
bool getSystemTimeIsRead(void);
#endif

/****************************************end of file************************************************/

