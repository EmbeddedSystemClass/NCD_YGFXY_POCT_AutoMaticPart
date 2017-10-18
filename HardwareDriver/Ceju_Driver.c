/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Ceju_Driver.h"
/***************************************************************************************************/
/******************************************Static Variables***************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/******************************************Static Methods***************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
void  CejuADC_Init(void)
{    
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(CejuADC_Gpio_Rcc, ENABLE);
	RCC_APB2PeriphClockCmd(CejuADC_Rcc, ENABLE);

	GPIO_InitStructure.GPIO_Pin = CejuADC_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(CejuADC_Group, &GPIO_InitStructure);

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(CejuADC, &ADC_InitStructure);

	ADC_Cmd(CejuADC, ENABLE);
}

unsigned short readCejuADC(void)
{
	ADC_RegularChannelConfig(CejuADC, CejuADC_Channel, 1, ADC_SampleTime_480Cycles);
	ADC_SoftwareStartConv(CejuADC);
	
	while(!ADC_GetFlagStatus(CejuADC, ADC_FLAG_EOC));
	return ADC_GetConversionValue(CejuADC);
}

unsigned short readJuliValue(void)
{
	unsigned char i=0;
	unsigned int value = 0;
	
	for(i=0; i<50; i++)
		value += readCejuADC();
	
	value /= 50;
	
	return value;
}


void stopCejuADCFun(void)
{
	ADC_Cmd(CejuADC, DISABLE);
}
/****************************************end of file************************************************/
