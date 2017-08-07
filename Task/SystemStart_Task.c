/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"SystemStart_Task.h"

#include	"Iwdg_Task.h"
#include	"Universal_Task.h"
#include	"Motor_Data.h"
#include	"Motor1_Fun.h"
#include	"Motor2_Fun.h"
#include	"Motor3_Fun.h"
#include	"Motor4_Fun.h"

#include	"Define.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
#define	SystemStartTask_PRIORITY			2
const char * SystemStartTaskName = "vSystemStartTask";
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void vSystemStartTask( void *pvParameters );
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName: StartSystemStartTask
*Description: 创建系统初始化任务
*Input: 
*Output: 
*Author: xsx
*Date: 2016年9月21日11:58:17
***************************************************************************************************/
void StartSystemStartTask(void)
{
	xTaskCreate( vSystemStartTask, SystemStartTaskName, configMINIMAL_STACK_SIZE, NULL, SystemStartTask_PRIORITY, NULL );
}

/***************************************************************************************************
*FunctionName: vSystemStartTask
*Description: 系统初始化以及自检，启动
*Input: 
*Output: 
*Author: xsx
*Date: 2016年9月21日11:58:46
***************************************************************************************************/
static void vSystemStartTask( void *pvParameters )
{
	/*开启看门狗任务*/
	StartvIwdgTask();
	
	/*通用任务*/
	StartvUniversalTask();
	
	
//	resetToOriginLocation(true);
/*	motor4ResetToOriginLocation();
		vTaskDelay(1000 / portTICK_RATE_MS);
	resetToOriginLocation(true);
	
	motor3StartMove(true, 10000);
	vTaskDelay(1000 / portTICK_RATE_MS);
	
	while(1)
	{
		motor4MoveTo(500);
		motor3StartMove(true, 2000);
		vTaskDelay(1000 / portTICK_RATE_MS);
		
		motor4MoveTo(960);
		vTaskDelay(1000 / portTICK_RATE_MS);
		
		motor3StartMove(true, 15000);
		vTaskDelay(1000 / portTICK_RATE_MS);
		
		motor4MoveTo(500);
		vTaskDelay(1000 / portTICK_RATE_MS);
		
		motor3StartMove(false, 5000);
		vTaskDelay(1000 / portTICK_RATE_MS);
		
		motor3StartMove(true, 5000);
		vTaskDelay(1000 / portTICK_RATE_MS);
		
		motor4MoveTo(960);
		vTaskDelay(1000 / portTICK_RATE_MS);
		
		motor3StartMove(false, 17000);
		vTaskDelay(1000 / portTICK_RATE_MS);
		
		
	}*/
	vTaskDelete(NULL);
}

/****************************************end of file************************************************/
