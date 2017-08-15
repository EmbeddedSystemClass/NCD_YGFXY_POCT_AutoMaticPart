/****************************************file start**************************************************/
#ifndef	_AD_D_H_H
#define	_AD_D_H_H

#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"stm32f4xx_dac.h"

#define	DA_Led_Pin			GPIO_Pin_4
#define	DA_Led_Group		GPIOA
#define	DA_Led_Rcc			RCC_AHB1Periph_GPIOA
#define	DA_Led_Channel		DAC_Channel_1

#define	DA_Line_Pin			GPIO_Pin_5
#define	DA_Line_Group		GPIOA
#define	DA_Line_Rcc			RCC_AHB1Periph_GPIOA
#define	DA_Line_Channel		DAC_Channel_2

void DA_GPIOInit(void);
void SetLedVol(unsigned short volNum);
void SetLineVol(unsigned short volNum);
	
#endif
/****************************************end of file************************************************/

