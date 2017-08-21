/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Channel_Driver.h"
/***************************************************************************************************/
/******************************************Static Variables***************************************/
/***************************************************************************************************/
static unsigned char channelNum = 0;
/***************************************************************************************************/
/******************************************Static Methods***************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName: Channel_GPIOInit
*Description: 通道选择GPIO初始化
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年8月7日 09:01:07
***************************************************************************************************/
void Channel_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(ChannelA_Rcc | ChannelB_Rcc | ChannelC_Rcc, ENABLE); /*使能LED灯使用的GPIO时钟*/

  	GPIO_InitStructure.GPIO_Pin = ChannelA_Pin; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ChannelA_GpioGroup, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ChannelB_Pin;
	GPIO_Init(ChannelB_GpioGroup, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ChannelC_Pin;
	GPIO_Init(ChannelC_GpioGroup, &GPIO_InitStructure);
}

void SelectChannel(unsigned char index)
{
	if(index > 7)
		index = 7;
	
	if(index&0x04)
		GPIO_WriteBit(ChannelA_GpioGroup, ChannelA_Pin, Bit_SET);
	else
		GPIO_WriteBit(ChannelA_GpioGroup, ChannelA_Pin, Bit_RESET);
	
	if(index&0x02)
		GPIO_WriteBit(ChannelB_GpioGroup, ChannelB_Pin, Bit_SET);
	else
		GPIO_WriteBit(ChannelB_GpioGroup, ChannelB_Pin, Bit_RESET);
	
	if(index&0x01)
		GPIO_WriteBit(ChannelC_GpioGroup, ChannelC_Pin, Bit_SET);
	else
		GPIO_WriteBit(ChannelC_GpioGroup, ChannelC_Pin, Bit_RESET);
	
	channelNum = index;
}

unsigned char getChannelNum(void)
{
	return channelNum;
}

/****************************************end of file************************************************/
