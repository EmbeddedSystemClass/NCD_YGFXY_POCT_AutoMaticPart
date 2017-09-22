#ifndef ADS8325_D_H_H
#define ADS8325_D_H_H

#include	"stm32f4xx.h"
#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"

#define	ADS_SCK_Pin			GPIO_Pin_8
#define	ADS_SCK_Group		GPIOA
#define	ADS_SCK_Rcc			RCC_AHB1Periph_GPIOA

#define	ADS_SDA_Pin			GPIO_Pin_9
#define	ADS_SDA_Group		GPIOC
#define	ADS_SDA_Rcc			RCC_AHB1Periph_GPIOC

#define	ADS_CS_Pin			GPIO_Pin_8
#define	ADS_CS_Group		GPIOC
#define	ADS_CS_Rcc			RCC_AHB1Periph_GPIOC

#define ADS_SDA_H()	    	GPIO_SetBits(ADS_SDA_Group, ADS_SDA_Pin)
#define ADS_SDA_L()	    	GPIO_ResetBits(ADS_SDA_Group, ADS_SDA_Pin)
#define ADS_SDA_PIN()	    GPIO_ReadInputDataBit(ADS_SDA_Group, ADS_SDA_Pin) //读取引脚电平

#define	ADS_SCK_H()			GPIO_SetBits(ADS_SCK_Group, ADS_SCK_Pin)
#define	ADS_SCK_L()			GPIO_ResetBits(ADS_SCK_Group, ADS_SCK_Pin)

#define	ADS_CS_H()			GPIO_SetBits(ADS_CS_Group, ADS_CS_Pin)
#define	ADS_CS_L()			GPIO_ResetBits(ADS_CS_Group, ADS_CS_Pin)


void ADS_IO_Init(void);
double ADS8325(void);

#endif


