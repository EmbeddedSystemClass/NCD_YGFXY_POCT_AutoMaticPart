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
unsigned char motorStep = 0;
/**************************************************************************************************/
/******************************************Static Methods******************************************/
/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
/****************************************File Start************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

void MotorMoveToWaitCardPutIn(unsigned char num)
{
	motor4MoveTo(Motor4_OpenLocation, true);
	
	if(num != getMotorxLocation(Motor_1))
	{
		motor2MoveTo(Motor2_MidLocation, true);
	
		motor1MoveToNum(num, true);
	}
	motor2MoveTo(Motor2_WaitCardLocation, true);
}

void MotorMoveToStartTestLocation(unsigned char num)
{
	motor4MoveTo(Motor4_OpenLocation, true);
	motor2MoveTo(Motor2_MidLocation, true);
	motor1MoveToNum(num, true);
	motor2MoveTo(Motor2_CatchCardLocation, true);
	motor4MoveTo(Motor4_CardLocation, true);
	motor2MoveTo(Motor2_StartTestLocation, true);
}

void PutCardOutOfDevice(void)
{
	motor4MoveTo(Motor4_OpenLocation, false);
	motor2MoveTo(0, true);
	motor2MoveTo(Motor2_MidLocation, true);
}

void MotorMoveToOriginLocation(unsigned char num)
{
	motor4MoveTo(Motor4_OpenLocation, false);
	motor2MoveTo(Motor2_MidLocation, true);
	
	if(num != getMotorxLocation(Motor_1))
		motor1MoveToNum(num, true);
}

/****************************************end of file***********************************************/
