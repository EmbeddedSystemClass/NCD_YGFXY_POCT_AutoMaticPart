/****************************************file start****************************************************/
#ifndef _MOTOR_E_H
#define	_MOTOR_E_H

#include	"Define.h"

#pragma pack(1)
typedef struct Motor_Tag
{
	unsigned char highTime;							//高电平时间，*100us
	unsigned char lowTime;							//低电平时间，*100us
	unsigned char periodCnt;						//当前周期内，定时器溢出中断次数
	bool isFront;									//是否前进
	unsigned short moveStepNum;						//运动步数
	unsigned short motorLocation;					//电机当前位置
	unsigned short motorTargetLocation;				//电机目标位置
	unsigned short motorMaxLocation;				//电机最大位置
	unsigned short parm1;							//电机参数1
	bool parm2; 									//电机参数2
	unsigned short parm3; 									//电机参数3
}Motor;
#pragma pack()

#endif

/****************************************end of file************************************************/
