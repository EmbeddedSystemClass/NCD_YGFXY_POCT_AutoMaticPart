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
	unsigned char testIndex;								//测试次数索引
	ResultState cardTestResult;								//测试结果
	ScanCodeResult cardScanResult;							//扫码结果
	PaiduiUnitData paiduiUnitData[MaxQualityCount];			//数据缓存
	PaiduiUnitData * currentPaiduiUnitData;					//当前数据缓存
	PaiduiUnitData * tempPaiduiUnitData;					//当前数据缓存
	double resultSum;										//有效测试结果和
	double resultNum;										//有效测试数目
	bool isCancel;											//是否取消质控
	unsigned char statuesNullNum;							//空状态数目
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

