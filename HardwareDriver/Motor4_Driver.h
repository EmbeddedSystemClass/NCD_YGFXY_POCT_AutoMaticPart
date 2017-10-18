/******************************************File Start**********************************************/
#ifndef _MOTOR4_D_H_H
#define _MOTOR4_D_H_H

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#if (Motor4Type == Motor4IOMotor)
/***************************************************************************************************/
/******************************************Macro Definition*****************************************/
/***************************************************************************************************/
#define	Motor4_CLK_Pin			GPIO_Pin_15
#define	Motor4_CLK_Group		GPIOE
#define	Motor4_CLK_RCC			RCC_AHB1Periph_GPIOE

#define	Motor4_DIR_Pin			GPIO_Pin_8
#define	Motor4_DIR_Group		GPIOE
#define	Motor4_DIR_RCC			RCC_AHB1Periph_GPIOE

#define	Motor4_Sleep_Pin		GPIO_Pin_9
#define	Motor4_Sleep_Group		GPIOE
#define	Motor4_Sleep_RCC		RCC_AHB1Periph_GPIOE

#define	Motor4_Sensor1_Pin		GPIO_Pin_0
#define	Motor4_Sensor1_Group	GPIOB
#define	Motor4_Sensor1_RCC		RCC_AHB1Periph_GPIOB

#define	Motor4Sensor1Triggered	GPIO_ReadInputDataBit(Motor4_Sensor1_Group, Motor4_Sensor1_Pin)
/***************************************************************************************************/
/***************************************************************************************************/
/********************************************Methods************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
void Motor4_GPIO_Init(void);
void setMotor4ClkGPIO(MyBitAction myBitAction);
void setMotor4DirGPIO(MyBitAction myBitAction);
void setMotor4SleepGPIO(MyBitAction myBitAction);
MyBitAction getMotor4OriginStatus(void);

#endif
#endif
/****************************************end of file************************************************/
