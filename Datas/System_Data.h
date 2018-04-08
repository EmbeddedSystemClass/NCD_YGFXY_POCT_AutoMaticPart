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
	bool deviceIsNew;													//�豸��Ϣ�Ƿ��Ѹ���
	char deviceId[DeviceIdLen];											//�豸ID,����Ϊ�豸id�ĸ���
	char deviceAdd[DeviceAddrLen];										//�豸��ַ����
	unsigned short testLedValue;										//����LED DAֵ
	unsigned short baseLineValue;										//����DAֵ
	unsigned short systemWorkStatus;									//ϵͳ����״̬
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
void setSystemDeviceIsNew(bool status);
bool systemDeviceIsNew(void);
void setSystemDeviceId(char * device);
void getSystemDeviceId(char * device);
void getSystemDeviceAddr(char * device);
void setSystemDeviceAddr(char * device);
void setTestLedValue(unsigned short value);
void setBaseLineValue(unsigned short value);
void updateSystemWorkStatus(unsigned short workStatus, MyBitAction action);
unsigned short readSystemWorkStatus(void);
#endif

/****************************************end of file************************************************/

