/******************************************File Start**********************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include	"Timer3_Driver.h"
#include	"Motor1_Driver.h"
#include	"Motor2_Driver.h"
#include	"Motor3_Driver.h"
#include	"Motor4_Driver.h"
#include	"Motor_Data.h"
#include	"Led_Driver.h"

#include	"stdio.h"
/***************************************************************************************************/
/******************************************Static Variables*****************************************/
/***************************************************************************************************/
static Motor * motor = NULL;
/***************************************************************************************************/
/******************************************Static Methods*******************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/******************************************Main Body************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName：Timer_Init
*Description：定时器3初始化，这里设置成1S中断
*Input：None
*Output：None
*Author：xsx
*Data：2016年5月14日15:48:03
***************************************************************************************************/
void Timer3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = 100;									//reload value 100
	TIM_TimeBaseInitStructure.TIM_Prescaler= (84 - 1);  						//84prescaler, 84MHZ/84 = 1us
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x04;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	assert_param(IS_TIM_ALL_PERIPH(TIM3)); 
}


void TIM3_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		motor = getMotor(Motor_1);
		if(motor->moveStepNum > 0)
		{
			plusMotorxPeriodCnt(Motor_1);
			
			if(motor->periodCnt <= motor->highTime)
				setMotor1ClkGPIO(ON);
			else if(motor->periodCnt < motor->lowTime)
				setMotor1ClkGPIO(OFF);
			else if(motor->periodCnt == motor->lowTime)
			{
				setMotorxPeriodCnt(Motor_1, 0);
				setMotor1ClkGPIO(OFF);
				minusMotorxMoveStepNum(Motor_1);
				plusMotorxLocation(Motor_1, 1);
			}
		}

		motor = getMotor(Motor_2);
		if(motor->moveStepNum > 0)
		{
			plusMotorxPeriodCnt(Motor_2);
			
			if(motor->periodCnt <= motor->highTime)
				setMotor2ClkGPIO(ON);
			else if(motor->periodCnt < motor->lowTime)
				setMotor2ClkGPIO(OFF);
			else if(motor->periodCnt == motor->lowTime)
			{
				setMotorxPeriodCnt(Motor_2, 0);
				setMotor2ClkGPIO(OFF);
				minusMotorxMoveStepNum(Motor_2);
				plusMotorxLocation(Motor_2, 1);
			}
		}
	
		motor = getMotor(Motor_3);
		if(motor->moveStepNum > 0)
		{
			plusMotorxPeriodCnt(Motor_3);
			
			if(motor->periodCnt <= motor->highTime)
				setMotor3ClkGPIO(ON);
			else if(motor->periodCnt < motor->lowTime)
				setMotor3ClkGPIO(OFF);
			else if(motor->periodCnt == motor->lowTime)
			{
				setMotorxPeriodCnt(Motor_3, 0);
				setMotor3ClkGPIO(OFF);
				minusMotorxMoveStepNum(Motor_3);
				plusMotorxLocation(Motor_3, 1);
			}
			
			//传感器触发,且是后退，则停止运动，置位置为0点
			if((getMotorxDir(Motor_3) == false) && (ON == getMotor3OriginStatus()))
			{
				setMotorxMoveStepNum(Motor_3, 0);
				setMotorxLocation(Motor_3, 0);
			}
		}
		
		motor = getMotor(Motor_4);
		if(motor->moveStepNum > 0)
		{
			plusMotorxPeriodCnt(Motor_4);
			
			if(motor->periodCnt <= motor->highTime)
				setMotor4ClkGPIO(ON);
			else if(motor->periodCnt < motor->lowTime)
				setMotor4ClkGPIO(OFF);
			else if(motor->periodCnt == motor->lowTime)
			{
				setMotorxPeriodCnt(Motor_4, 0);
				setMotor4ClkGPIO(OFF);
				minusMotorxMoveStepNum(Motor_4);
				plusMotorxLocation(Motor_4, 1);
			}
			
			//如果是打开过程，检测到最大位置，则停止，记为原点
			if((motor->isFront == false) && (ON == getMotor4OriginStatus()))
			{
				setMotorxParm1(Motor_4, false);
				setMotorxMoveStepNum(Motor_4, 0);
				setMotorxLocation(Motor_4, 0);
			}
		}
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
/****************************************end of file************************************************/
