#ifndef __RETSET_P_H__
#define __RETSET_P_H__

#include	"Define.h"
#include	"UI_Data.h"
#include	"QRCode.h"
#include	"TestData.h"
#include	"Timer.h"
#include	"DateTime.h"
#include	"Motor_Task.h"

typedef struct ReTestPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[100];
	bool isTestting;
	unsigned char i;
	unsigned short testTotalCnt;
	unsigned short testCnt;
	Timer testTimer;
	float result[PaiDuiWeiNum];
	QRCode qrCode[PaiDuiWeiNum];
	ScanCodeResult scancode;
	TestData testData;
	DateTime dateTime;
	char resultDesc[100];
	unsigned char waitCardIndex;
	unsigned char testCardIndex;
	unsigned char testCardIndex2;
	unsigned char motorStep;
	unsigned char testStep;
	unsigned short count;
	MotorAction motorAction;
}ReTestPageBuffer;


MyRes createReTestActivity(Activity * thizActivity, Intent * pram);

#endif

