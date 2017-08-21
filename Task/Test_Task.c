/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Test_Task.h"
#include	"Test_Fun.h"
#include	"System_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
#define TestTask_PRIORITY			3
const char * TestTaskName = "vTestTask";

static xQueueHandle xStartTestQueue = NULL ;						//ɨ�����ݿռ���У�����������ɨ������
static PaiduiUnitData * paiduiUnitData;											//ɨ������ָ��

static xQueueHandle xTestResultQueue = NULL;						//ɨ���������
static ResultState resultstatues;									//ɨ�����
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void vTestTask( void *pvParameters );
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

char StartvTestTask(void)
{
	//�����������ݶ��У�ͬʱ������������
	if(xStartTestQueue == NULL)
		xStartTestQueue = xQueueCreate(1, sizeof(void *));
	
	if(xStartTestQueue == NULL)
		return My_Fail;
	
	//��������������
	if(xTestResultQueue == NULL)
		xTestResultQueue = xQueueCreate(1, sizeof(ResultState));
	
	if(xTestResultQueue == NULL)
		return My_Fail;
	
	return xTaskCreate( vTestTask, TestTaskName, configMINIMAL_STACK_SIZE, NULL, TestTask_PRIORITY, NULL );
}


static void vTestTask( void *pvParameters )
{
	while(1)
	{
		if(pdPASS == xQueueReceive( xStartTestQueue, &paiduiUnitData, portMAX_DELAY))
		{
			clearTestResult();

			resultstatues = TestFunction(paiduiUnitData);
			
			xQueueSend( xTestResultQueue, &resultstatues, 1000/portTICK_RATE_MS );

		}
	}
}


MyRes StartTest(TestData * parm)
{
	if(pdPASS == xQueueSend( xStartTestQueue, &parm, 10/portTICK_RATE_MS ))
		return My_Pass;
	else
		return My_Fail;	
}


MyRes TakeTestResult(ResultState *testsult)
{
	if(pdPASS == xQueueReceive( xTestResultQueue, testsult,  10/portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;
}

void clearTestResult(void)
{
	while(pdPASS == TakeTestResult(&resultstatues))
		;
}

/****************************************end of file************************************************/
