#ifndef CARDCHECK_D_H_H
#define CARDCHECK_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#define	CardCheck_Pin				GPIO_Pin_3
#define	CardCheck_Group				GPIOD
#define	CardCheck_RCC				RCC_AHB1Periph_GPIOD

#define	ReadCardCheckPin 			GPIO_ReadInputDataBit(CardCheck_Group, CardCheck_Pin)


void CardCheck_GPIO_Init(void);
MyBitAction readCaedCheckStatus(void);

#endif


