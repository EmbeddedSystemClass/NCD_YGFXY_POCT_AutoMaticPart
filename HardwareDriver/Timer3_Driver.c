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
*FunctionName��Timer_Init
*Description����ʱ��3��ʼ�����������ó�1S�ж�
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��14��15:48:03
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
}


void TIM3_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		if(getMotorxMoveStepNum(Motor_1) > 0)
		{
			plusMotorxPeriodCnt(Motor_1);
			
			if(getMotorxPeriodCnt(Motor_1) <= getMotorxHighTime(Motor_1))
				setMotor1ClkGPIO(ON);
			else if(getMotorxPeriodCnt(Motor_1) < getMotorxLowTime(Motor_1))
				setMotor1ClkGPIO(OFF);
			else if(getMotorxPeriodCnt(Motor_1) == getMotorxLowTime(Motor_1))
			{
				setMotorxPeriodCnt(Motor_1, 0);
				setMotor1ClkGPIO(OFF);
				minusMotorxMoveStepNum(Motor_1);
				plusMotorxLocation(Motor_1, 1);
			}
		}
		
		if(getMotorxMoveStepNum(Motor_2) > 0)
		{
			plusMotorxPeriodCnt(Motor_2);
			
			if(getMotorxPeriodCnt(Motor_2) <= getMotorxHighTime(Motor_2))
				setMotor2ClkGPIO(ON);
			else if(getMotorxPeriodCnt(Motor_2) < getMotorxLowTime(Motor_2))
				setMotor2ClkGPIO(OFF);
			else if(getMotorxPeriodCnt(Motor_2) == getMotorxLowTime(Motor_2))
			{
				setMotorxPeriodCnt(Motor_2, 0);
				setMotor2ClkGPIO(OFF);
				minusMotorxMoveStepNum(Motor_2);
				plusMotorxLocation(Motor_2, 1);
			}
		}
	
		if(getMotorxMoveStepNum(Motor_3) > 0)
		{
			plusMotorxPeriodCnt(Motor_3);
			
			if(getMotorxPeriodCnt(Motor_3) <= getMotorxHighTime(Motor_3))
				setMotor3ClkGPIO(ON);
			else if(getMotorxPeriodCnt(Motor_3) < getMotorxLowTime(Motor_3))
				setMotor3ClkGPIO(OFF);
			else if(getMotorxPeriodCnt(Motor_3) == getMotorxLowTime(Motor_3))
			{
				setMotorxPeriodCnt(Motor_3, 0);
				setMotor3ClkGPIO(OFF);
				minusMotorxMoveStepNum(Motor_3);
				plusMotorxLocation(Motor_3, 1);
			}
			
			//����������,���Ǻ��ˣ���ֹͣ�˶�����λ��Ϊ0��
			if((getMotorxDir(Motor_3) == false) && (ON == getMotor3OriginStatus()))
			{
				setMotorxMoveStepNum(Motor_3, 0);
				setMotorxLocation(Motor_3, 0);
			}
		}
		
		if(getMotorxMoveStepNum(Motor_4) > 0)
		{
			plusMotorxPeriodCnt(Motor_4);
			
			if(getMotorxPeriodCnt(Motor_4) <= getMotorxHighTime(Motor_4))
				setMotor4ClkGPIO(ON);
			else if(getMotorxPeriodCnt(Motor_4) < getMotorxLowTime(Motor_4))
				setMotor4ClkGPIO(OFF);
			else if(getMotorxPeriodCnt(Motor_4) == getMotorxLowTime(Motor_4))
			{
				setMotorxPeriodCnt(Motor_4, 0);
				setMotor4ClkGPIO(OFF);
				minusMotorxMoveStepNum(Motor_4);
				plusMotorxLocation(Motor_4, 1);
			}
			
			//����Ǵ򿪹��̣���⵽���λ�ã���ֹͣ����Ϊԭ��
			if(getMotorxParm1(Motor_4) == true)
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
