#ifndef CS_D_H_H
#define CS_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Delay.h"

#define	Trig_Pin			GPIO_Pin_9
#define	Trig_GpioGroup		GPIOC
#define	Trig_Rcc			RCC_AHB1Periph_GPIOC


void CodeScanner_GPIO_Init(void);
void OpenCodeScanner(void);
void CloseCodeScanner(void);

#endif


