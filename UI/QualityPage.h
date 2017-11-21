#ifndef __QUALITY_P_H__
#define __QUALITY_P_H__

#include	"UI_Data.h"
#include	"DeviceQuality.h"
#include	"Timer.h"
#include	"PaiduiUnitData.h"
#include	"Motor_Fun.h"

typedef struct QualityPageBuffer_tag {
	unsigned short lcdinput[100];
	char tempBuf[50];
	unsigned int ScheduleCount;
	DeviceQuality * deviceQuality;
	unsigned char testIndex;								//���Դ�������
	ResultState cardTestResult;								//���Խ��
	ScanCodeResult cardScanResult;							//ɨ����
	PaiduiUnitData paiduiUnitData[MaxQualityCount];			//���ݻ���
	PaiduiUnitData * currentPaiduiUnitData;					//��ǰ���ݻ���
	PaiduiUnitData * tempPaiduiUnitData;					//��ǰ���ݻ���
	double resultSum;										//��Ч���Խ����
	double resultNum;										//��Ч������Ŀ
	bool isCancel;											//�Ƿ�ȡ���ʿ�
	unsigned char statuesNullNum;							//��״̬��Ŀ
	double tempValue1;
	double tempValue2;
	float tempFloat1;
	unsigned short i;
	unsigned short tempShortV1;
	unsigned short minTime;
	int tempIntV1;
	bool isMotorStartted;
	MotorAction motorAction;
}QualityPageBuffer;

#define	QualityPageBufferStructSize	sizeof(QualityPageBuffer)

MyRes createQualityActivity(Activity * thizActivity, Intent * pram);

#endif

