/****************************************file start************************************************/
#ifndef	_CEJU_D_H_H
#define	_CEJU_D_H_H

#include	"stm32f4xx_adc.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"

#define	CejuADC_Pin			GPIO_Pin_2
#define	CejuADC_Group		GPIOC
#define	CejuADC_Gpio_Rcc	RCC_AHB1Periph_GPIOC
#define	CejuADC				ADC1
#define	CejuADC_Rcc			RCC_APB2Periph_ADC1
#define	CejuADC_Channel		ADC_Channel_12


void  CejuADC_Init(void);
unsigned short ChangeValueToVol(void);
void stopCejuADCFun(void);
#endif
/****************************************end of file***********************************************/
