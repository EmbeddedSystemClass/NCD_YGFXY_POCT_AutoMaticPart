/******************************************File Start**********************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include	"Motor1_Driver.h"
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
*FunctionName:  Motor1_GPIO_Init
*Description:  ��ʼ�����1������
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  2017��7��20�� 11:31:25
***************************************************************************************************/
void Motor1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(Motor1_CLK_RCC | Motor1_DIR_RCC | Motor1_Sleep_RCC | Motor1_Sensor1_RCC | Motor1_Sensor2_RCC, ENABLE);
	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = Motor1_CLK_Pin;
	GPIO_Init(Motor1_CLK_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Motor1_DIR_Pin;
	GPIO_Init(Motor1_DIR_Group, &GPIO_InitStructure);
	setMotor1DirGPIO(ON);
	
	GPIO_InitStructure.GPIO_Pin = Motor1_Sleep_Pin;
	GPIO_Init(Motor1_Sleep_Group, &GPIO_InitStructure);
	setMotor1SleepGPIO(ON);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = Motor1_Sensor1_Pin;
	GPIO_Init(Motor1_Sensor1_Group, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Motor1_Sensor2_Pin;
	GPIO_Init(Motor1_Sensor2_Group, &GPIO_InitStructure);
}

/***************************************************************************************************
*FunctionName:  setMotor1ClkGPIO
*Description:   ���Ƶ��ʱ�ӽ�
*Input:  on -- ����
*		off -- ����
*Output:  
*Return:  
*Author:  xsx
*Date:  2017��7��20�� 11:36:18
***************************************************************************************************/
void setMotor1ClkGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor1_CLK_Group, Motor1_CLK_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor1_CLK_Group, Motor1_CLK_Pin, Bit_RESET);
}

/***************************************************************************************************
*FunctionName:  setMotor1DirGPIO
*Description:   ���Ƶ������
*Input:  on -- ����
*		off -- ����
*Output:  
*Return:  
*Author:  xsx
*Date:  2017��7��20�� 11:36:18
***************************************************************************************************/
void setMotor1DirGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor1_DIR_Group, Motor1_DIR_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor1_DIR_Group, Motor1_DIR_Pin, Bit_RESET);
}

/***************************************************************************************************
*FunctionName:  setMotor1ClkGPIO
*Description:   ���Ƶ������
*Input:  on -- ����
*		off -- ����
*Output:  
*Return:  
*Author:  xsx
*Date:  2017��7��20�� 11:36:18
***************************************************************************************************/
void setMotor1SleepGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor1_Sleep_Group, Motor1_Sleep_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor1_Sleep_Group, Motor1_Sleep_Pin, Bit_RESET);
}
/****************************************end of file************************************************/
