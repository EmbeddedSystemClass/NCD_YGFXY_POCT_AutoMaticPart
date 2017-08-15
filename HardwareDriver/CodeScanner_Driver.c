/******************************************File Start**********************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include	"CodeScanner_Driver.h"
/***************************************************************************************************/
/******************************************Static Variables*****************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/******************************************Static Methods*******************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/******************************************Main Body************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName：CodeScanner_GPIOInit
*Description：二维码扫描器io初始化
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月24日18:12:44
***************************************************************************************************/
void CodeScanner_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(Trig_Rcc, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = Trig_Pin; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(Trig_GpioGroup, &GPIO_InitStructure);

  	OpenCodeScanner();
}

/***************************************************************************************************
*FunctionName：OpenCodeScanner
*Description：打开二维码扫描器，开始扫描
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月24日18:14:54
***************************************************************************************************/
void OpenCodeScanner(void)
{
	GPIO_WriteBit(Trig_GpioGroup, Trig_Pin, Bit_RESET);
	delay_ms(1);
}
void CloseCodeScanner(void)
{
	GPIO_WriteBit(Trig_GpioGroup, Trig_Pin, Bit_SET);
	delay_ms(1);
}
/****************************************end of file************************************************/
