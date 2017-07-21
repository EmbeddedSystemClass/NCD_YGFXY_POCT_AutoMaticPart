/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"CodeScan_Task.h"
#include	"CodeScanFunction.h"
#include	"QRCode_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

#define vCodeScanTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )
const char * CodeScanTaskName = "vCodeScanTask";

static SemaphoreHandle_t xSemaphore;								//������,����������ȡ��ά������
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

static void vCodeScanTask( void *pvParameters );

/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/************************************************************************
** ������:
** ��  ��:
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
char StartCodeScanTask(void)
{
	xSemaphore = xSemaphoreCreateBinary();
	
	return xTaskCreate( vCodeScanTask, CodeScanTaskName, configMINIMAL_STACK_SIZE+100, NULL, vCodeScanTask_PRIORITY, NULL );
}

/************************************************************************
** ������:
** ��  ��:
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
static void vCodeScanTask( void *pvParameters )
{
	while(1)
	{
		if(pdPASS == xSemaphoreTake(xSemaphore, portMAX_DELAY))
		{
			setReadQRCodeStatus(true);
			resetGB_QRCode();
			vTaskDelay(10 / portTICK_RATE_MS);
			
			ScanCodeFun();
			
			vTaskDelay(10 / portTICK_RATE_MS);
			setReadQRCodeStatus(false);
		}
	}
}


MyRes startScanQRCode(void)
{
	if(pdPASS == xSemaphoreGive(xSemaphore))
		return My_Pass;
	else
		return My_Fail;	
}

