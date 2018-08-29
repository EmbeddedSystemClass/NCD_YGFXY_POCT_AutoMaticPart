#ifndef __SampleID_P_H__
#define __SampleID_P_H__

#include	"Define.h"
#include	"Timer.h"
#include	"MyTest_Data.h"
#include	"UI_Data.h"
#include	"Motor_Fun.h"

typedef struct SampleIDPage_tag {
	char tempbuf[150];
	unsigned int tempv1;
	unsigned char sampleIdLen;
	PaiduiUnitData * currenttestdata;												//当前测试数据
	unsigned short lcdinput[50];
	bool isConfirmed;
	MyRes isMotorStartted;
	MotorAction motorAction;
	unsigned int cnt;
}SampleIDPage;

MyRes createSampleActivity(Activity * thizActivity, Intent * pram);

#endif

