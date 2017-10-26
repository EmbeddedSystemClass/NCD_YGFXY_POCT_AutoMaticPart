/****************************************file start****************************************************/
#ifndef _DEVICEQUALITY_E_H
#define	_DEVICEQUALITY_E_H

#include	"DateTime.h"
#include	"Operator.h"
#include	"Define.h"
#include	"QRCode.h"
#include	"TestData.h"

#define		DeviceQualityRecordPageShowNum	8

#pragma pack(1)
typedef struct
{
	DateTime dateTime;												//�ʿ�ʱ��
	Operator operator;												//�ʿ���
	char itemName[ItemNameLen];										//�ʿ���Ŀ����
	double standardValue;											//�ʿر�׼ֵ
	double avgValue;												//����ƽ��ֵ
	double avgPicha;												//ƽ��ƫ��
	double testValue[MaxQualityCount];								//�ʿز���ֵ
	double testPicha[MaxQualityCount];								//�ʿز���ƫ��
	MyBool testResult[MaxQualityCount];								//�ʿؽ����־
	bool isOk;														//�ʿؽ��
	char desc[50];													//�ʿ�˵��
	unsigned short crc;
}DeviceQuality;
#pragma pack()

#define	DeviceQualityStructSize		sizeof(DeviceQuality)							//��ౣ����û���Ŀ
#define	DeviceQualityStructCrcSize	DeviceQualityStructSize - 2						//��ౣ����û���Ŀ

#endif

/****************************************end of file************************************************/
