/****************************************file start************************************************/
#ifndef	_MOTOR_F_H_H
#define	_MOTOR_F_H_H

#include	"Motor_Data.h"
#include	"Define.h"

typedef struct MotorAction_tag{
	Motorx_Def motor;
	unsigned short motorParm;
	bool isWait;
}MotorAction;

void MotorActionInit(void);
void MotorActionFunction(void);
MyRes StartMotorAction(Motorx_Def motor, unsigned short motorParm, bool isWait);
bool isMotorActionOver(unsigned short motor1Location, unsigned short motor2Location, unsigned short motor4Location);
	
void MotorMoveToWaitCardPutIn(unsigned char num);
void MotorMoveToStartTestLocation(unsigned char num);
void PutCardOutOfDevice(void);
void MotorMoveToOriginLocation(unsigned char num);
void motorMoveToDisablePutInCard(unsigned char num);
#endif
/****************************************end of file***********************************************/
