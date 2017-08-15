#ifndef _MOTOR2_D_H_H
#define _MOTOR2_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#define	Motor2_CLK_Pin			GPIO_Pin_11
#define	Motor2_CLK_Group		GPIOE
#define	Motor2_CLK_RCC			RCC_AHB1Periph_GPIOE

#define	Motor2_DIR_Pin			GPIO_Pin_12
#define	Motor2_DIR_Group		GPIOE
#define	Motor2_DIR_RCC			RCC_AHB1Periph_GPIOE

#define	Motor2_Sleep_Pin		GPIO_Pin_10
#define	Motor2_Sleep_Group		GPIOE
#define	Motor2_Sleep_RCC		RCC_AHB1Periph_GPIOE

//原点传感器
#define	Motor2_Sensor1_Pin				GPIO_Pin_8
#define	Motor2_Sensor1_Group			GPIOD
#define	Motor2_Sensor1_RCC				RCC_AHB1Periph_GPIOD
#define	Motor2_Sensor1_EXTI_PortSource	EXTI_PortSourceGPIOD
#define	Motor2_Sensor1_EXTI_PinSource	EXTI_PinSource8
#define	Motor2_Sensor1_EXTI_Line		EXTI_Line8
#define	Motor2_Sensor1_EXTI_IRQ			EXTI9_5_IRQn

//中间传感器
#define	Motor2_Sensor2_Pin				GPIO_Pin_15
#define	Motor2_Sensor2_Group			GPIOB
#define	Motor2_Sensor2_RCC				RCC_AHB1Periph_GPIOB
#define	Motor2_Sensor2_EXTI_PortSource	EXTI_PortSourceGPIOB
#define	Motor2_Sensor2_EXTI_PinSource	EXTI_PinSource15
#define	Motor2_Sensor2_EXTI_Line		EXTI_Line15
#define	Motor2_Sensor2_EXTI_IRQ			EXTI15_10_IRQn

//最外面传感器
#define	Motor2_Sensor3_Pin				GPIO_Pin_14
#define	Motor2_Sensor3_Group			GPIOB
#define	Motor2_Sensor3_RCC				RCC_AHB1Periph_GPIOB
#define	Motor2_Sensor3_EXTI_PortSource	EXTI_PortSourceGPIOB
#define	Motor2_Sensor3_EXTI_PinSource	EXTI_PinSource14
#define	Motor2_Sensor3_EXTI_Line		EXTI_Line14
#define	Motor2_Sensor3_EXTI_IRQ			EXTI15_10_IRQn

#define	Motor2Sensor1Triggered	!GPIO_ReadInputDataBit(Motor2_Sensor1_Group, Motor2_Sensor1_Pin)
#define	Motor2Sensor2Triggered	!GPIO_ReadInputDataBit(Motor2_Sensor2_Group, Motor2_Sensor2_Pin)
#define	Motor2Sensor3Triggered	!GPIO_ReadInputDataBit(Motor2_Sensor3_Group, Motor2_Sensor3_Pin)

void Motor2_GPIO_Init(void);
void setMotor2ClkGPIO(MyBitAction myBitAction);
void setMotor2DirGPIO(MyBitAction myBitAction);
void setMotor2SleepGPIO(MyBitAction myBitAction);

#endif

