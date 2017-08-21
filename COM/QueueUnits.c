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
/***************************************************************************************************
*FunctionName��ReceiveDataFromQueue
*Description���Ӷ����ж�ȡ���޳��ȵ�����
*Input��queue -- Ŀ�����
*		receivedstr -- ��Ž������ݵĵ�ַ
*		len -- ���յ����ݳ���(����Ϊ�˶��е�Ԫ���ݴ�С�ĸ����������ݽ������ݵ��ֽڳ����޹�)
*		readSize -- ʵ�ʶ�ȡ�������ݳ���
*		itemsize -- ���е�Ԫ���ݵĴ�С
*		queueBlockTime -- ��������ʱ��
*Output��None
*Author��xsx
*Data��2016��4��22��15:35:40
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
*FunctionName��SendDataToQueue
*Description���������ݵ�����
*Input��queue -- Ŀ�����
*		sendstr -- ���͵����ݵ�ַ
*		len -- ���յ����ݳ���(����Ϊ�˶��е�Ԫ���ݴ�С�ĸ����������ݽ������ݵ��ֽڳ���
*		itemsize -- ���е�Ԫ���ݵĴ�С
*		queueBlockTime -- ��������ʱ��
*Output��None
*Author��xsx
*Data��2016��4��22��15:33:38
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

