#ifndef _MOTOR1_D_H_H
#define _MOTOR1_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"stm32f4xx_exti.h"
#include 	"stm32f4xx_syscfg.h"
#include	"misc.h"
#include	"Define.h"

#define	Motor1_CLK_Pin					GPIO_Pin_12
#define	Motor1_CLK_Group				GPIOE
#define	Motor1_CLK_RCC					RCC_AHB1Periph_GPIOE

#define	Motor1_DIR_Pin					GPIO_Pin_13
#define	Motor1_DIR_Group				GPIOE
#define	Motor1_DIR_RCC					RCC_AHB1Periph_GPIOE

#define	Motor1_Sleep_Pin				GPIO_Pin_14
#define	Motor1_Sleep_Group				GPIOE
#define	Motor1_Sleep_RCC				RCC_AHB1Periph_GPIOE

//测试位置号位传感器
#define	Motor1_Sensor1_Pin				GPIO_Pin_11
#define	Motor1_Sensor1_Group			GPIOD
#define	Motor1_Sensor1_RCC				RCC_AHB1Periph_GPIOD
#define	Motor1_Sensor1_EXTI_PortSource	EXTI_PortSourceGPIOD
#define	Motor1_Sensor1_EXTI_PinSource	EXTI_PinSource11
#define	Motor1_Sensor1_EXTI_Line		EXTI_Line11
#define	Motor1_Sensor1_EXTI_IRQ			EXTI15_10_IRQn

//1号位传感器
#define	Motor1_Sensor2_Pin				GPIO_Pin_12
#define	Motor1_Sensor2_Group			GPIOD
#define	Motor1_Sensor2_RCC				RCC_AHB1Periph_GPIOD
#define	Motor1_Sensor2_EXTI_PortSource	EXTI_PortSourceGPIOD
#define	Motor1_Sensor2_EXTI_PinSource	EXTI_PinSource12
#define	Motor1_Sensor2_EXTI_Line		EXTI_Line12
#define	Motor1_Sensor2_EXTI_IRQ			EXTI15_10_IRQn

#define	Motor1Sensor1Triggered	!GPIO_ReadInputDataBit(Motor1_Sensor1_Group, Motor1_Sensor1_Pin)
#define	Motor1Sensor2Triggered	!GPIO_ReadInputDataBit(Motor1_Sensor2_Group, Motor1_Sensor2_Pin)

void Motor1_GPIO_Init(void);
void setMotor1ClkGPIO(MyBitAction myBitAction);
void setMotor1DirGPIO(MyBitAction myBitAction);
void setMotor1SleepGPIO(MyBitAction myBitAction);

#endif

