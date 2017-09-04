/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"Motor_Task.h"
#include	"Motor_Fun.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

#define vMotorTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )
const char * MotorTaskName = "vMotorTask";

static xQueueHandle xMotorActionQueue = NULL ;			//���ָ�����
static SemaphoreHandle_t xMotorMoveStateSemaphoreHandle;	//������������ź���
static MotorAction motorAction;							//���ָ���
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

static void vMotorTask( void *pvParameters );

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
char StartMotorTask(void)
{
	xMotorActionQueue = xQueueCreate(1, sizeof(MotorAction));
	
	xMotorMoveStateSemaphoreHandle = xSemaphoreCreateBinary();
	
	return xTaskCreate( vMotorTask, MotorTaskName, configMINIMAL_STACK_SIZE, NULL, vMotorTask_PRIORITY, NULL );
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

