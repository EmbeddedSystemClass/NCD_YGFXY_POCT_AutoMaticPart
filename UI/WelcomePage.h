#ifndef __WELCOME_P_H__
#define __WELCOME_P_H__

#include	"MyStack.h"
#include	"Timer.h"
#include	"SystemSet_Data.h"
#include	"SelfTest_Fun.h"

typedef struct WelcomePageBuffer_tag {
	ERROR_SelfTest selfTestStatus;
	unsigned short currentPageId;
	Timer timer;													//ҳ�泬ʱʱ��
	unsigned char tempv1;											//��ʱ����
	unsigned short lcdinput[100];									//lcd�������
}WelcomePageBuffer;

MyRes createWelcomeActivity(Activity * thizActivity, Intent * pram);

#endif

