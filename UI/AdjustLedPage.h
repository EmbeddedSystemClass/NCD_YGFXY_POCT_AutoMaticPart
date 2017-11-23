#ifndef __ADJUSTLED_P_H__
#define __ADJUSTLED_P_H__

#include	"UI_Data.h"
#include	"PaiduiUnitData.h"
#include	"SystemSet_Data.h"
#include	"Motor_Fun.h"

#define	MinLedValue		600								//0.5V
#define	MiddleLedValue	1200							//1V
#define	MaxLedValue		1800							//1.5V
#define	AdjustLedStep	100
#define	MaxAdjustCnt	7

typedef struct AdjustLedPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[100];
	ResultState cardTestResult;								//测试结果
	unsigned short targetValue;							//目标值
	unsigned short targetRange;							//误差值
	unsigned short maxPoint[2];							//0--最大值，1--最大值位置
	unsigned short i;
	unsigned char testIndex;								//查找次数
	SystemSetData systemSetData;						//系统参数副本，用于保存失败回滚
	PaiduiUnitData paiduiUnitData;
	unsigned short currentLedValue;						//当前系统中的led值
	MotorAction motorAction;
}AdjustLedPageBuffer;


MyRes createAdjustLedActivity(Activity * thizActivity, Intent * pram);

#endif
