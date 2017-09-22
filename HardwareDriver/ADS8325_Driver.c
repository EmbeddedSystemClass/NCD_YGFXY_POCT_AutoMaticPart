/***************************************************************************************************
*FileName��Ads8325_Driver
*Description��ADCоƬADS8325����
*Author��xsx
*Data��2016��3��18��14:11:02
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"Ads8325_Driver.h"

#include	"Delay.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��ADS_IO_Init
*Description��ADоƬ�˿ڳ�ʼ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��3��18��14:37:44
***************************************************************************************************/
void ADS_IO_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(ADS_SCK_Rcc | ADS_SDA_Rcc | ADS_CS_Rcc, ENABLE);

	GPIO_InitStructure.GPIO_Pin = ADS_SCK_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(ADS_SCK_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADS_CS_Pin;
	GPIO_Init(ADS_CS_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADS_SDA_Pin;
	GPIO_Init(ADS_SDA_Group, &GPIO_InitStructure);
	
    ADS_CS_H();
}

/***************************************************************************************************
*FunctionName��TM1623_SDA_IN
*Description���Ŷӹ���ģ��ͨ��SDA������
*Input��None
*Output��None
*Author��xsx
*Data��2016��3��12��14:33:11
***************************************************************************************************/
static void ADS_SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = ADS_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ADS_SDA_Group, &GPIO_InitStructure);
}
/***************************************************************************************************
*FunctionName��TM1623_SDA_OUT
*Description���Ŷӹ���ģ��ͨ��SDA�����
*Input��None
*Output��None
*Author��xsx
*Data��2016��3��12��14:33:23
***************************************************************************************************/
static void ADS_SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = ADS_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ADS_SDA_Group, &GPIO_InitStructure);
}

/***************************************************************************************************
*FunctionName��ADS8325
*Description���ɼ���ǿ��mv
*Input��None
*Output����ǿ�ĵ�ѹֵmv
*Author��xsx
*Data��2016��6��27��16:50:52
***************************************************************************************************/
double ADS8325(void)
{
	unsigned short AData=0;
	unsigned char i=16;
	double temp = 0;
	
	ADS_SDA_OUT();
	ADS_CS_H();
	ADS_SCK_H();
	ADS_SDA_H();
	
	ADS_CS_L();
	
	ADS_SDA_IN();
	delay_us(1);
	while(ADS_SDA_PIN())
	{
		ADS_SCK_L();
		delay_us(1);
		
		ADS_SCK_H();
		delay_us(1);
		
		AData++;
		if(AData > 200)
			return 0;
	}
	
	for(i=0; i<16; i++)
	{
		ADS_SCK_L();

		ADS_SCK_H();
		delay_us(1);
		
		AData=AData<<1;
		if(ADS_SDA_PIN())
			AData=AData|0x01;
		
		delay_us(1);
	}
	
	delay_us(10);
	ADS_CS_H();
	
	temp = AData;

	temp /= 65535;
	temp *= 4096;
	
	return temp;
} 


