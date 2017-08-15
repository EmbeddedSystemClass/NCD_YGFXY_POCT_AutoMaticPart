#ifndef __QUALITY_P_H__
#define __QUALITY_P_H__

#include	"UI_Data.h"
#include	"DeviceQuality.h"

typedef struct QualityPageBuffer_tag {
	unsigned short lcdinput[100];
	char tempBuf[50];
	DeviceQuality * deviceQuality;
	bool canTestNow;										//�����ܷ���в���
	bool isTestting;										//�Ƿ����ڽ���һ�β���
	unsigned char testIndex;								//���Դ�������
	ResultState cardTestResult;								//���Խ��
	ScanCodeResult cardScanResult;							//ɨ����
	TestData  testData;										//�������ݻ���
	float tempValue1;
	double resultSum;										//��Ч���Խ����
	double tempValue2;
}QualityPageBuffer;

#define	QualityPageBufferStructSize	sizeof(QualityPageBuffer)

MyRes createQualityActivity(Activity * thizActivity, Intent * pram);

#endif

