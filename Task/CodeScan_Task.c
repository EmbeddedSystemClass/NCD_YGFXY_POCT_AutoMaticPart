/******************************************************************************************/
/*****************************************头文件*******************************************/

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
/*****************************************局部变量声明*************************************/

#define vCodeScanTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )
const char * CodeScanTaskName = "vCodeScanTask";

static xQueueHandle xStartScanQueue = NULL ;			//扫描二维码数据空间地址的队列，如果接受到空间地址则启动扫描二维码任务
static QRCode * cardQR;							//扫描二维码数据空间地址
static ScanCodeResult scanResult;						//扫码二维码的结果
/******************************************************************************************/
/*****************************************局部函数声明*************************************/

static void vCodeScanTask( void *pvParameters );

/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/************************************************************************
** 函数名:
** 功  能:
** 输  入:无
** 输  出:无
** 返  回：无
** 备  注：无
** 时  间:  
** 作  者：xsx                                                 
************************************************************************/
char StartCodeScanTask(void)
{
	xStartScanQueue = xQueueCreate(1, sizeof(void *));
	
	return xTaskCreate( vCodeScanTask, CodeScanTaskName, configMINIMAL_STACK_SIZE+100, NULL, vCodeScanTask_PRIORITY, NULL );
}

/************************************************************************
** 函数名:
** 功  能:
** 输  入:无
** 输  出:无
** 返  回：无
** 备  注：无
** 时  间:  
** 作  者：xsx                                                 
************************************************************************/
static void vCodeScanTask( void *pvParameters )
{
	while(1)
	{
		if(pdPASS == xQueueReceive( xStartScanQueue, &cardQR, portMAX_DELAY))
		{			
			scanResult = ScanCodeFun(cardQR);										//读取二维码
			
			vTaskDelay(10 / portTICK_RATE_MS);
		}
	}
}


MyRes StartScanQRCode(QRCode * parm)
{
	scanResult = CardCodeScanning;
	
	if(pdPASS == xQueueSend( xStartScanQueue, &parm, 10*portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;	
}

ScanCodeResult getScanResult(void)
{
	return scanResult;
}

