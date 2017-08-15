#ifndef _MOTOR1_D_H_H
#define _MOTOR1_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#define	Motor1_CLK_Pin					GPIO_Pin_9
#define	Motor1_CLK_Group				GPIOB
#define	Motor1_CLK_RCC					RCC_AHB1Periph_GPIOB

#define	Motor1_DIR_Pin					GPIO_Pin_0
#define	Motor1_DIR_Group				GPIOE
#define	Motor1_DIR_RCC					RCC_AHB1Periph_GPIOE

#define	Motor1_Sleep_Pin				GPIO_Pin_8
#define	Motor1_Sleep_Group				GPIOB
#define	Motor1_Sleep_RCC				RCC_AHB1Periph_GPIOB

//原点传感器
#define	Motor1_Sensor1_Pin				GPIO_Pin_5
#define	Motor1_Sensor1_Group			GPIOB
#define	Motor1_Sensor1_RCC				RCC_AHB1Periph_GPIOB
#define	Motor1_Sensor1_EXTI_PortSource	EXTI_PortSourceGPIOB
#define	Motor1_Sensor1_EXTI_PinSource	EXTI_PinSource5
#define	Motor1_Sensor1_EXTI_Line		EXTI_Line5
#define	Motor1_Sensor1_EXTI_IRQ			EXTI9_5_IRQn

//一格传感器
#define	Motor1_Sensor2_Pin				GPIO_Pin_6
#define	Motor1_Sensor2_Group			GPIOB
#define	Motor1_Sensor2_RCC				RCC_AHB1Periph_GPIOB
#define	Motor1_Sensor2_EXTI_PortSource	EXTI_PortSourceGPIOB
#define	Motor1_Sensor2_EXTI_PinSource	EXTI_PinSource6
#define	Motor1_Sensor2_EXTI_Line		EXTI_Line6
#define	Motor1_Sensor2_EXTI_IRQ			EXTI9_5_IRQn

#define	Motor1Sensor1Triggered	!GPIO_ReadInputDataBit(Motor1_Sensor1_Group, Motor1_Sensor1_Pin)
#define	Motor1Sensor2Triggered	GPIO_ReadInputDataBit(Motor1_Sensor2_Group, Motor1_Sensor2_Pin)

void Motor1_GPIO_Init(void);
void setMotor1ClkGPIO(MyBitAction myBitAction);
void setMotor1DirGPIO(MyBitAction myBitAction);
void setMotor1SleepGPIO(MyBitAction myBitAction);

#endif

