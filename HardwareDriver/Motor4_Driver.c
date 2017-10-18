/******************************************File Start**********************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include	"Motor4_Driver.h"

#include	"Motor_Data.h"
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
#if (Motor4Type == Motor4IOMotor)
/***************************************************************************************************
*FunctionName:  Motor4_GPIO_Init
*Description:  初始化电机1的引脚
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:31:25
***************************************************************************************************/
void Motor4_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(Motor4_CLK_RCC | Motor4_DIR_RCC | Motor4_Sleep_RCC | Motor4_Sensor1_RCC, ENABLE);
	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = Motor4_CLK_Pin;
	GPIO_Init(Motor4_CLK_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Motor4_DIR_Pin;
	GPIO_Init(Motor4_DIR_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Motor4_Sleep_Pin;
	GPIO_Init(Motor4_Sleep_Group, &GPIO_InitStructure);
	setMotor4SleepGPIO(ON);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = Motor4_Sensor1_Pin;
	GPIO_Init(Motor4_Sensor1_Group, &GPIO_InitStructure);
}

/***************************************************************************************************
*FunctionName:  setMotor4ClkGPIO
*Description:   控制电机时钟脚
*Input:  on -- 拉高
*		off -- 拉低
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:36:18
***************************************************************************************************/
void setMotor4ClkGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor4_CLK_Group, Motor4_CLK_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor4_CLK_Group, Motor4_CLK_Pin, Bit_RESET);
}

/***************************************************************************************************
*FunctionName:  setMotor4DirGPIO
*Description:   控制电机方向
*Input:  on -- 拉高
*		off -- 拉低
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:36:18
***************************************************************************************************/
void setMotor4DirGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor4_DIR_Group, Motor4_DIR_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor4_DIR_Group, Motor4_DIR_Pin, Bit_RESET);
}

/***************************************************************************************************
*FunctionName:  setMotor4ClkGPIO
*Description:   控制电机休眠
*Input:  on -- 拉高
*		off -- 拉低
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:36:18
***************************************************************************************************/
void setMotor4SleepGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor4_Sleep_Group, Motor4_Sleep_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor4_Sleep_Group, Motor4_Sleep_Pin, Bit_RESET);
}
#endif
/****************************************end of file************************************************/
