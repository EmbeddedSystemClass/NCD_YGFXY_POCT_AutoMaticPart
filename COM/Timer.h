#ifndef _TIMER_S_D_H__
#define _TIMER_S_D_H__

#include	"Define.h"

typedef enum
{ 
	TimerStopped = 0,							//��ʱ��ֹͣ״̬
	TimerDowning = 1,							//��ʱ�����ڼ�ʱ��
	TimerOut = 2,								//��ʱ����ʱʱ�䵽
	TimerError = 3								//��ʱ������
}TimerStatus;

#pragma pack(1)
typedef struct
{
	unsigned int start;
	unsigned int interval;
	TimerStatus timerStatus;
}Timer;
#pragma pack()

void timer_SetAndStart(Timer *t, unsigned int interval);
void timer_restart(Timer *t);
TimerStatus timer_expired(Timer *t);
unsigned int timer_surplus(Timer *t);
bool timerIsStartted(Timer * t);
	
#endif
