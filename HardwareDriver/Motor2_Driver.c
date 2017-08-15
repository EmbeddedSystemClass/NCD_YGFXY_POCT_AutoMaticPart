/******************************************File Start**********************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include	"Motor2_Driver.h"
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
*FunctionName:  Motor2_GPIO_Init
*Description:  初始化电机1的引脚
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:31:25
***************************************************************************************************/
void Motor2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(Motor2_CLK_RCC | Motor2_DIR_RCC | Motor2_Sleep_RCC | Motor2_Sensor1_RCC | 
		Motor2_Sensor2_RCC | Motor2_Sensor3_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = Motor2_CLK_Pin;
	GPIO_Init(Motor2_CLK_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Motor2_DIR_Pin;
	GPIO_Init(Motor2_DIR_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Motor2_Sleep_Pin;
	GPIO_Init(Motor2_Sleep_Group, &GPIO_InitStructure);
	setMotor2SleepGPIO(ON);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = Motor2_Sensor1_Pin;
	GPIO_Init(Motor2_Sensor1_Group, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Motor2_Sensor2_Pin;
	GPIO_Init(Motor2_Sensor2_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Motor2_Sensor3_Pin;
	GPIO_Init(Motor2_Sensor3_Group, &GPIO_InitStructure);
	
/*	SYSCFG_EXTILineConfig(Motor2_Sensor1_EXTI_PortSource, Motor2_Sensor1_EXTI_PinSource);
	SYSCFG_EXTILineConfig(Motor2_Sensor2_EXTI_PortSource, Motor2_Sensor2_EXTI_PinSource);
	SYSCFG_EXTILineConfig(Motor2_Sensor3_EXTI_PortSource, Motor2_Sensor3_EXTI_PinSource);
	
	EXTI_InitStructure.EXTI_Line = Motor2_Sensor1_EXTI_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = Motor2_Sensor2_EXTI_Line;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = Motor2_Sensor3_EXTI_Line;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = Motor2_Sensor1_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = Motor2_Sensor2_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = Motor2_Sensor2_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
	NVIC_Init(&NVIC_InitStructure);*/
}

/***************************************************************************************************
*FunctionName:  setMotor2ClkGPIO
*Description:   控制电机时钟脚
*Input:  on -- 拉高
*		off -- 拉低
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:36:18
***************************************************************************************************/
void setMotor2ClkGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor2_CLK_Group, Motor2_CLK_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor2_CLK_Group, Motor2_CLK_Pin, Bit_RESET);
}

/***************************************************************************************************
*FunctionName:  setMotor2DirGPIO
*Description:   控制电机方向
*Input:  on -- 拉高
*		off -- 拉低
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:36:18
***************************************************************************************************/
void setMotor2DirGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor2_DIR_Group, Motor2_DIR_Pin, Bit_RESET);
	else
		GPIO_WriteBit(Motor2_DIR_Group, Motor2_DIR_Pin, Bit_SET);
}

/***************************************************************************************************
*FunctionName:  setMotor2ClkGPIO
*Description:   控制电机休眠
*Input:  on -- 拉高
*		off -- 拉低
*Output:  
*Return:  
*Author:  xsx
*Date:  2017年7月20日 11:36:18
***************************************************************************************************/
void setMotor2SleepGPIO(MyBitAction myBitAction)
{
	if(ON == myBitAction)
		GPIO_WriteBit(Motor2_Sleep_Group, Motor2_Sleep_Pin, Bit_SET);
	else
		GPIO_WriteBit(Motor2_Sleep_Group, Motor2_Sleep_Pin, Bit_RESET);
}
/****************************************end of file************************************************/
