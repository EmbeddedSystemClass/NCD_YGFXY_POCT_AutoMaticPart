/****************************************file start**************************************************/
#ifndef	_AD_D_H_H
#define	_AD_D_H_H

#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"

#define	DA_SCK_Pin			GPIO_Pin_8
#define	DA_SCK_Group		GPIOE
#define	DA_SCK_Rcc			RCC_AHB1Periph_GPIOE

#define	DA_SDA_Pin			GPIO_Pin_9
#define	DA_SDA_Group		GPIOE
#define	DA_SDA_Rcc			RCC_AHB1Periph_GPIOE

#define	DA_CS_Pin			GPIO_Pin_15
#define	DA_CS_Group			GPIOE
#define	DA_CS_Rcc			RCC_AHB1Periph_GPIOE

#define DA_SDA_H()	    	GPIO_SetBits(DA_SDA_Group, DA_SDA_Pin)
#define DA_SDA_L()	    	GPIO_ResetBits(DA_SDA_Group, DA_SDA_Pin)
#define DA_SDA_PIN()	    GPIO_ReadInputDataBit(DA_SDA_Group, DA_SDA_Pin) //读取引脚电平

#define	DA_SCK_H()			GPIO_SetBits(DA_SCK_Group, DA_SCK_Pin)
#define	DA_SCK_L()			GPIO_ResetBits(DA_SCK_Group, DA_SCK_Pin)

#define	DA_CS_H()			GPIO_SetBits(DA_CS_Group, DA_CS_Pin)
#define	DA_CS_L()			GPIO_ResetBits(DA_CS_Group, DA_CS_Pin)

void DA_GPIOInit(void);
void SetLedVol(unsigned short volNum);
void SetLineVol(unsigned short volNum);
	
#endif
/****************************************end of file************************************************/

