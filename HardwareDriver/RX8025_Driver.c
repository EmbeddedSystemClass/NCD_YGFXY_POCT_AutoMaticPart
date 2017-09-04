/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"RX8025_Driver.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

#include 	"delay.h"
/***************************************************************************************************/
/******************************************Static Variables*****************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/******************************************Static Methods*******************************************/
/***************************************************************************************************/
static void RX_SDA_IN(void);
static void RX_SDA_OUT(void);
static void RX_IIC_Start(void);
static void RX_IIC_Stop(void);
static unsigned char RX8025_Write(unsigned char addr, unsigned char *pdata, unsigned char len);
static unsigned char RX8025_Read(unsigned char addr, unsigned char *pdata, unsigned char len);
static unsigned char BCD2HEX(unsigned char bcd_data);
static unsigned char HEX2BCD(unsigned char hex_data);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
void RTC_BSPInit(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RX_SCK_Rcc | RX_SDA_Rcc, ENABLE);

	GPIO_InitStructure.GPIO_Pin = RX_SCK_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(RX_SCK_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RX_SDA_Pin;
    GPIO_Init(RX_SDA_Group, &GPIO_InitStructure);
}

/***************************************************************************************************
*FunctionName��RX_SDA_IN
*Description��ʱ��оƬ�����߸�������Ϊ���
*Input��None
*Output��None
*Author��xsx
*Data�2016��9��18��10:10:12
***************************************************************************************************/
static void RX_SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = RX_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(RX_SDA_Group, &GPIO_InitStructure);
}
/***************************************************************************************************
*FunctionName��RX_SDA_OUT
*Description��ʱ��оƬ�����߸�������Ϊ���
*Input��None
*Output��None
*Author��xsx
*Data��2016��9��18��10:10:16
***************************************************************************************************/
static void RX_SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = RX_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(RX_SDA_Group, &GPIO_InitStructure);
}

static unsigned char BCD2HEX(unsigned char bcd_data)  
{   
    unsigned char temp;   
    temp=(bcd_data>>4)*10 + (bcd_data&0x0f);
    return temp;
}
static unsigned char HEX2BCD(unsigned char hex_data)  
{   
    unsigned char temp; 
    temp=((hex_data/10)<<4) + (hex_data%10);
    return temp; 	
}

/***************************************************************************************************
*FunctionName:RX_IIC_Start 
*Description: IIC������ʼ�ź�
*Input: 
*Output: 
*Author: xsx
*Date: 2016��9��18��10:18:20
***************************************************************************************************/
static void RX_IIC_Start(void)
{
	RX_SDA_OUT();
	
	RX_SDA_H();
	RX_SCK_H();
	delay_us(RX_DelayTime);
	
	RX_SDA_L();
	delay_us(RX_DelayTime);
	
	RX_SCK_L();
	delay_us(RX_DelayTime);
}

/***************************************************************************************************
*FunctionName:RX_IIC_Stop 
*Description: IIC���߽����ź�
*Input: 
*Output: 
*Author: xsx
*Date: 2016��9��18��10:18:35
***************************************************************************************************/
static void RX_IIC_Stop(void)
{
	RX_SDA_OUT();
	
	RX_SCK_L();
	RX_SDA_L();
	delay_us(RX_DelayTime);
	
	RX_SCK_H();
	delay_us(RX_DelayTime);
	
	RX_SDA_H();
	delay_us(RX_DelayTime);
}

static unsigned char RX_IIC_Wait_Ack(void)
{
	unsigned char errortime = 0;
	
	RX_SDA_IN();
	RX_SCK_H();
	delay_us(RX_DelayTime);
	
	while(RX_SDA_PIN())
	{
		errortime++;
		
		if(errortime > 250)
		{
			RX_IIC_Stop();
			return 0;
		}
	}
	delay_us(RX_DelayTime);
	RX_SCK_L();
	return 1;
}

//����ACKӦ��
void RX_IIC_Ack(void)
{
	RX_SCK_L();
	
	RX_SDA_OUT();
	RX_SDA_L();
	delay_us(RX_DelayTime);
	
	RX_SCK_H();
	delay_us(RX_DelayTime);
	RX_SCK_L();
	delay_us(RX_DelayTime);
}
//������ACKӦ��		    
void RX_IIC_NAck(void)
{
	RX_SCK_L();
	RX_SDA_OUT();
	RX_SDA_H();
	delay_us(RX_DelayTime);
	
	RX_SCK_H();
	delay_us(RX_DelayTime);
	RX_SCK_L();
	delay_us(RX_DelayTime);
}

static unsigned char RX_IIC_WriteByte(unsigned char data)
{
	unsigned char i=0;
	
	RX_SDA_OUT();
	
	RX_SCK_L();
	delay_us(RX_DelayTime);
	for(i=0; i<8; i++)
	{		
		if(data&0x80)
			RX_SDA_H();
		else
			RX_SDA_L();
		delay_us(RX_DelayTime);
		
		RX_SCK_H();
		delay_us(RX_DelayTime);
		
		RX_SCK_L();
		delay_us(RX_DelayTime);
		
		data <<= 1;
	}
	
	return RX_IIC_Wait_Ack();
}

static unsigned char RX_IIC_ReadByte(unsigned char ack)
{
	unsigned char i, data = 0;
	
	RX_SDA_IN();
	
	for(i=0; i<8; i++)
	{
		RX_SCK_L();
		delay_us(RX_DelayTime);
		
		RX_SCK_H();
		
		data <<= 1;
		
		if(RX_SDA_PIN())
			data++;
		delay_us(RX_DelayTime);
	}
	
	if(!ack)
		RX_IIC_NAck();
	else
		RX_IIC_Ack();
	
	return data;
}

static unsigned char RX8025_Write(unsigned char addr, unsigned char *pdata, unsigned char len)
{
	unsigned char i=0;
	
	RX_IIC_Start();
	
	if(RX_IIC_WriteByte(0x64) == 0)
	{
		RX_IIC_Stop();
		return 0;
	}
	
	if(RX_IIC_WriteByte(addr) == 0)
	{
		RX_IIC_Stop();
		return 0;
	}
	
	for(i=0; i<len; i++)
	{
		if(RX_IIC_WriteByte(*pdata) == 0)
		{
			RX_IIC_Stop();
			return 0;
		}
		pdata++;
	}
	
	RX_IIC_Stop();
	
	return 1;
}

static unsigned char RX8025_Read(unsigned char addr, unsigned char *pdata, unsigned char len)
{
	unsigned char i=0;
	
	RX_IIC_Start();
	
	if(RX_IIC_WriteByte(0x64) == 0)
	{
		RX_IIC_Stop();
		return 0;
	}
	
	if(RX_IIC_WriteByte(addr) == 0)
	{
		RX_IIC_Stop();
		return 0;
	}
	
	RX_IIC_Start();
	
	if(RX_IIC_WriteByte(0x65) == 0)
	{
		RX_IIC_Stop();
		return 0;
	}
	
	for(i=0; i<len-1; i++)
	{
		*pdata++ = RX_IIC_ReadByte(1);
	}
	
	*pdata++ = RX_IIC_ReadByte(0);
	
	RX_IIC_Stop();
	
	return 1;
}

/***************************************************************************************************
*FunctionName: RTC_SetTimeData
*Description: ����rtcʱ��
*Input: data -- ����ΪDateTime�ṹ��ָ��
*Output: 
*Author: xsx
*Date: 2016��9��18��16:11:52
***************************************************************************************************/
MyRes RTC_SetTimeData(DateTime * data)
{
	unsigned char buf[7];
	
	/*����ʱ��*/
	buf[0] = HEX2BCD(data->sec);
	buf[1] = HEX2BCD(data->min);
	buf[2] = HEX2BCD(data->hour);
	buf[3] = HEX2BCD(0);
	
	/*��������*/
	buf[4] = HEX2BCD(data->day);
	buf[5] = HEX2BCD(data->month);
	buf[6] = HEX2BCD(data->year);
	
	if(RX8025_Write(0, buf, 7) == 0)
		return My_Fail;
	else
		return My_Pass;
}

MyRes RTC_GetTimeData(DateTime * time)
{
	unsigned char buf[16];
	unsigned char tempV = 0;
	
	RX8025_Read(0, buf, 16);
	
	tempV = BCD2HEX(buf[6]);
	if((tempV >= 16) && (tempV <= 99))
		time->year = tempV;  
	else
		return My_Fail;
	
	tempV = BCD2HEX(buf[5]);
	if((tempV >= 1) && (tempV <= 12))
		time->month = tempV;
	else
		return My_Fail;
	
	tempV = BCD2HEX(buf[4]);
	if((tempV >= 1) && (tempV <= 31))
		time->day = tempV;
	else
		return My_Fail;
	
	tempV = BCD2HEX(buf[2]);
	if(tempV <= 23)
		time->hour = tempV;
	else
		return My_Fail;
	
	tempV = BCD2HEX(buf[1]);
	if(tempV <= 59)
		time->min = tempV;
	else
		return My_Fail;
	
	tempV = BCD2HEX(buf[0]);
	if(tempV <= 59)
		time->sec = tempV;
	else
		return My_Fail;
	
	return My_Pass;
}

/****************************************end of file************************************************/
