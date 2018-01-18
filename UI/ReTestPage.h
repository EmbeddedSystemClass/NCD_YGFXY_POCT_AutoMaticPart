#ifndef __RETSET_P_H__
#define __RETSET_P_H__

#include	"Define.h"
#include	"UI_Data.h"
#include	"QRCode.h"
#include	"TestData.h"
#include	"Timer.h"
#include	"DateTime.h"
#include	"PaiduiUnitData.h"
#include	"Motor_Fun.h"

typedef struct ReTestPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[50];
	char longBuffer[1024];
	unsigned char testStep;								//0 -- 插卡, 1 -- 老化中
	unsigned int testNum;								//测试次数，9个卡全部测试一次加1
	unsigned char testIndex;							//当前测试卡索引
	PaiduiUnitData paiduiUnitData[PaiDuiWeiNum];
	PaiduiUnitData * currentPaiduiUnitData;
	unsigned char i;
	MotorAction motorAction;
	ScanCodeResult cardScanResult;							//扫码结果
}ReTestPageBuffer;


MyRes createReTestActivity(Activity * thizActivity, Intent * pram);

#endif

