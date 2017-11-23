/***************************************************************************************************
*FileName:GetLCDInputFun
*Description:获取lcd输入
*Author:xsx
*Data:2016年4月28日17:42:51
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"LCDInputFun.h"
#include	"Usart6_Driver.h"
#include	"UI_Data.h"

#include	"QueueUnits.h"
#include	"MyMem.h"
#include	"Define.h"
#include	"CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
static unsigned char buf[100];
static unsigned char rxcount = 0;
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName：GetLCDInputData
*Description：获取LCD按键输入数据
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月28日17:43:24
***************************************************************************************************/
void LCDInputData(void)
{
	rxcount = 0;
	
	while(pdPASS == xQueueReceive(GetUsart6RXQueue(), buf+rxcount , 20 / portTICK_RATE_MS))
		rxcount++;

	if(rxcount > 0)
	{
		if(buf[0] == LCD_Head_1 && buf[1] == LCD_Head_2 && rxcount == (buf[2]+3))
		{
			activityInputFunction(buf, rxcount);
		}
	}
}

