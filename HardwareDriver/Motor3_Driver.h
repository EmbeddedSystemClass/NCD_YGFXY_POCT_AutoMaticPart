#ifndef _MOTOR3_D_H_H
#define _MOTOR3_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#define	Motor3_CLK_Pin			GPIO_Pin_1
#define	Motor3_CLK_Group		GPIOB
#define	Motor3_CLK_RCC			RCC_AHB1Periph_GPIOB

#define	Motor3_DIR_Pin			GPIO_Pin_2
#define	Motor3_DIR_Group		GPIOB
#define	Motor3_DIR_RCC			RCC_AHB1Periph_GPIOB

#define	Motor3_Sleep_Pin		GPIO_Pin_0
#define	Motor3_Sleep_Group		GPIOB
#define	Motor3_Sleep_RCC		RCC_AHB1Periph_GPIOB

void Motor3_GPIO_Init(void);
void setMotor3ClkGPIO(MyBitAction myBitAction);
void setMotor3DirGPIO(MyBitAction myBitAction);
void setMotor3SleepGPIO(MyBitAction myBitAction);

#endif

