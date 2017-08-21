#ifndef __LUNCH_P_H__
#define __LUNCH_P_H__

#include	"Timer.h"
#include	"MyTest_Data.h"
#include	"UI_Data.h"
#include	"SystemSet_Data.h"
#include	"Timer.h"

typedef struct LunchPageBuffer_tag {
	Timer timer;
	unsigned char presscount;
	PaiduiUnitData * currentTestDataBuffer;							//��ǰ�������ݵ�ַ
	unsigned char step;												//��������
	char buf[100];
	unsigned short lcdinput[100];
	CreateTestErrorType error;
}LunchPageBuffer;

MyRes createLunchActivity(Activity * thizActivity, Intent * pram);

#endif

