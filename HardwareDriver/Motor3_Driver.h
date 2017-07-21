#ifndef _MOTOR3_D_H_H
#define _MOTOR3_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#define	Motor3_CLK_Pin			GPIO_Pin_7
#define	Motor3_CLK_Group		GPIOE
#define	Motor3_CLK_RCC			RCC_AHB1Periph_GPIOE

#define	Motor3_DIR_Pin			GPIO_Pin_8
#define	Motor3_DIR_Group		GPIOE
#define	Motor3_DIR_RCC			RCC_AHB1Periph_GPIOE

#define	Motor3_Sleep_Pin		GPIO_Pin_5
#define	Motor3_Sleep_Group		GPIOE
#define	Motor3_Sleep_RCC		RCC_AHB1Periph_GPIOE

#define	Motor3_Origin_Pin		GPIO_Pin_13
#define	Motor3_Origin_Group		GPIOE
#define	Motor3_Origin_RCC		RCC_AHB1Periph_GPIOE

void Motor3_GPIO_Init(void);
void setMotor3ClkGPIO(MyBitAction myBitAction);
void setMotor3DirGPIO(MyBitAction myBitAction);
void setMotor3SleepGPIO(MyBitAction myBitAction);
MyBitAction getMotor3OriginStatus(void);

#endif

