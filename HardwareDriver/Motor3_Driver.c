/******************************************File Start**********************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include	"Motor3_Driver.h"
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
*FunctionName:  Motor3_GPIO_Init
*Description:  ��ʼ�����1������
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  2017��7��20�� 11:31:25
***************************************************************************************************/
void Motor3_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(Motor3_CLK_RCC | Motor3_DIR_RCC | Motor3_Sleep_RCC | Motor3_Origin_RCC, ENABLE);

  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = Motor3_CLK_Pin;
	GPIO_Init(Motor3_CLK_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Motor3_DIR_Pin;
	GPIO_Init(Motor3_DIR_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Motor3_Sleep_Pin;
	GPIO_Init(Motor3_Sleep_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = Motor3_Origin_Pin;
	GPIO_Init(Motor3_Origin_Group, &GPIO_InitStructure);
}

/***************************************************************************************************
*FunctionName:  setMotor3ClkGPIO
*Description:   ���Ƶ��ʱ�ӽ�
*Input:  on -- ����
*		off -- ����
*Output:  
*Return:  
*Author:  xsx
*Date:  2017��7��20�� 11:36:18
***************************************************************************************************/
void setMotor3ClkGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor3_CLK_Group, Motor3_CLK_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor3_CLK_Group, Motor3_CLK_Pin, Bit_RESET);
}

/***************************************************************************************************
*FunctionName:  setMotor3DirGPIO
*Description:   ���Ƶ������
*Input:  on -- ���ߣ�ǰ��
*		off -- ���ͣ�����
*Output:  
*Return:  
*Author:  xsx
*Date:  2017��7��20�� 11:36:18
***************************************************************************************************/
void setMotor3DirGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor3_DIR_Group, Motor3_DIR_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor3_DIR_Group, Motor3_DIR_Pin, Bit_RESET);
}

/***************************************************************************************************
*FunctionName:  setMotor3ClkGPIO
*Description:   ���Ƶ������
*Input:  on -- ����,����
*		off -- ���ͣ�������
*Output:  
*Return:  
*Author:  xsx
*Date:  2017��7��20�� 11:36:18
***************************************************************************************************/
void setMotor3SleepGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor3_Sleep_Group, Motor3_Sleep_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor3_Sleep_Group, Motor3_Sleep_Pin, Bit_RESET);
}

/***************************************************************************************************
*FunctionName:  getMotor3OriginStatus
*Description:   ��ȡצ���ƶ�ԭ�㴫����״̬
*Input:  	on -- ����������
*			off -- ������û����
*Output:  
*Return:  
*Author:  xsx
*Date:  2017��7��20�� 15:52:11
***************************************************************************************************/
MyBitAction getMotor3OriginStatus(void)
{
	if(GPIO_ReadInputDataBit(Motor3_Origin_Group, Motor3_Origin_Pin))
		return OFF;
	else
		return ON;
}
/****************************************end of file************************************************/
