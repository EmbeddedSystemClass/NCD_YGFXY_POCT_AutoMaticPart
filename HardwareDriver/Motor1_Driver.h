#ifndef _MOTOR1_D_H_H
#define _MOTOR1_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#define	Motor1_CLK_Pin			GPIO_Pin_13
#define	Motor1_CLK_Group		GPIOF
#define	Motor1_CLK_RCC			RCC_AHB1Periph_GPIOF

#define	Motor1_DIR_Pin			GPIO_Pin_13
#define	Motor1_DIR_Group		GPIOF
#define	Motor1_DIR_RCC			RCC_AHB1Periph_GPIOF

#define	Motor1_Sleep_Pin		GPIO_Pin_13
#define	Motor1_Sleep_Group		GPIOF
#define	Motor1_Sleep_RCC		RCC_AHB1Periph_GPIOF

void Motor1_GPIO_Init(void);
void setMotor1ClkGPIO(MyBitAction myBitAction);
void setMotor1DirGPIO(MyBitAction myBitAction);
void setMotor1SleepGPIO(MyBitAction myBitAction);

#endif

