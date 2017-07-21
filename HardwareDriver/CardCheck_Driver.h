#ifndef CARDCHECK_D_H_H
#define CARDCHECK_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"

#define	CardIn_Pin				GPIO_Pin_9
#define	CardIn_Group			GPIOB
#define	CardIn_RCC				RCC_AHB1Periph_GPIOB

#define	CardPinIn 				GPIO_ReadInputDataBit(CardIn_Group, CardIn_Pin)


void CardCheck_GPIO_Init(void);


#endif


