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
/***************************************************************************************************/
/******************************************Macro Definition*****************************************/
/***************************************************************************************************/
#define	Motor4_CLK_Pin			GPIO_Pin_6
#define	Motor4_CLK_Group		GPIOC
#define	Motor4_CLK_RCC			RCC_AHB1Periph_GPIOC

#define	Motor4_DIR_Pin			GPIO_Pin_7
#define	Motor4_DIR_Group		GPIOC
#define	Motor4_DIR_RCC			RCC_AHB1Periph_GPIOC

#define	Motor4_Sleep_Pin		GPIO_Pin_2
#define	Motor4_Sleep_Group		GPIOG
#define	Motor4_Sleep_RCC		RCC_AHB1Periph_GPIOG

#define	Motor4_Power_Pin		GPIO_Pin_5
#define	Motor4_Power_Group		GPIOG
#define	Motor4_Power_RCC		RCC_AHB1Periph_GPIOG

#define	Motor4_Open_Pin			GPIO_Pin_8
#define	Motor4_Open_Group		GPIOC
#define	Motor4_Open_RCC			RCC_AHB1Periph_GPIOC
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
void setMotor4PowerGPIO(MyBitAction myBitAction);
MyBitAction getMotor4OriginStatus(void);


#endif
/****************************************end of file************************************************/
