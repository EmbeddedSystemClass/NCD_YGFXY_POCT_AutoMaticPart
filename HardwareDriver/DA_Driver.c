/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"DA_Driver.h"

#include	"System_Data.h"

#include	"Delay.h"
/***************************************************************************************************/
/******************************************Static Variables*****************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/******************************************Static Methods*******************************************/
/***************************************************************************************************/
static void DA_Write(unsigned short data);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName: DA_GPIOInit
*Description: ��������ܣ����߿���DA GPIO��ʼ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��8��7�� 10:08:39
***************************************************************************************************/
void DA_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(DA_SCK_Rcc | DA_SDA_Rcc | DA_CS_Rcc, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DA_SCK_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(DA_SCK_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DA_SDA_Pin;
    GPIO_Init(DA_SDA_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DA_CS_Pin;
    GPIO_Init(DA_CS_Group, &GPIO_InitStructure);
  
	SetLedVol(0);
	SetLineVol(0);
}

/***************************************************************************************************
*FunctionName: DA_Write
*Description: IIC write
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2018��1��15�� 10:13:21
***************************************************************************************************/
static void DA_Write(unsigned short data)
{
	unsigned char i;
	
	DA_SCK_H();
	delay_us(5);
	
	for(i=0; i<16; i++)
    {
		if(data & ((unsigned short)0x8000))
			DA_SDA_H();
        else
			DA_SDA_L();
		delay_us(5);
	
		data <<= 1;
		
		DA_SCK_L();
		delay_us(5);
		
		DA_SCK_H();
		delay_us(5);
    }

}

/***************************************************************************************************
*FunctionName: SetLedVol
*Description: ���Ʒ�������������ѹ
*Input: volNum -- �����ѹֵ��mv��
*Output: 
*Return: 
*Author: xsx
*Date: 2017��8��7�� 10:09:15
***************************************************************************************************/
void SetLedVol(unsigned short volNum)
{
	DA_CS_H();
	delay_us(10);
	
	DA_CS_L();
	delay_us(50);
	
	setTestLedValue(volNum);
	//��2λΪ0
	volNum <<= 2;					
	
	/*����DAC Bֵ*/
	volNum &= ~((unsigned short)1<<(15));
	volNum &= ~((unsigned short)1<<(12));
	
	/*����ģʽ*/
	volNum &= ~((unsigned short)1<<(14));
	
	/*����ģʽ*/
	volNum &= ~((unsigned short)1<<(13));
	
	DA_Write(volNum);
	
	delay_us(50);
	DA_CS_H();
}

/***************************************************************************************************
*FunctionName: SetLineVol
*Description: ���ƻ��������ѹ��mv��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��8��7�� 10:09:37
***************************************************************************************************/
void SetLineVol(unsigned short volNum)
{
	DA_CS_H();
	delay_us(10);
	
	DA_CS_L();
	delay_us(50);
	
	setBaseLineValue(volNum);
	
	//��2λΪ0
	volNum <<= 2;					
	
	/*����DAC Bֵ*/
	volNum |= ((unsigned short)1<<(15));
	volNum &= ~((unsigned short)1<<(12));
	
	/*����ģʽ*/
	volNum &= ~((unsigned short)1<<(14));
	
	/*����ģʽ*/
	volNum &= ~((unsigned short)1<<(13));

	DA_Write(volNum);
	
	delay_us(50);
	DA_CS_H();
}
/****************************************end of file************************************************/
