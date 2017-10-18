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
	WaitCardPutInDef = 0x11,								/*�ȴ��忨״̬*/
	StartTestDef = 0x12,									/*��ʼ����״̬*/
	PutCardOutOfDeviceDef = 0x13,							/*�����ų��豸*/
	OriginLocationDef = 0x14,								/*�ָ�����״̬*/
	DisablePutInCardDef = 0x15,
	PutDownCardInPlaceDef = 0x16,							/*���������Ŷ�λ*/
}MotorActionEnum;

typedef enum
{
	MotorActionResultNone = 0x11,							//����
	MotorActionResultError = 0x12,							//�ƶ�����
	MotorActionResultOk = 0x13,								//�ƶ����
	MotorActionResultMoving = 0x14,							//�ƶ���
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
