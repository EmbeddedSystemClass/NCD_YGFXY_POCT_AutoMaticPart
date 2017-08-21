/***************************************************************************************************
*FileName:QueueUnits
*Description:队列操作工具类
*Author:xsx
*Data:2016年4月22日14:28:03
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"QueueUnits.h"


/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/

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
*FunctionName：ReceiveDataFromQueue
*Description：从队列中读取有限长度的数据
*Input：queue -- 目标队列
*		receivedstr -- 存放接收数据的地址
*		len -- 接收的数据长度(长度为此队列单元数据大小的个数，与数据接收数据的字节长度无关)
*		readSize -- 实际读取到的数据长度
*		itemsize -- 队列单元数据的大小
*		queueBlockTime -- 队列阻塞时间
*Output：None
*Author：xsx
*Data：2016年4月22日15:35:40
***************************************************************************************************/
MyRes ReceiveDataFromQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *receivedstr, unsigned short len, unsigned short * readSize, 
	unsigned short itemsize, portTickType queueBlockTime, portTickType mutexBlockTime)
{
	unsigned short i=0;
	unsigned char *pdata = (unsigned char *)receivedstr;
	
	if(queue == NULL)
		return My_Fail;
	
	if(mutex != NULL)
	{
		if(pdFAIL == xSemaphoreTake(mutex, mutexBlockTime))
			return My_Fail;
	}
	
	for(i=0; i<len; i++)
	{
		if(pdPASS == xQueueReceive(queue, pdata , queueBlockTime))
			pdata += itemsize;
			
		else
			break;
	}

	if(readSize)
		*readSize = i;
	
	if(mutex != NULL)
		xSemaphoreGive(mutex);

	if(i > 0)
		return My_Pass;
	else
		return My_Fail;
}
/***************************************************************************************************
*FunctionName：SendDataToQueue
*Description：发送数据到队列
*Input：queue -- 目标队列
*		sendstr -- 发送的数据地址
*		len -- 接收的数据长度(长度为此队列单元数据大小的个数，与数据接收数据的字节长度
*		itemsize -- 队列单元数据的大小
*		queueBlockTime -- 队列阻塞时间
*Output：None
*Author：xsx
*Data：2016年4月22日15:33:38
***************************************************************************************************/
MyRes SendDataToQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *sendstr, unsigned short len, unsigned short itemsize, 
	portTickType queueBlockTime, portTickType mutexBlockTime, void (*fun)(void))
{
	unsigned short i=0;
	unsigned char *pdata = (unsigned char *)sendstr;
	MyRes statues = My_Pass;
	
	if(queue == NULL)
		return My_Fail;

	if(mutex != NULL)
	{
		if(pdFAIL == xSemaphoreTake(mutex, mutexBlockTime))
			return My_Fail;
	}
	
	for(i=0; i<len; i++)
	{
		if(pdPASS == xQueueSend(queue, pdata , queueBlockTime))
			pdata += itemsize;
		else
		{
			statues = My_Fail;
			break;
		}
	}

	if(statues == My_Pass && fun != NULL)
		fun();
	
	if(mutex != NULL)
		xSemaphoreGive(mutex);

	return statues;
}

