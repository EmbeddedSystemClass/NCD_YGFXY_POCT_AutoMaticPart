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
static PaiduiUnitData * (PaiduiTestDataBuffer[PaiDuiWeiNum]);

static PaiduiUnitData * CurrentTestDataBuffer;

static Operator * lastOperator = NULL;
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

	if(40 > GetMinWaitTime())
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
				CurrentTestDataBuffer = PaiduiTestDataBuffer[i];
				memset(CurrentTestDataBuffer, 0, PaiduiUnitDataStructSize);
				CurrentTestDataBuffer->testlocation = i + 1;
				
				//保存最新的操作人
				if(lastOperator != NULL)
					memcpy(&CurrentTestDataBuffer->testData.operator, lastOperator, OneOperatorStructSize);

				lastOperator = &CurrentTestDataBuffer->testData.operator;	
				
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
	return PaiduiTestDataBuffer[index];
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
			(PaiduiTestDataBuffer[index]->timeUp_timer.start != 0))
		{
			return true;
		}
	}
	
	return false;
}

unsigned short GetMinWaitTime(void)
{
	unsigned short min = 0xffff;
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
		PaiduiTestDataBuffer[CurrentTestDataBuffer->testlocation-1] = NULL;

		MyFree(CurrentTestDataBuffer);
		
		CurrentTestDataBuffer = NULL;
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

