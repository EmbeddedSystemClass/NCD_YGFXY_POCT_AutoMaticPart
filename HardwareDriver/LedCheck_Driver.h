#ifndef LEDCHECK_D_H_H
#define LEDCHECK_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#define	LedCheck_Pin				GPIO_Pin_2
#define	LedCheck_Group				GPIOE
#define	LedCheck_RCC				RCC_AHB1Periph_GPIOE

#define	ReadLedCheckPin 			GPIO_ReadInputDataBit(LedCheck_Group, LedCheck_Pin)


void LedCheck_GPIO_Init(void);
MyBitAction readLedCheckStatus(void);

#endif


