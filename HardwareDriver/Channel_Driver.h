#ifndef CHANNEL_D_H_H
#define CHANNEL_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"Define.h"

#define	ChannelA_Pin			GPIO_Pin_5
#define	ChannelA_GpioGroup		GPIOE
#define	ChannelA_Rcc			RCC_AHB1Periph_GPIOE

#define	ChannelB_Pin			GPIO_Pin_4
#define	ChannelB_GpioGroup		GPIOE
#define	ChannelB_Rcc			RCC_AHB1Periph_GPIOE

#define	ChannelC_Pin			GPIO_Pin_3
#define	ChannelC_GpioGroup		GPIOE
#define	ChannelC_Rcc			RCC_AHB1Periph_GPIOE


void Channel_GPIOInit(void);
void SelectChannel(unsigned char index);

#endif


