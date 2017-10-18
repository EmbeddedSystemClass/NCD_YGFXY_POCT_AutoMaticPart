/****************************************file start****************************************************/
#ifndef SYSTEMSET_D_H
#define	SYSTEMSET_D_H

#include	"Define.h"
#include	"Device.h"
#include	"NetWork.h"

#pragma pack(1)
typedef struct SystemSetData_Tag
{		
	char deviceId[DeviceIdLen];											//�豸��Ϣ
	WireNetSet wireNetSet;												//����������
	ServerSet serverSet;												//����������
	bool isAutoPrint;													//�Ƿ��Զ���ӡ
	unsigned short ledSleepTime;										//����ʱ�� s
	unsigned char ledLightIntensity;									//��Ļ���� 0-100
	unsigned short testLedLightIntensity;								//����ʱ���õ�led����ֵ��mv
	unsigned char parm1[512];											//Ԥ��512�ֽڵĲ���λ��
	unsigned short crc;
}SystemSetData;
#pragma pack()

#define	SystemSetDataStructSize		sizeof(SystemSetData)
#define SystemSetDataStructCrcSize	SystemSetDataStructSize - 2

void setDefaultSystemSetData(SystemSetData * systemSetData);
SystemSetData * getGBSystemSetData(void);
void readGbSystemSetData(SystemSetData * systemSetData);
void upDateSystemSetData(SystemSetData * systemSetData);
void setIsShowRealValue(bool isShow);
bool IsShowRealValue(void);
void readSystemDeviceId(char * device);
#endif

/****************************************end of file************************************************/
