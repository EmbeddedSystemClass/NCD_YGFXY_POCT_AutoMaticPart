#ifndef _MOTOR2_D_H_H
#define _MOTOR2_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#define	Motor2_CLK_Pin			GPIO_Pin_14
#define	Motor2_CLK_Group		GPIOF
#define	Motor2_CLK_RCC			RCC_AHB1Periph_GPIOF

#define	Motor2_DIR_Pin			GPIO_Pin_13
#define	Motor2_DIR_Group		GPIOF
#define	Motor2_DIR_RCC			RCC_AHB1Periph_GPIOF

#define	Motor2_Sleep_Pin		GPIO_Pin_13
#define	Motor2_Sleep_Group		GPIOF
#define	Motor2_Sleep_RCC		RCC_AHB1Periph_GPIOF

void Motor2_GPIO_Init(void);
void setMotor2ClkGPIO(MyBitAction myBitAction);
void setMotor2DirGPIO(MyBitAction myBitAction);
void setMotor2SleepGPIO(MyBitAction myBitAction);

#endif

