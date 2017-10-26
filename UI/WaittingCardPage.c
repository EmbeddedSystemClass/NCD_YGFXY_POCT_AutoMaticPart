/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"WaittingCardPage.h"
#include	"PreReadCardPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"CardCheck_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"Motor_Fun.h"

#include	"LunchPage.h"
#include	"PaiDuiPage.h"
#include	"SampleIDPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static WaitPageData * S_WaitPageData = NULL;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyRes activityBufferMalloc(void);
static void activityBufferFree(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName: createWaittingCardActivity
*Description: �����ȴ��忨����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyRes createWaittingCardActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "WaittingCardActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: ��ʾ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:32
***************************************************************************************************/
static void activityStart(void)
{	
	S_WaitPageData->currenttestdata = GetCurrentTestItem();
	S_WaitPageData->motorAction.motorActionEnum = OriginLocationDef;
	S_WaitPageData->motorAction.motorParm = S_WaitPageData->currenttestdata->cardLocation;
	S_WaitPageData->isMotorStartted = My_Fail;
	S_WaitPageData->isBack = false;
	
	timer_SetAndStart(&S_WaitPageData->timer2, 90);							//90S���忨��ȡ����ǰ����
	
	SelectPage(88);
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	S_WaitPageData->lcdinput[0] = pbuf[4];
	S_WaitPageData->lcdinput[0] = (S_WaitPageData->lcdinput[0]<<8) + pbuf[5];
		
	//����
	if(S_WaitPageData->lcdinput[0] == 0x1303)
	{
		memset(S_WaitPageData->currenttestdata->testData.sampleid, 0, MaxSampleIDLen);
		S_WaitPageData->isBackButtonPressed = true;
		S_WaitPageData->isBack = true;
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: ����ˢ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:16
***************************************************************************************************/
static void activityFresh(void)
{
	if(S_WaitPageData->isMotorStartted == My_Pass)
	{
		if(isMotorMoveEnd(0 / portTICK_RATE_MS))
		{
			if(S_WaitPageData->isBackButtonPressed)
				backToFatherActivity();
			else
			{
				DeleteCurrentTest();
		
				backToActivity(lunchActivityName);
							
				if(IsPaiDuiTestting())
					startActivity(createPaiDuiActivity, NULL, NULL);
			}
		}
	}
	else if(S_WaitPageData->isBack)
	{
		S_WaitPageData->isMotorStartted = StartMotorAction(&S_WaitPageData->motorAction, true, false);
		if(S_WaitPageData->isMotorStartted == My_Pass)
			SendKeyCode(1);
	}
	else if(readCaedCheckStatus() == ON)											//�Ƿ�忨
	{
		startActivity(createPreReadCardActivity, NULL, NULL);
		return;
	}
	else if(TimerOut == timer_expired(&(S_WaitPageData->timer2)))
	{
		S_WaitPageData->isBackButtonPressed = false;
		S_WaitPageData->isBack = true;
	}
}

/***************************************************************************************************
*FunctionName: activityHide
*Description: ���ؽ���ʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:40
***************************************************************************************************/
static void activityHide(void)
{
	SendKeyCode(16);
}

/***************************************************************************************************
*FunctionName: activityResume
*Description: ����ָ���ʾʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:58
***************************************************************************************************/
static void activityResume(void)
{
	SelectPage(88);
}

/***************************************************************************************************
*FunctionName: activityDestroy
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:02:15
***************************************************************************************************/
static void activityDestroy(void)
{
	SendKeyCode(16);
	activityBufferFree();
}

/***************************************************************************************************
*FunctionName: activityBufferMalloc
*Description: ���������ڴ�����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static MyRes activityBufferMalloc(void)
{
	if(NULL == S_WaitPageData)
	{
		S_WaitPageData = MyMalloc(sizeof(WaitPageData));
		
		if(S_WaitPageData)
		{
			memset(S_WaitPageData, 0, sizeof(WaitPageData));
	
			return My_Pass;
		}
		else
			return My_Fail;
	}
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: activityBufferFree
*Description: �����ڴ��ͷ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:03:10
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(S_WaitPageData);
	S_WaitPageData = NULL;
}

