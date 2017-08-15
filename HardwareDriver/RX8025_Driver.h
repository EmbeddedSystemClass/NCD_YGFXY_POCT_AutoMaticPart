/****************************************file start**************************************************/
#ifndef	_RX8025_D_H_H
#define	_RX8025_D_H_H

#include	"stm32f4xx_gpio.h"

#include	"Define.h"
#include	"DateTime.h"

#define	RX_SCK_Pin			GPIO_Pin_14
#define	RX_SCK_Group		GPIOE
#define	RX_SCK_Rcc			RCC_AHB1Periph_GPIOE

#define	RX_SDA_Pin			GPIO_Pin_9
#define	RX_SDA_Group		GPIOE
#define	RX_SDA_Rcc			RCC_AHB1Periph_GPIOE

#define RX_SDA_H()	    	GPIO_SetBits(RX_SDA_Group, RX_SDA_Pin)
#define RX_SDA_L()	    	GPIO_ResetBits(RX_SDA_Group, RX_SDA_Pin)
#define RX_SDA_PIN()	    GPIO_ReadInputDataBit(RX_SDA_Group, RX_SDA_Pin)

#define	RX_SCK_H()			GPIO_SetBits(RX_SCK_Group, RX_SCK_Pin)
#define	RX_SCK_L()			GPIO_ResetBits(RX_SCK_Group, RX_SCK_Pin)

#define	RX_DelayTime		5

void RTC_BSPInit(void);
MyRes RTC_SetTimeData(DateTime * data);
MyRes RTC_GetTimeData(DateTime * time);

#endif
/****************************************end of file************************************************/
