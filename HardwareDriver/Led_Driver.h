#ifndef LED_D_H_H
#define LED_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#define	Led_Pin			GPIO_Pin_9
#define	Led_GpioGroup	GPIOF
#define	Led_Rcc			RCC_AHB1Periph_GPIOF


void Led_GPIOInit(void);
void SetLedState(MyBitAction state);
void LedToggle(void);

#endif


