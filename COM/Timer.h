#ifndef _TIMER_S_D_H__
#define _TIMER_S_D_H__

#include	"Define.h"

typedef enum
{ 
	TimerStopped = 0,							//定时器停止状态
	TimerDowning = 1,							//定时器正在计时中
	TimerOut = 2,								//定时器计时时间到
	TimerError = 3								//定时器错误
}TimerStatus;

#pragma pack(1)
typedef struct
{
	unsigned int start;
	unsigned int interval;
}Timer;
#pragma pack()

void timer_SetAndStart(Timer *t, unsigned int interval);
void timer_restart(Timer *t);
TimerStatus timer_expired(Timer *t);
unsigned int timer_surplus(Timer *t);
unsigned int timer_Count(Timer *t);
bool timerIsStartted(Timer * t);
unsigned int getClockValue(void);

#endif
