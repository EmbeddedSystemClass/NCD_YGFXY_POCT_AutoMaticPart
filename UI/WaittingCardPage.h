#ifndef __WAITCARD_P_H__
#define __WAITCARD_P_H__

#include	"Timer.h"
#include	"MyTest_Data.h"
#include	"UI_Data.h"
#include	"Motor_Fun.h"

typedef struct WaitPage_tag {
	Timer timer2;				//º‰∏ÙÃ· æ≤Âø®
	PaiduiUnitData * currenttestdata;
	unsigned short lcdinput[20];
	MyRes isMotorStartted;
	bool isBack;
	bool isBackButtonPressed;
	MotorAction motorAction;
}WaitPageData;

MyRes createWaittingCardActivity(Activity * thizActivity, Intent * pram);

#endif

