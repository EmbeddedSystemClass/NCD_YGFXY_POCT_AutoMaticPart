#ifndef __MOTOR_T_H__
#define __MOTOR_T_H__

#include	"Define.h"
#include	"Motor_Fun.h"

typedef enum
{ 
	WaitPutInCard = 1,
	MoveToStartTestLocation,
	OutOutCard,
	OriginLocation
}MotorActionName;

typedef struct MotorAction_tag{
	MotorActionName motorActionName;
	unsigned char motorActionParm;
}MotorAction;

char StartMotorTask(void);
MyRes StartMotorAction(MotorAction * motorAction);
bool isMotorActionOver(void);

#endif

