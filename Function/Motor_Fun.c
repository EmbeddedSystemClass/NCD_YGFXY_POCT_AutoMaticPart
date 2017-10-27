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

#include 	"FreeRTOS.h"
#include 	"task.h"
/**************************************************************************************************/
/******************************************Static Variables****************************************/
/**************************************************************************************************/
static xSemaphoreHandle xMotorMutex;
static MotorAction S_MotorAction;																//电机指令保存
static bool motorStopActionPermission = false;
/**************************************************************************************************/
/******************************************Static Methods******************************************/
/**************************************************************************************************/
static void MotorMoveToWaitCardPutIn(unsigned char num);
static void MotorMoveToStartTestLocation(unsigned char num);
static void PutCardOutOfDevice(unsigned char num);
static void PutCardOutOfDeviceAfterTest(void);
static void MotorMoveToOriginLocation(unsigned char num);
static void motorMoveToPutDownCardInPlace(void);
/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
/****************************************File Start************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
void MotorActionInit(void)
{
	vSemaphoreCreateBinary(xMotorMutex);
	while(pdPASS == xSemaphoreTake(xMotorMutex, 0))
		;
}

void MotorActionFunction(void)
{
	if(S_MotorAction.motorActionEnum != MotorActionNone)
	{
		switch(S_MotorAction.motorActionEnum)
		{
			case Motor1MoveDef :			motor1MoveToNum(S_MotorAction.motorParm, true);	break;
				
			case Motor2MoveDef :			motor2MoveTo(1, 2, S_MotorAction.motorParm, true);	break;
				
			case Motor4MoveDef :			motor4MoveTo(S_MotorAction.motorParm, true);	break;
			
			case WaitCardPutInDef :			MotorMoveToWaitCardPutIn(S_MotorAction.motorParm);	break;
				
			case StartTestDef :				MotorMoveToStartTestLocation(S_MotorAction.motorParm);	break;
				
			case PutCardOutOfDeviceDef :	PutCardOutOfDevice(S_MotorAction.motorParm);	break;
			
			case PutCardOutOfDeviceAfterTestDef :	PutCardOutOfDeviceAfterTest();	break;
			
			case OriginLocationDef :		MotorMoveToOriginLocation(S_MotorAction.motorParm);	break;
											
			case PutDownCardInPlaceDef :	motorMoveToPutDownCardInPlace();	break;
				
			default: 						break;
		}
		
		S_MotorAction.motorActionEnum = MotorActionNone;
		
		xSemaphoreGive(xMotorMutex);
	}
	
	vTaskDelay(100 / portTICK_RATE_MS);
}

MyRes StartMotorAction(MotorAction * motorAction, bool isStopWhenBusy, bool waitActionDone)
{
	if(motorAction == NULL)
		return My_Fail;
	
	if(S_MotorAction.motorActionEnum != MotorActionNone)
	{
		if(isStopWhenBusy)
		{
			motorStopActionPermission = true;
			motor1StopMove();
			motor2StopMove();
			
			xSemaphoreTake(xMotorMutex, portMAX_DELAY);
		}
		else
			return My_Fail;
	}
	
	S_MotorAction.motorActionEnum = motorAction->motorActionEnum;
	S_MotorAction.motorParm = motorAction->motorParm;
		
	if(waitActionDone)
	{
		xSemaphoreTake(xMotorMutex, portMAX_DELAY);
	}
	
	return My_Pass;
}

MyRes StartMotorActionWithParm(MotorActionEnum motorActionEnum, unsigned int motorParm, bool isStopWhenBusy, bool waitActionDone)
{
	MotorAction tempMotorAction;
	tempMotorAction.motorActionEnum = motorActionEnum;
	tempMotorAction.motorParm = motorParm;
	
	return StartMotorAction(&tempMotorAction, isStopWhenBusy, waitActionDone);
}

bool isMotorMoveEnd(portTickType waitBlockTime)
{
	if(pdPASS == xSemaphoreTake(xMotorMutex, waitBlockTime))
		return true;
	else
		return false;
}


static void MotorMoveToWaitCardPutIn(unsigned char num)
{
	motorStopActionPermission = false;
	
	motor4MoveTo(Motor4_OpenLocation, true);
	
	if(num != getMotorxLocation(Motor_1))
	{
		motor2MoveTo(1, 2, Motor2_MidLocation, true);
		if(motorStopActionPermission == true)
			return;
	
		motor1MoveToNum(num, true);
		if(motorStopActionPermission == true)
			return;
	}
	
	motor2MoveTo(1, 2, Motor2_WaitCardLocation, true);
	if(motorStopActionPermission == true)
			return;
}


static void MotorMoveToStartTestLocation(unsigned char num)
{
	motorStopActionPermission = false;
	
	#if(Motor4Type == Motor4UsartMotor)
		motor4Reset();
	#endif
	
	motor4MoveTo(Motor4_OpenLocation, true);
	
	motor2MoveTo(1, 2, Motor2_MidLocation, true);
	if(motorStopActionPermission == true)
			return;
	
	motor1MoveToNum(num, true);
	if(motorStopActionPermission == true)
			return;
	
	motor2MoveTo(1, 2, Motor2_CatchCardLocation, true);
	if(motorStopActionPermission == true)
			return;
	
	motor4MoveTo(Motor4_CardLocation, true);
	
	motor2MoveTo(1, 2, Motor2_StartTestLocation, true);
	if(motorStopActionPermission == true)
			return;
}


static void PutCardOutOfDevice(unsigned char num)
{
	motorStopActionPermission = false;
	
	MotorMoveToOriginLocation(num);
	
	motor4MoveTo(Motor4_OpenLocation, true);
	
	motor2MoveTo(1, 2, Motor2_StartTestLocation, true);
	if(motorStopActionPermission == true)
			return;
	motor4MoveTo(Motor4_CardLocation, true);
	
	motor2MoveTo(1, 2, Motor2_PutCardOutLocation, true);
	if(motorStopActionPermission == true)
			return;

	motor4MoveTo(Motor4_OpenLocation, true);
	
	motor2MoveTo(1, 2, Motor2_MidLocation, true);
	if(motorStopActionPermission == true)
			return;
}

static void PutCardOutOfDeviceAfterTest(void)
{
	motor2MoveTo(1, 2, Motor2_PutCardOutLocation, true);
	if(motorStopActionPermission == true)
			return;

	motor4MoveTo(Motor4_OpenLocation, true);
	
	motor2MoveTo(1, 2, Motor2_MidLocation, true);
	if(motorStopActionPermission == true)
			return;
}

static void MotorMoveToOriginLocation(unsigned char num)
{
	motorStopActionPermission = false;
	
	motor4MoveTo(Motor4_OpenLocation, true);
	
	motor2MoveTo(1, 2, Motor2_MidLocation, true);
	if(motorStopActionPermission == true)
			return;
	
	if(num != getMotorxLocation(Motor_1))
	{
		motor1MoveToNum(num, true);
		if(motorStopActionPermission == true)
			return;
	}
}


static void motorMoveToPutDownCardInPlace(void)
{
	motorStopActionPermission = false;
	
	motor4MoveTo(Motor4_OpenLocation, true);

	motor2MoveTo(1, 2, Motor2_WaitCardLocation, true);
	if(motorStopActionPermission == true)
			return;
	
	motor4MoveTo(Motor4_CardLocation, true);
	
	motor2MoveTo(1, 2, Motor2_PutDownCardLocation, true);
	if(motorStopActionPermission == true)
			return;
	
	motor4MoveTo(Motor4_OpenLocation, true);
	
	motor2MoveTo(1, 2, Motor2_MidLocation, true);
	if(motorStopActionPermission == true)
			return;
}

/****************************************end of file***********************************************/
