#ifndef __TIMEDOWN_P_H__
#define __TIMEDOWN_P_H__

#include	"Timer.h"
#include	"MyTest_Data.h"
#include	"UI_Data.h"
#include	"Motor_Task.h"

typedef struct TimeDownPage_tag {
	Timer *S_Timer;
	PaiduiUnitData * currenttestdata;
	MotorAction motorAction;
	unsigned short count;
	unsigned short time;
	bool canToTestPage;
	unsigned char cardNum;
}TimeDownPageData;


MyRes createTimeDownActivity(Activity * thizActivity, Intent * pram);

#endif

