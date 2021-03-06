/****************************************file start************************************************/
#ifndef	_CEJU_D_H_H
#define	_CEJU_D_H_H

#include	"stm32f4xx_adc.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"

#define	CejuADC_Pin			GPIO_Pin_0
#define	CejuADC_Group		GPIOA
#define	CejuADC_Gpio_Rcc	RCC_AHB1Periph_GPIOA
#define	CejuADC				ADC2
#define	CejuADC_Rcc			RCC_APB2Periph_ADC2
#define	CejuADC_Channel		ADC_Channel_0

typedef struct
{
	unsigned short data[30];
	unsigned short data2[30];
	unsigned char i;
	double value;
	float baseCV;
	float cv;
}CejuStruct; 

#define	CejuStructSize	sizeof(CejuStruct)

void  CejuADC_Init(void);
void stopCejuADCFun(void);
unsigned short readJuliValue(void);
unsigned short readCejuADC(void);
unsigned short readJuliValueWithFilter(void);

#endif
/****************************************end of file***********************************************/
