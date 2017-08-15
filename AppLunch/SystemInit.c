/***************************************************************************************************
*FileName:SystemInit
*Description: 系统所有模块初始化
*Author : xsx
*Data: 2016年4月21日14:18:28
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"SystemInit.h"

#include	"Led_Driver.h"
#include	"CardCheck_Driver.h"
#include 	"Usart1_Driver.h"
#include 	"Usart2_Driver.h"
#include 	"Usart3_Driver.h"
#include 	"Usart4_Driver.h"
#include 	"Usart5_Driver.h"
#include 	"Usart6_Driver.h"
#include	"Motor1_Driver.h"
#include	"Motor2_Driver.h"
#include	"Motor3_Driver.h"
#include	"Motor4_Driver.h"
#include 	"Iwdg_Driver.h"
#include	"Timer3_Driver.h"
#include	"CodeScanner_Driver.h"
#include	"DA_Driver.h"
#include	"Ads8325_Driver.h"
#include	"Channel_Driver.h"
#include	"LedCheck_Driver.h"

#include	"Delay.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
extern void SystemInit(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName：MySystemInit
*Description：系统初始化
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月21日14:20:44
***************************************************************************************************/
void MySystemBSPInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	delay_ms(500);
	SystemInit();						//手动系统初始化
	delay_ms(500);
	
	Led_GPIOInit();						//系统指示灯初始化
	delay_ms(1);

	Usart1_Init();						//串口2初始化
	delay_ms(1);
	
	Usart2_Init();						//串口3初始化
	delay_ms(1);
	
	Usart3_Init();						//串口2初始化
	delay_ms(1);
	
	Usart4_Init();						//串口4初始化
	delay_ms(1);
	
	Usart5_Init();						//串口4初始化
	delay_ms(1);
	
	Usart6_Init();						//串口6初始化
	delay_ms(1);
	
	CardCheck_GPIO_Init();						//限位开关初始化
	delay_ms(1);
	
	Motor1_GPIO_Init();							//电机 IO初始化
	delay_ms(1);
	
	Motor2_GPIO_Init();							//电机 IO初始化
	delay_ms(1);
	
//	Motor3_GPIO_Init();							//电机 IO初始化
	delay_ms(1);
	
	Motor4_GPIO_Init();							//电机 IO初始化
	delay_ms(1);
	
	CodeScanner_GPIO_Init();
	delay_ms(1);
	
	DA_GPIOInit();
	delay_ms(1);
	
	Channel_GPIOInit();
	delay_ms(1);
	
	ADS_IO_Init();
	delay_ms(1);
	
	LedCheck_GPIO_Init();
	delay_ms(1);

	Timer3_Init();						//定时器初始化
	delay_ms(1);
	
	IWDG_Init(3, 3000);					//看门狗初始化,超时时间2S
	delay_ms(1);
}
