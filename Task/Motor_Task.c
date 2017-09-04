/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"Motor_Task.h"
#include	"Motor_Fun.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"
/******************************************************************************************/
/*****************************************局部变量声明*************************************/

#define vMotorTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )
const char * MotorTaskName = "vMotorTask";

static xQueueHandle xMotorActionQueue = NULL ;			//电机指令队列
static SemaphoreHandle_t xMotorMoveStateSemaphoreHandle;	//电机动作结束信号量
static MotorAction motorAction;							//电机指令保存
/******************************************************************************************/
/*****************************************局部函数声明*************************************/

static void vMotorTask( void *pvParameters );

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
char StartMotorTask(void)
{
	xMotorActionQueue = xQueueCreate(1, sizeof(MotorAction));
	
	xMotorMoveStateSemaphoreHandle = xSemaphoreCreateBinary();
	
	return xTaskCreate( vMotorTask, MotorTaskName, configMINIMAL_STACK_SIZE, NULL, vMotorTask_PRIORITY, NULL );
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
static void vMotorTask( void *pvParameters )
{
	while(1)
	{
		if(pdPASS == xQueueReceive( xMotorActionQueue, &motorAction, portMAX_DELAY))
		{	
			switch(motorAction.motorActionName)
			{
				case WaitPutInCard:
					MotorMoveToWaitCardPutIn(motorAction.motorActionParm);
					break;
				
				case MoveToStartTestLocation:
					MotorMoveToStartTestLocation(motorAction.motorActionParm);
					break;
				
				case OutOutCard:
					PutCardOutOfDevice();
					break;
				
				case OriginLocation:
					MotorMoveToOriginLocation(motorAction.motorActionParm);
					break;
				default: break;
			}

			xSemaphoreGive( xMotorMoveStateSemaphoreHandle );
		}
	}
}


MyRes StartMotorAction(MotorAction * motorAction)
{
	while(xSemaphoreTake( xMotorMoveStateSemaphoreHandle, ( TickType_t ) 10 ) == pdTRUE);
	
	if(pdPASS == xQueueSend( xMotorActionQueue, motorAction, 10*portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;	
}

bool isMotorActionOver(void)
{
	if(xSemaphoreTake( xMotorMoveStateSemaphoreHandle, ( TickType_t ) 10 ) == pdTRUE)
		return true;
	else
		return false;
}

