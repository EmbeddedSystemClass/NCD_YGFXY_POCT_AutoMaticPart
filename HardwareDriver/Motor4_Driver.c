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
	NVIC_InitTypeDef   	NVIC_InitStructure;
	EXTI_InitTypeDef   	EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(Motor4_CLK_RCC | Motor4_DIR_RCC | Motor4_Sleep_RCC | Motor4_Open_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
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
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = Motor4_Open_Pin;
	GPIO_Init(Motor4_Open_Group, &GPIO_InitStructure);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)	 
	{
		//如果是打开电机，产生高电平中断，表明爪子打开到最大位置
		//if(getMotorxDir(Motor_4) == false)
			setMotorxParm1(Motor_4, true);
		
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}

MyBitAction getMotor4OriginStatus(void)
{
	if(GPIO_ReadInputDataBit(Motor4_Open_Group, Motor4_Open_Pin))
		return OFF;
	else
		return ON;
}

/****************************************end of file************************************************/
