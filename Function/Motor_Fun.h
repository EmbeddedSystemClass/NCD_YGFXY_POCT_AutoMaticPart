/****************************************file start************************************************/
#ifndef	_MOTOR_F_H_H
#define	_MOTOR_F_H_H

#include	"Motor_Data.h"
#include	"Define.h"

typedef enum
{
	Motor1MoveDef = 0x09,
	Motor2MoveDef = 0x0a,
	Motor4MoveDef = 0x0b,
	WaitCardPutInDef = 0x11,								/*等待插卡状态*/
	StartTestDef = 0x12,									/*开始测试状态*/
	PutCardOutOfDeviceDef = 0x13,							/*将卡排除设备*/
	OriginLocationDef = 0x14,								/*恢复待机状态*/
	DisablePutInCardDef = 0x15,
	PutDownCardInPlaceDef = 0x16,							/*将卡放在排队位*/
}MotorActionEnum;

typedef enum
{
	MotorActionResultNone = 0x11,							//空闲
	MotorActionResultError = 0x12,							//移动错误
	MotorActionResultOk = 0x13,								//移动完成
	MotorActionResultMoving = 0x14,							//移动中
}MotorActionResultEnum;

typedef struct MotorAction_tag{
	MotorActionEnum motorActionEnum;
	unsigned int motorParm;
}MotorAction;

#define	MotorActionStructSize	sizeof(MotorAction)

void MotorActionInit(void);
void MotorActionFunction(void);
MyRes StartMotorAction(MotorAction * motorAction, bool isStopAction, unsigned char waitCnt, portTickType waitBlockTime);
bool isMotorInRightLocation(unsigned int motor1Location, unsigned int motor2Location, unsigned int motor4Location);
bool isMotorMoveEnd(portTickType waitBlockTime);

#endif
/****************************************end of file***********************************************/
