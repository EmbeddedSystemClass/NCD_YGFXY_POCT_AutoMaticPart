/***************************************************************************************************
*FileName : TestLinks_Data
*Description: ������������
*Author:xsx
*Data: 2016��5��14��17:14:02
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"MyTest_Data.h"
#include	"SystemSet_Data.h"
#include	"System_Data.h"
#include	"Timer.h"

#include	"MyMem.h"
#include	"string.h"
#include 	"FreeRTOS.h"
#include 	"task.h"
/***************************************************************************************************/
/**************************************����*************************************************/
/***************************************************************************************************/
static PaiduiUnitData * (PaiduiTestDataBuffer[PaiDuiWeiNum]);					//�������ݻ�����

static PaiduiUnitData * CurrentTestDataBuffer;									//��ǰ����

static Operator lastOperator;											//�ϴ�ѡ��Ĳ�����
/***************************************************************************************************/
/**************************************�ڲ�����*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

CreateTestErrorType CreateANewTest(PaiduiUnitData ** TestDataBuffer)
{
	unsigned char i=0;
	
	if(CurrentTestDataBuffer != NULL)
		return Error_PaiduiTesting;

	if(DisableCreateNewTestTime > GetMinWaitTime())
		return Error_PaiDuiBusy;
		
	if(true == isSomePaiduiInOutTimeStatus())
		return Error_PaiDuiBusy;
		
	for(i=0; i<PaiDuiWeiNum; i++)
	{
		if(PaiduiTestDataBuffer[i] == NULL)
		{
			//��������ڴ�
			PaiduiTestDataBuffer[i] = (PaiduiUnitData *)MyMalloc(PaiduiUnitDataStructSize);
				
			//�ڴ�����ʧ��
			if(PaiduiTestDataBuffer[i] == NULL)
				return Error_Mem;
			else
			{
				SetCurrentTestItem(PaiduiTestDataBuffer[i]);
				memset(CurrentTestDataBuffer, 0, PaiduiUnitDataStructSize);
				CurrentTestDataBuffer->index = i;
				CurrentTestDataBuffer->cardLocation = 2*i + 1;
				CurrentTestDataBuffer->testLocation = CurrentTestDataBuffer->cardLocation;
				CurrentTestDataBuffer->testLocation += PaiDuiWeiNum;
				if(CurrentTestDataBuffer->testLocation > PaiDuiWeiNum*2)
					CurrentTestDataBuffer->testLocation -= PaiDuiWeiNum*2;
				
				//�������µĲ�����
				if(lastOperator.crc != 0)
					memcpy(&CurrentTestDataBuffer->testData.operator, &lastOperator, OneOperatorStructSize);

				if(TestDataBuffer)
					*TestDataBuffer = CurrentTestDataBuffer;
					
				return Error_OK;
			}
		}
	}
	
	return Error_PaiduiFull;
}

PaiduiUnitData * GetTestItemByIndex(unsigned char index)
{
	if(index >= PaiDuiWeiNum)
		return NULL;
	
	return PaiduiTestDataBuffer[index];
}

void upDateLastOperator(Operator * operator)
{
	if(operator)
		memcpy(&lastOperator, operator, OneOperatorStructSize);
}

/***************************************************************************************************
*FunctionName:  isSomePaiduiInOutTimeStatus
*Description:  �ж��Ƿ��п����ڳ�ʱ�׶�
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 16:20:57
***************************************************************************************************/
bool isSomePaiduiInOutTimeStatus(void)
{
	unsigned char index = 0;
	
	for(index = 0; index < PaiDuiWeiNum; index++)
	{
		if((PaiduiTestDataBuffer[index])&&
			(PaiduiTestDataBuffer[index]->statues == status_timeup))
		{
			return true;
		}
	}
	
	return false;
}

unsigned short GetMinWaitTime(void)
{
	unsigned short min = 0xffff;
	unsigned char index = 0;
	unsigned short temp = 0;
	
	for(index = 0; index < PaiDuiWeiNum; index++)
	{
		if(PaiduiTestDataBuffer[index] && PaiduiTestDataBuffer[index]->statues == status_timedown)
		{
			temp = timer_surplus(&PaiduiTestDataBuffer[index]->timeDown_timer);
			if(temp < min)
				min = temp;
		}
	}
	return min;
}

/***************************************************************************************************
*FunctionName:  isInTimeOutStatus
*Description:  �ж��ǲ��ǳ�ʱ�׶�
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��3��22�� 11:31:48
***************************************************************************************************/
bool isInTimeOutStatus(PaiduiUnitData * paiduiUnitData)
{
	if(paiduiUnitData && (paiduiUnitData->timeUp_timer.start > 0))
		return true;
	else
		return false;
}

/***************************************************************************************************
*FunctionName: SetCurrentTestItem, GetCurrentTestItem
*Description: ��д��ǰ���Ի���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��2��15:54:25
***************************************************************************************************/
void SetCurrentTestItem(PaiduiUnitData * s_PaiduiUnitData)
{
	CurrentTestDataBuffer = s_PaiduiUnitData;
}

PaiduiUnitData * GetCurrentTestItem(void)
{
	return CurrentTestDataBuffer;
}

/***************************************************************************************************
*FunctionName: DeleteCurrentTest
*Description: ɾ����ǰ����
*Input: None
*Output: None
*Return: My_Pass -- ɾ���ɹ�
*Author: xsx
*Date: 2016��12��2��15:55:07
***************************************************************************************************/
MyRes DeleteCurrentTest(void)
{
	if(CurrentTestDataBuffer)
	{
		PaiduiTestDataBuffer[CurrentTestDataBuffer->index] = NULL;

		MyFree(CurrentTestDataBuffer);
		
		SetCurrentTestItem(NULL);
	}
	return My_Pass;
}

bool IsPaiDuiTestting(void)
{
	unsigned char i=0;

	for(i=0; i<PaiDuiWeiNum; i++)
	{
		if(PaiduiTestDataBuffer[i])
			return true;
	}
	
	return false;
}

