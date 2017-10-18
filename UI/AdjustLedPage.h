#ifndef __ADJUSTLED_P_H__
#define __ADJUSTLED_P_H__

#include	"UI_Data.h"
#include	"PaiduiUnitData.h"
#include	"SystemSet_Data.h"
#include	"Motor_Fun.h"

typedef struct AdjustLedPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[100];
	bool isTestting;									//是否正在测试中
	bool isReady;
	ResultState cardpretestresult;
	unsigned short targetValue;							//目标值
	unsigned short targetRange;							//误差值
	unsigned short maxPoint[2];							//0--最大值，1--最大值位置
	unsigned short i;
	unsigned char testCnt;								//查找次数
	SystemSetData systemSetData;						//系统参数副本，用于保存失败回滚
	PaiduiUnitData paiduiUnitData;
	unsigned short ledLevel;
	MotorAction motorAction;
}AdjustLedPageBuffer;


MyRes createAdjustLedActivity(Activity * thizActivity, Intent * pram);

#endif
