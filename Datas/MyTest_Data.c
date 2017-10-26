/***************************************************************************************************
*FileName : TestLinks_Data
*Description: 测试数据链表
*Author:xsx
*Data: 2016年5月14日17:14:02
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
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
/**************************************变量*************************************************/
/***************************************************************************************************/
static PaiduiUnitData * (PaiduiTestDataBuffer[PaiDuiWeiNum]);					//测试数据缓存区

static PaiduiUnitData * CurrentTestDataBuffer;									//当前测试

static Operator lastOperator;											//上次选择的操作人
/***************************************************************************************************/
/**************************************内部函数*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
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
			//申请测试内存
			PaiduiTestDataBuffer[i] = (PaiduiUnitData *)MyMalloc(PaiduiUnitDataStructSize);
				
			//内存申请失败
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
				
				//保存最新的操作人
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
*Description:  判断是否有卡处于超时阶段
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
*Description:  判断是不是超时阶段
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017年3月22日 11:31:48
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
*Description: 读写当前测试缓存
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月2日15:54:25
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
*Description: 删除当前测试
*Input: None
*Output: None
*Return: My_Pass -- 删除成功
*Author: xsx
*Date: 2016年12月2日15:55:07
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

