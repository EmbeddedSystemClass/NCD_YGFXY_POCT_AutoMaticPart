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
	bool deviceInfoIsNew;								//�豸��Ϣ�Ƿ��и��¡��Ƿ���Ҫ�ϴ��豸��Ϣ��Ŧ���ȷ������ı�ʶ
	bool timeIsReaded;									//һ̨�豸һ�ο���ֻ��ȡһ��ʱ�䣬����У׼�豸��ʱ��
	bool isUsartMotorOnline;							//���ڼ�צ�Ƿ����ߣ�����ʹ��io���Ƶļ�צ
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

