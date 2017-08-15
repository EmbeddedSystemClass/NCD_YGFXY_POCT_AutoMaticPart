/****************************************file start****************************************************/
#ifndef _DEVICEQUALITY_E_H
#define	_DEVICEQUALITY_E_H

#include	"DateTime.h"
#include	"Operator.h"
#include	"Define.h"
#include	"QRCode.h"
#include	"TestData.h"

#define		DeviceQualityRecordPageShowNum	8
#define		DeviceQualityMaxTestCount		10

#pragma pack(1)
typedef struct
{
	DateTime dateTime;												//�ʿ�ʱ��
	Operator operator;												//�ʿ���
	char itemName[ItemNameLen];										//�ʿ���Ŀ����
	double standardValue;									//�ʿر�׼ֵ
	float testValue[DeviceQualityMaxTestCount];						//�ʿز���ֵ
	bool isOk;														//�ʿؽ��
	char desc[50];													//�ʿ�˵��
	unsigned short crc;
}DeviceQuality;
#pragma pack()

#pragma pack(1)
typedef struct
{
	PageRequest pageRequest;
	DeviceRecordHeader deviceRecordHeader;
	DeviceQuality deviceQuality[DeviceQualityRecordPageShowNum];
	unsigned char readTotalNum;							//��ȡ������Ч������Ŀ
}DeviceQualityReadPackge;
#pragma pack()

#define	DeviceQualityStructSize		sizeof(DeviceQuality)							//��ౣ����û���Ŀ
#define	DeviceQualityStructCrcSize	DeviceQualityStructSize - 2						//��ౣ����û���Ŀ


#define	DeviceQualityReadPackgeStructSize		sizeof(DeviceQualityReadPackge)								//��ౣ����û���Ŀ

#endif

/****************************************end of file************************************************/
