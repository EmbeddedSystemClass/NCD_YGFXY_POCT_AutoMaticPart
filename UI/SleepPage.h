#ifndef __SLEEP_P_H__
#define __SLEEP_P_H__

#include	"UI_Data.h"
#include	"Define.h"
#include	"Timer.h"
#include	"SystemSet_Data.h"

typedef struct SleepPageBuffer_tag {
	unsigned short lcdinput[20];
}SleepPageBuffer;


MyRes createSleepActivity(Activity * thizActivity, Intent * pram);

#endif

