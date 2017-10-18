/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Motor1_Fun.h"

#include	"Motor1_Driver.h"
#include	"Timer3_Driver.h"

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
static Motor * motor1 = NULL;
static bool motor1StopMovePermission = false;
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: motor1StartMove
*Description: ���1��ʼ�˶�N��
*Input:  isFront -- �Ƿ���ǰ��
*		stepNum -- �ߵĲ���
*		waitTime -- �ȴ�ʱ��
*Output: 
*Return: 
*Author: xsx
*Date: 2017��8��10�� 14:12:21
***************************************************************************************************/
void motor1MoveToNum(unsigned char num, bool isWait)
{
	motor1 = getMotor(Motor_1);

/*	if(motor1->motorLocation == 0)
		motor1->isFront = true;
	else
	{
		if(motor1->motorLocation == num)
			return;
		else if(motor1->motorLocation > num)
		{
			if(motor1->motorLocation - num <= Motor1_HalfLocation)
				motor1->isFront = false;
			else
				motor1->isFront = true;
		}
		else
		{
			if(num - motor1->motorLocation <= Motor1_HalfLocation)
				motor1->isFront = true;
			else
				motor1->isFront = false;
		}
	}
	
	if(motor1->isFront)
		setMotor1DirGPIO(ON);
	else
		setMotor1DirGPIO(OFF);
	vTaskDelay(10 / portTICK_RATE_MS);
*/	
	motor1->periodCnt = 0;
	motor1->motorTargetLocation = num;
	motor1->moveStepNum = 650000;
	motor1->parm1 = 0;
	motor1StopMovePermission = false;
	
	while(isWait && motor1->motorLocation != motor1->motorTargetLocation)
	{
		vTaskDelay(100 / portTICK_RATE_MS);
		if(motor1StopMovePermission)
		{
			motor1->moveStepNum = 0;
			break;
		}
	}
}

void motor1StopMove(void)
{
	motor1StopMovePermission = true;
}

/****************************************end of file************************************************/

