/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Motor4_Fun.h"

#include	"Motor4_Driver.h"
#include	"Usart2_Driver.h"
#include	"Timer3_Driver.h"

#include	"QueueUnits.h"
#include	"MyMem.h"
#include	"CRC16.h"
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
#if(Motor4Type == Motor4UsartMotor)

	static unsigned char motor4TxBuf[8];
	static unsigned char motor4RxBuf[8];
	static unsigned short motorCrc = 0;
	static unsigned short tempCrc = 0;
	static xSemaphoreHandle xMutex;									//»¥³âÁ¿

#elif(Motor4Type == Motor4IOMotor)

	static Motor * motor4 = NULL;

#endif
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
#if(Motor4Type == Motor4UsartMotor)

MyRes motor4Reset(void)
{
	motor4TxBuf[0] = 1;
	motor4TxBuf[1] = 6;
	motor4TxBuf[2] = 0;
	motor4TxBuf[3] = 1;
	motor4TxBuf[4] = 0;
	motor4TxBuf[5] = 0;
	motorCrc = CalModbusCRC16Fun(motor4TxBuf, 6, NULL);
	motor4TxBuf[6] = motorCrc>>8;
	motor4TxBuf[7] = motorCrc;
	
	memset(motor4RxBuf, 0, 8);
	xQueueReset(GetUsart2RXQueue());
	SendDataToQueue(GetUsart2TXQueue(), NULL, motor4TxBuf, 8, 1, 100 / portTICK_RATE_MS, 0, EnableUsart2TXInterrupt);

	if(My_Pass == ReceiveDataFromQueue(GetUsart2RXQueue(), NULL, motor4RxBuf, 8, NULL, 1, 2000 / portTICK_RATE_MS, 0))
	{
		motorCrc = motor4RxBuf[6];
		motorCrc <<= 8;
		motorCrc += motor4RxBuf[7];
		tempCrc = CalModbusCRC16Fun(motor4RxBuf, 6, NULL);
		if(motorCrc == tempCrc)
		{
			return My_Pass;
		}
	}
	
	return My_Fail;
}

void motor4MoveTo(unsigned short location, bool isWait)
{
	unsigned char i=3;
	motor4TxBuf[0] = 1;
	motor4TxBuf[1] = 6;
	motor4TxBuf[2] = 0;
	motor4TxBuf[3] = 2;
	motor4TxBuf[4] = 0;
	motor4TxBuf[5] = location & 0xff;
	motorCrc = CalModbusCRC16Fun(motor4TxBuf, 6, NULL);
	motor4TxBuf[6] = motorCrc>>8;
	motor4TxBuf[7] = motorCrc;
	
	while(i--)
	{
		memset(motor4RxBuf, 0, 8);
		xQueueReset(GetUsart2RXQueue());
		SendDataToQueue(GetUsart2TXQueue(), NULL, motor4TxBuf, 8, 1, 100 / portTICK_RATE_MS, 0, EnableUsart2TXInterrupt);
		
		if(My_Pass == ReceiveDataFromQueue(GetUsart2RXQueue(), NULL, motor4RxBuf, 8, NULL, 1, 3000 / portTICK_RATE_MS, 0))
		{
			motorCrc = motor4RxBuf[6];
			motorCrc <<= 8;
			motorCrc += motor4RxBuf[7];
			tempCrc = CalModbusCRC16Fun(motor4RxBuf, 6, NULL);
			if(motorCrc == tempCrc)
			{
				return;
			}
		}
	}
}

unsigned short readMotorLocation(void)
{
	motor4TxBuf[0] = 1;
	motor4TxBuf[1] = 3;
	motor4TxBuf[2] = 0;
	motor4TxBuf[3] = 5;
	motor4TxBuf[4] = 0;
	motor4TxBuf[5] = 1;
	motorCrc = CalModbusCRC16Fun(motor4TxBuf, 6, NULL);
	motor4TxBuf[6] = motorCrc>>8;
	motor4TxBuf[7] = motorCrc;
	
	memset(motor4RxBuf, 0, 8);
	xQueueReset(GetUsart2RXQueue());
	SendDataToQueue(GetUsart2TXQueue(), NULL, motor4TxBuf, 8, 1, 100 / portTICK_RATE_MS, 0, EnableUsart2TXInterrupt);

	if(My_Pass == ReceiveDataFromQueue(GetUsart2RXQueue(), NULL, motor4TxBuf, 8, NULL, 1, 500 / portTICK_RATE_MS, 0))
	{
		motorCrc = motor4TxBuf[6];
		motorCrc <<= 8;
		motorCrc += motor4TxBuf[7];
		tempCrc = CalModbusCRC16Fun(motor4TxBuf, 6, NULL);
		if(motorCrc == tempCrc)
		{
			motorCrc = motor4TxBuf[4];
			motorCrc <<= 8;
			motorCrc += motor4TxBuf[5];

			return motorCrc;
		}
	}
	
	return 0;
}

#elif(Motor4Type == Motor4IOMotor)

void motor4MoveTo(unsigned short location, bool isWait)
{
	motor4 = getMotor(Motor_4);

	if(location == 0)
		motor4->motorLocation = 60000;
	
	if(motor4->motorLocation == location)
		return;
	else if(motor4->motorLocation > location)
		motor4->isFront = false;
	else
		motor4->isFront = true;

	if(motor4->isFront)
		setMotor4DirGPIO(ON);
	else
		setMotor4DirGPIO(OFF);
	delay_ms(5);
	
	motor4->periodCnt = 0;
	motor4->motorTargetLocation = location;
	motor4->moveStepNum = 65000;
	
	while(isWait && motor4->motorLocation != motor4->motorTargetLocation)
		vTaskDelay(100 / portTICK_RATE_MS);
}

#endif



/****************************************end of file************************************************/

