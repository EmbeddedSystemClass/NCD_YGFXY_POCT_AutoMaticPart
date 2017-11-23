#ifndef __WELCOME_P_H__
#define __WELCOME_P_H__

#include	"MyStack.h"
#include	"Timer.h"
#include	"SystemSet_Data.h"
#include	"SelfTest_Fun.h"

typedef struct WelcomePageBuffer_tag {
	ERROR_SelfTest selfTestStatus;
	unsigned short currentPageId;
	Timer timer;													//页面超时时间
	unsigned short lcdinput[20];									//lcd输入解析
	bool canAram;
}WelcomePageBuffer;

MyRes createWelcomeActivity(Activity * thizActivity, Intent * pram);

#endif

