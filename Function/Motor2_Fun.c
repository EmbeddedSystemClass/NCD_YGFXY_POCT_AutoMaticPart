/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Motor2_Fun.h"

#include	"Motor2_Driver.h"
#include	"Timer3_Driver.h"

#include	"Motor_Data.h"
#include	"Motor.h"

#include	"Delay.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static Motor * motor2 = NULL;
static bool motor2StopMovePermission = false;
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void motor2MoveStep(bool isFront, unsigned short stepNum, bool isWait)
{
	motor2 = getMotor(Motor_2);

	motor2->isFront = isFront;
	motor2->motorLocation = 0;
	motor2->motorTargetLocation = 0;
	
	if(motor2->isFront)
	{
		setMotor2DirGPIO(ON);
		motor2->motorTargetLocation = stepNum;
	}
	else
	{
		setMotor2DirGPIO(OFF);
		motor2->motorLocation = stepNum;
	}
	vTaskDelay(10 / portTICK_RATE_MS);
	
	motor2->periodCnt = 0;
	motor2->highTime = 1;
	motor2->lowTime = 2;
	motor2->moveStepNum = stepNum;
	
	while(isWait && motor2->motorLocation != motor2->motorTargetLocation)
	{
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void motor2MoveTo(unsigned char highTime, unsigned char lowTime, unsigned short location, bool isWait)
{
	motor2 = getMotor(Motor_2);

	if(motor2->motorLocation == location)
		return;
	else if(motor2->motorLocation > location)
		motor2->isFront = false;
	else
		motor2->isFront = true;

	if(motor2->isFront)
	{
		setMotor2DirGPIO(ON);
		if(location == Motor2_MidLocation)
			motor2->motorLocation = 0;
	}
	else
		setMotor2DirGPIO(OFF);
	vTaskDelay(10 / portTICK_RATE_MS);
	
	motor2->periodCnt = 0;
	motor2->parm1 = 0;
	motor2->highTime = highTime;
	motor2->lowTime = lowTime;
	motor2->motorTargetLocation = location;
	motor2->moveStepNum = 60000;
	motor2StopMovePermission = false;
	
	while(isWait && motor2->motorLocation != motor2->motorTargetLocation)
	{
		vTaskDelay(100 / portTICK_RATE_MS);
		if(motor2StopMovePermission)
		{
			motor2->moveStepNum = 0;
			break;
		}
	}
}

void motor2StopMove(void)
{
	motor2StopMovePermission = true;
}


/****************************************end of file************************************************/

