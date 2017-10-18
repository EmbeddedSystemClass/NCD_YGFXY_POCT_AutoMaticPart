#ifndef _MOTOR2_D_H_H
#define _MOTOR2_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"stm32f4xx_exti.h"
#include 	"stm32f4xx_syscfg.h"
#include	"misc.h"
#include	"Define.h"

#define	Motor2_CLK_Pin			GPIO_Pin_11
#define	Motor2_CLK_Group		GPIOE
#define	Motor2_CLK_RCC			RCC_AHB1Periph_GPIOE

#define	Motor2_DIR_Pin			GPIO_Pin_10
#define	Motor2_DIR_Group		GPIOE
#define	Motor2_DIR_RCC			RCC_AHB1Periph_GPIOE

#define	Motor2_Sleep_Pin		GPIO_Pin_7
#define	Motor2_Sleep_Group		GPIOE
#define	Motor2_Sleep_RCC		RCC_AHB1Periph_GPIOE

//中间传感器
#define	Motor2_Sensor2_Pin				GPIO_Pin_15
#define	Motor2_Sensor2_Group			GPIOB
#define	Motor2_Sensor2_RCC				RCC_AHB1Periph_GPIOB
#define	Motor2_Sensor2_EXTI_PortSource	EXTI_PortSourceGPIOB
#define	Motor2_Sensor2_EXTI_PinSource	EXTI_PinSource15
#define	Motor2_Sensor2_EXTI_Line		EXTI_Line15
#define	Motor2_Sensor2_EXTI_IRQ			EXTI15_10_IRQn

#define	Motor2Sensor2Triggered	!GPIO_ReadInputDataBit(Motor2_Sensor2_Group, Motor2_Sensor2_Pin)

void Motor2_GPIO_Init(void);
void setMotor2ClkGPIO(MyBitAction myBitAction);
void setMotor2DirGPIO(MyBitAction myBitAction);
void setMotor2SleepGPIO(MyBitAction myBitAction);

#endif

