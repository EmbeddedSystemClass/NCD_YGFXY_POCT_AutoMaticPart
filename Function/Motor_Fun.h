/****************************************file start************************************************/
#ifndef	_MOTOR_F_H_H
#define	_MOTOR_F_H_H

#include	"Motor_Data.h"
#include	"Define.h"

typedef enum
{
	MotorActionNone = 0x00,
	Motor1MoveDef = 0x09,
	Motor2MoveDef = 0x0a,
	Motor4MoveDef = 0x0b,
	WaitCardPutInDef = 0x11,								/*�ȴ��忨״̬*/
	StartTestDef = 0x12,									/*��ʼ����״̬*/
	PutCardOutOfDeviceDef = 0x13,							/*�����ų��豸*/
	PutCardOutOfDeviceAfterTestDef = 0x15,					/*�����ų��豸,ֻ���ڲ������*/
	OriginLocationDef = 0x14,								/*�ָ�����״̬*/
	PutDownCardInPlaceDef = 0x16,							/*���������Ŷ�λ*/
	PutDownCardInTestPlaceDef = 0x17,						/*�����ڲ��Զ˷ź�*/
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
	unsigned char step;
}MotorAction;

#define	MotorActionStructSize	sizeof(MotorAction)

void MotorActionInit(void);
void MotorActionFunction(void);
MyRes FormatParmAndStartMotorAction(MotorAction * motorAction, MotorActionEnum motorActionEnum, unsigned int motorParm, bool waitActionDone);
MyRes StartMotorAction(MotorAction * motorAction, bool waitActionDone);
bool isMotorMoveEnd(portTickType waitBlockTime);

#endif
/****************************************end of file***********************************************/
