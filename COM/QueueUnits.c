/***************************************************************************************************
*FileName:QueueUnits
*Description:���в���������
*Author:xsx
*Data:2016��4��22��14:28:03
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"QueueUnits.h"


/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
unsigned char WaittingForMutex(xSemaphoreHandle mutex, portTickType xBlockTime)
{
	return xSemaphoreTake(mutex, xBlockTime);
}
void GivexMutex(xSemaphoreHandle mutex)
{
	xSemaphoreGive(mutex);
}

/***************************************************************************************************
*FunctionName��ReceiveDataFromQueue
*Description���Ӷ����ж�ȡ���޳��ȵ�����
*Input��queue -- Ŀ�����
*		mutex -- �˶��еĻ���������Ϊnull
*		receivedstr -- ��Ž������ݵĵ�ַ
*		len -- ���յ����ݳ���(����Ϊ�˶��е�Ԫ���ݴ�С�ĸ����������ݽ������ݵ��ֽڳ����޹�)
*		readSize -- ʵ�ʶ�ȡ�������ݳ���
*		itemsize -- ���е�Ԫ���ݵĴ�С
*		queueBlockTime -- ��������ʱ��
*		mutexBlockTime -- ����������ʱ��
*Output��None
*Author��xsx
*Data��2016��4��22��15:35:40
***************************************************************************************************/
unsigned char ReceiveDataFromQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *receivedstr , unsigned short len ,
	unsigned short * readSize, unsigned short itemsize, portTickType queueBlockTime, portTickType mutexBlockTime)
{
	unsigned short i=0;
	unsigned char *pdata = (unsigned char *)receivedstr;
	unsigned char statues = pdFAIL;
	
	if(queue == NULL)
		return pdFAIL;
	
	if(mutex != NULL)
	{
		if(pdFAIL == WaittingForMutex(mutex, mutexBlockTime))
			return pdFAIL;
	}
	
	for(i=0; i<len; i++)
	{
		if(pdPASS == xQueueReceive(queue, pdata , queueBlockTime))
			pdata += itemsize;
			
		else
			break;
	}
	
	if(i > 0)
		statues = pdPASS;
	
	if(readSize)
		*readSize = i;
	
	if(mutex != NULL)
		GivexMutex(mutex);
	
	return statues;
}
/***************************************************************************************************
*FunctionName��SendDataToQueue
*Description���������ݵ�����
*Input��queue -- Ŀ�����
*		mutex -- �˶��еĻ���������Ϊnull
*		sendstr -- ���͵����ݵ�ַ
*		len -- ���յ����ݳ���(����Ϊ�˶��е�Ԫ���ݴ�С�ĸ����������ݽ������ݵ��ֽڳ���
*		itemsize -- ���е�Ԫ���ݵĴ�С
*		queueBlockTime -- ��������ʱ��
*		mutexBlockTime -- ����������ʱ��
*Output��None
*Author��xsx
*Data��2016��4��22��15:33:38
***************************************************************************************************/
unsigned char SendDataToQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *sendstr , unsigned short len ,  
	unsigned short itemsize, portTickType queueBlockTime, portTickType mutexBlockTime, void (*fun)(void))
{
	unsigned short i=0;
	unsigned char *pdata = (unsigned char *)sendstr;
	unsigned char statues = pdFAIL;
	
	if(queue == NULL)
		return pdFAIL;
	
	if(mutex != NULL)
	{
		if(pdFAIL == WaittingForMutex(mutex, mutexBlockTime))
			return pdFAIL;
	}

	for(i=0; i<len; i++)
	{
		if(pdPASS == xQueueSend(queue, pdata , queueBlockTime))
		{
			pdata += itemsize;
			statues = pdPASS;
		}
		else
		{
			statues = pdFAIL;
			break;
		}
	}
	
	if(mutex != NULL)
		GivexMutex(mutex);
	
	if(fun != NULL)
		fun();
	
	return statues;
}

