/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"CodeScan_Task.h"
#include	"CodeScanFunction.h"

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

static xQueueHandle xStartScanQueue = NULL ;			//ɨ���ά�����ݿռ��ַ�Ķ��У�������ܵ��ռ��ַ������ɨ���ά������
static xQueueHandle xScanResultQueue = NULL;			//����ɨ��������
static QRCode * cardQR;									//ɨ���ά�����ݿռ��ַ
static ScanCodeResult scanResult;						//ɨ���ά��Ľ��
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
	xStartScanQueue = xQueueCreate(1, sizeof(void *));
	
	xScanResultQueue = xQueueCreate(1, sizeof(ScanCodeResult));
	
	return xTaskCreate( vCodeScanTask, CodeScanTaskName, configMINIMAL_STACK_SIZE*2, NULL, vCodeScanTask_PRIORITY, NULL );
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
		if(pdPASS == xQueueReceive( xStartScanQueue, &cardQR, portMAX_DELAY))
		{	
			scanResult = ScanCodeFun(cardQR);										//��ȡ��ά��

			xQueueSend( xScanResultQueue, &scanResult, 100/portTICK_RATE_MS );
		}
	}
}


MyRes StartScanQRCode(QRCode * parm)
{
	while((pdPASS == xQueueReceive( xScanResultQueue, &scanResult,  10/portTICK_RATE_MS)));
	
	if(pdPASS == xQueueSend( xStartScanQueue, &parm, 10*portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;	
}

MyRes TakeScanQRCodeResult(ScanCodeResult *scanresult)
{
	if(pdPASS == xQueueReceive( xScanResultQueue, scanresult,  0/portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;
}

