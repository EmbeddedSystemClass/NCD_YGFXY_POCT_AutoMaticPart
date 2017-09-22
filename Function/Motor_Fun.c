/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/**************************************************************************************************/
/******************************************Header List*********************************************/
/**************************************************************************************************/
#include	"Motor_Fun.h"

#include	"Motor1_Fun.h"
#include	"Motor2_Fun.h"
#include	"Motor4_Fun.h"

#include	"Motor_Data.h"
/**************************************************************************************************/
/******************************************Static Variables****************************************/
/**************************************************************************************************/
static xQueueHandle xMotorActionQueue = NULL ;				//电机指令队列
static MotorAction motorAction;								//电机指令保存
static MotorAction motorActionSendBuf;						//电机指令发送缓存
/**************************************************************************************************/
/******************************************Static Methods******************************************/
/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
/****************************************File Start************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
void MotorActionInit(void)
{
	xMotorActionQueue = xQueueCreate(10, sizeof(MotorAction));
}

void MotorActionFunction(void)
{
	if(pdPASS == xQueueReceive( xMotorActionQueue, &motorAction, portMAX_DELAY))
	{	
		switch(motorAction.motor)
		{
			case Motor_1 :	motor1MoveToNum(motorAction.motorParm, motorAction.isWait);	break;
				
			case Motor_2 :	motor2MoveTo(motorAction.motorParm, motorAction.isWait);	break;
				
			case Motor_4 :	motor4MoveTo(motorAction.motorParm, motorAction.isWait);	break;
				
			default: break;
		}
	}
}

MyRes StartMotorAction(Motorx_Def motor, unsigned short motorParm, bool isWait)
{
	motorActionSendBuf.motor = motor;
	motorActionSendBuf.motorParm = motorParm;
	motorActionSendBuf.isWait = isWait;
	
	if(pdPASS == xQueueSend( xMotorActionQueue, &motorActionSendBuf, 1000/portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;	
}

bool isMotorActionOver(unsigned short motor1Location, unsigned short motor2Location, unsigned short motor4Location)
{
	Motor * motor = getMotor(Motor_1);

	if(motor1Location != MotorLocationNone)
	{
		if(motor1Location != motor->motorLocation)
			return false;
	}
	
	motor++;
	if(motor2Location != MotorLocationNone)
	{
		if(motor2Location != motor->motorLocation)
			return false;
	}
	
	motor++;
	if(motor4Location != MotorLocationNone)
	{
		if(motor4Location != motor->motorLocation)
			return false;
	}
	
	return true;
}

void MotorMoveToWaitCardPutIn(unsigned char num)
{
	StartMotorAction(Motor_4, Motor4_OpenLocation, true);
	
	if(num != getMotorxLocation(Motor_1))
	{
		StartMotorAction(Motor_2, Motor2_MidLocation, true);
	
		StartMotorAction(Motor_1, num, true);
	}
	
	StartMotorAction(Motor_2, Motor2_WaitCardLocation, true);
}

void MotorMoveToStartTestLocation(unsigned char num)
{
	StartMotorAction(Motor_4, Motor4_OpenLocation, true);
	StartMotorAction(Motor_2, Motor2_MidLocation, true);
	StartMotorAction(Motor_1, num, true);
	StartMotorAction(Motor_2, Motor2_CatchCardLocation, true);
	StartMotorAction(Motor_4, Motor4_CardLocation, true);
	StartMotorAction(Motor_2, Motor2_StartTestLocation, true);
}

void PutCardOutOfDevice(void)
{
	StartMotorAction(Motor_4, Motor4_OpenLocation, false);
	StartMotorAction(Motor_2, 0, true);
	StartMotorAction(Motor_2, Motor2_MidLocation, true);
}

void MotorMoveToOriginLocation(unsigned char num)
{
	StartMotorAction(Motor_4, Motor4_OpenLocation, false);
	StartMotorAction(Motor_2, Motor2_MidLocation, true);
	
	if(num != getMotorxLocation(Motor_1))
		StartMotorAction(Motor_1, num, true);
}

void motorMoveToDisablePutInCard(unsigned char num)
{
	StartMotorAction(Motor_2, Motor2_PutDownCardLocation, true);
	StartMotorAction(Motor_4, Motor4_OpenLocation, true);
	StartMotorAction(Motor_2, Motor2_MidLocation, true);
	StartMotorAction(Motor_1, num, true);
}

/****************************************end of file***********************************************/
