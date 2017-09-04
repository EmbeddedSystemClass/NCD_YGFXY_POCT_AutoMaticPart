#ifndef __QUALITY_P_H__
#define __QUALITY_P_H__

#include	"UI_Data.h"
#include	"DeviceQuality.h"
#include	"Timer.h"
#include	"Motor_Task.h"

typedef struct QualityPageBuffer_tag {
	unsigned short lcdinput[100];
	char tempBuf[50];
	DeviceQuality * deviceQuality;
	unsigned char testStep;
	unsigned char testCnt;								//���Դ�������
	ResultState cardTestResult;								//���Խ��
	ScanCodeResult cardScanResult;							//ɨ����
	TestData  testData;										//�������ݻ���
	double resultSum;										//��Ч���Խ����
	Timer cardTimer;										//�忨�ȴ�ʱ��
	
	MotorAction motorAction;
	bool isStartted;
	unsigned char testCardIndex2;
	double tempValue1;
	double tempValue2;
}QualityPageBuffer;

#define	QualityPageBufferStructSize	sizeof(QualityPageBuffer)

MyRes createQualityActivity(Activity * thizActivity, Intent * pram);

#endif

