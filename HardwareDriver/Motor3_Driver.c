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
*Description:  初始化电机1的引脚
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:31:25
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
*Description:   控制电机时钟脚
*Input:  on -- 拉高
*		off -- 拉低
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:36:18
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
*Description:   控制电机方向
*Input:  on -- 拉高，前进
*		off -- 拉低，后退
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:36:18
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
*Description:   控制电机休眠
*Input:  on -- 拉高,休眠
*		off -- 拉低，不休眠
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:36:18
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
*Description:   读取爪子移动原点传感器状态
*Input:  	on -- 传感器触发
*			off -- 传感器没触发
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 15:52:11
***************************************************************************************************/
MyBitAction getMotor3OriginStatus(void)
{
	if(GPIO_ReadInputDataBit(Motor3_Origin_Group, Motor3_Origin_Pin))
		return OFF;
	else
		return ON;
}
/****************************************end of file************************************************/
