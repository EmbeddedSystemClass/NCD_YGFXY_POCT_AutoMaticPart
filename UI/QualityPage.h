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
	unsigned char testCnt;								//测试次数索引
	ResultState cardTestResult;								//测试结果
	ScanCodeResult cardScanResult;							//扫码结果
	TestData  testData;										//测试数据缓存
	double resultSum;										//有效测试结果和
	Timer cardTimer;										//插卡等待时间
	
	MotorAction motorAction;
	bool isStartted;
	unsigned char testCardIndex2;
	double tempValue1;
	double tempValue2;
}QualityPageBuffer;

#define	QualityPageBufferStructSize	sizeof(QualityPageBuffer)

MyRes createQualityActivity(Activity * thizActivity, Intent * pram);

#endif

