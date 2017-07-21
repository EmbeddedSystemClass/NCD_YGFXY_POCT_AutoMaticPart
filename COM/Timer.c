/***************************************************************************************************
*FileName:Timer_Data
*Description:用作程序的计时器
*Author:xsx
*Data: 2016年5月14日15:49:39
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"Timer.h"

/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
unsigned int GB_ClockTime = 0;													//计时器的时基

/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionNameesetGBClockTime�
*Description：重启计时器的时基，可在没有测试任务是调用，也可不用，unsigned int最长计时时间为4294967295 S
*				 等于3268年
*Input：None
*Output：None
*Author：xsx
*Data：2016年5月14日16:17:31
***************************************************************************************************/
void ResetGBClockTime(void)
{
	GB_ClockTime = 0;
}

unsigned int getGBClockTime(void)
{
	return GB_ClockTime;
}

/***************************************************************************************************
*FunctionName：timer_set
*Description：初始化一个定时器
*Input：t -- 待初始化的定时器
*		interval -- 定时器计时时间（秒）
*Output：None
*Author：xsx
*Data：2016年5月14日16:03:58
***************************************************************************************************/
void timer_set(Timer *t, unsigned int interval)
{
  t->interval = interval;
  t->start = GB_ClockTime;
}

/***************************************************************************************************
*FunctionName: timer_stop
*Description: 停止定时器
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年3月10日09:36:03
***************************************************************************************************/
void timer_stop(Timer *t)
{
  t->start = 0;
}

/***************************************************************************************************
*FunctionName：timer_reset
*Description：重置定时器
*Input：None
*Output：None
*Author：xsx
*Data：2016年5月14日16:04:51
***************************************************************************************************/
void timer_reset(Timer *t)
{
  t->start += t->interval;
}

/***************************************************************************************************
*FunctionName：timer_restart
*Description：重启定时器
*Input：None
*Output：None
*Author：xsx
*Data：2016年5月14日16:05:25
***************************************************************************************************/
void timer_restart(Timer *t)
{
  t->start = GB_ClockTime;
}

/***************************************************************************************************
*FunctionName：timer_expired
*Description：判断计时时间是否到达
*Input：None
*Output：None
*Author：xsx
*Data：2016年5月14日16:08:55
***************************************************************************************************/
TimerState_Def timer_expired(Timer *t)
{
	if(t)
	{
		if(timerIsStartted(t))
		{
			if((GB_ClockTime - t->start) >= t->interval)
				return TimeOut;
			else
				return TimeNotTo;
		}
	}
	
	return TimeError;
}

/***************************************************************************************************
*FunctionName：timer_surplus
*Description：读取计时器还需多久到达计时时间
*Input：None
*Output：None
*Author：xsx
*Data：2016年5月14日16:13:40
***************************************************************************************************/
unsigned int timer_surplus(Timer *t)
{
	if(t)
	{
		if(timerIsStartted(t))
		{
			if((t->interval + t->start) > GB_ClockTime)
				return (t->interval - (GB_ClockTime - t->start));
			else
				return 0;
		}
	}
	
	return 0xffffff;
}

/***************************************************************************************************
*FunctionName：timer_used
*Description：读取已经过去多长时间
*Input：None
*Output：None
*Author：xsx
*Data：2016年5月14日16:13:40
***************************************************************************************************/
unsigned int timer_used(Timer *t)
{
	if(t)
	{
		return (GB_ClockTime - t->start);
	}
	else
		return 0;
}

/***************************************************************************************************
*FunctionName：timer_Count
*Description：距离定时器开始已经过了多长时间，无论时间是否到达
*Input：None
*Output：None
*Author：xsx
*Data：
***************************************************************************************************/
unsigned int timer_Count(Timer * t)
{
	if(t)
		return (GB_ClockTime - t->start);
	else
		return 0;
}

/***************************************************************************************************
*FunctionName: timerIsStartted
*Description: 判断定时器是否已经启动
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月8日16:27:47
***************************************************************************************************/
bool timerIsStartted(Timer * t)
{
	if(t)
	{
		if(t->start == 0)
			return false;
		else
			return true;
	}
	
	return false;
}
