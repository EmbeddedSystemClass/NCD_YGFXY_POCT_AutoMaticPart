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
static MotorAction motorAction;																//电机指令保存
static xQueueHandle xMotorQueue;																//发送队列
static MotorActionResultEnum motorActionResult = MotorActionResultNone;						//电机指令发送缓存
static bool motorStopActionPermission = false;
/**************************************************************************************************/
/******************************************Static Methods******************************************/
/**************************************************************************************************/
static void MotorMoveToWaitCardPutIn(unsigned char num);
static void MotorMoveToStartTestLocation(unsigned char num);
static void PutCardOutOfDevice(unsigned char num);
static void MotorMoveToOriginLocation(unsigned char num);
static void motorMoveToDisablePutInCard(unsigned char num);
static void motorMoveToPutDownCardInPlace(void);
/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
/****************************************File Start************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
void MotorActionInit(void)
{
	xMotorQueue = xQueueCreate( 10, ( unsigned portBASE_TYPE ) sizeof(MotorAction));
	vSemaphoreCreateBinary(xMotorMutex);
	while(pdPASS == xSemaphoreTake(xMotorMutex, 0))
		;
}

void MotorActionFunction(void)
{
	if(pdPASS == xQueueReceive(xMotorQueue, &motorAction, portMAX_DELAY))
	{
		switch(motorAction.motorActionEnum)
		{
			case Motor1MoveDef :			motor1MoveToNum(motorAction.motorParm, true);	break;
				
			case Motor2MoveDef :			motor2MoveTo(1, 2, motorAction.motorParm, true);	break;
				
			case Motor4MoveDef :			motor4MoveTo(motorAction.motorParm, true);	break;
			
			case WaitCardPutInDef :			MotorMoveToWaitCardPutIn(motorAction.motorParm);	break;
				
			case StartTestDef :				MotorMoveToStartTestLocation(motorAction.motorParm);	break;
				
			case PutCardOutOfDeviceDef :	PutCardOutOfDevice(motorAction.motorParm);	break;
			
			case OriginLocationDef :		MotorMoveToOriginLocation(motorAction.motorParm);	break;
			
			case DisablePutInCardDef :		motorMoveToDisablePutInCard(motorAction.motorParm);	break;
											
			case PutDownCardInPlaceDef :	motorMoveToPutDownCardInPlace();	break;
				
			default: 						break;
		}
		
		xSemaphoreGive(xMotorMutex);
	}
}

MyRes StartMotorAction(MotorAction * motorAction, bool isStopAction, unsigned char waitCnt, portTickType waitBlockTime)
{
	if(motorAction == NULL)
		return My_Fail;
	
	while(pdPASS == xSemaphoreTake(xMotorMutex, 0))
		;
	
	if(isStopAction)
	{
		motorStopActionPermission = true;
		motor1StopMove();
		motor2StopMove();
	}
	else
		motorStopActionPermission = false;
	
	while(waitCnt--)
	{
		if(pdPASS == xQueueSend(xMotorQueue, motorAction, waitBlockTime))
			return My_Pass;
	}
	
	return My_Fail;
}

bool isMotorMoveEnd(portTickType waitBlockTime)
{
	if(pdPASS == xSemaphoreTake(xMotorMutex, waitBlockTime))
		return true;
	else
		return false;
}

bool isMotorInRightLocation(unsigned int motor1Location, unsigned int motor2Location, unsigned int motor4Location)
{
	Motor * motor = getMotor(Motor_1);

	if(motor1Location != MotorLocationNone)
	{
		if(motor1Location != motor->motorLocation)
			return false;
	}
	
	motor = getMotor(Motor_2);
	if(motor2Location != MotorLocationNone)
	{
		if(motor2Location != motor->motorLocation)
			return false;
	}
	
	#if(Motor4Type == Motor4UsartMotor)
		
		if(motor4Location != MotorLocationNone)
		{
			if(motor4Location != readMotorLocation())
				return false;
		}
		
	#elif(Motor4Type == Motor4IOMotor)
		
		motor = getMotor(Motor_4);
		if(motor4Location != MotorLocationNone)
		{
			if(motor4Location != motor->motorLocation)
				return false;
		}
		
	#endif
	
	
	return true;
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

static void motorMoveToDisablePutInCard(unsigned char num)
{
	motorStopActionPermission = false;
	
	motor2MoveTo(1, 2, Motor2_PutDownCardLocation, true);
	if(motorStopActionPermission == true)
			return;
	
	motor4MoveTo(Motor4_OpenLocation, true);
	
	motor2MoveTo(1, 2, Motor2_MidLocation, true);
	if(motorStopActionPermission == true)
			return;
	
	motor1MoveToNum(num, true);
	if(motorStopActionPermission == true)
			return;
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
