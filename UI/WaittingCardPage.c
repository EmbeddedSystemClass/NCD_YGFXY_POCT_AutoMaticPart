/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"WaittingCardPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"CardCheck_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"

#include	"LunchPage.h"
#include	"SampleIDPage.h"
#include	"PreReadCardPage.h"
#include	"Motor_Fun.h"
#include	"PlaySong_Task.h"

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
	if(S_WaitPageData)
	{
		MotorMoveTo(WaittingCardLocation, 1);
		
		S_WaitPageData->currenttestdata = GetCurrentTestItem();
		S_WaitPageData->currenttestdata->statues = status_wait1;
		
		/*���һ��ʱ����ʾ�忨*/
		timer_set(&(S_WaitPageData->timer2), 50);
		
		//����޿�����ʾ�忨
		if(CardPinIn == NoCard)
			AddNumOfSongToList(11, 0);
	}
	
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
	if(S_WaitPageData)
	{
		/*����*/
		S_WaitPageData->lcdinput[0] = pbuf[4];
		S_WaitPageData->lcdinput[0] = (S_WaitPageData->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_WaitPageData->lcdinput[0] == 0x1303)
		{
			S_WaitPageData->currenttestdata->statues = status_sample;
			stopPlay();
			backToFatherActivity();
		}
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
	/*�Ƿ�忨*/
	if(CardPinIn == CardIN)
	{
		stopPlay();
		S_WaitPageData->currenttestdata->statues = status_preread;
		startActivity(createPreReadCardActivity, NULL, NULL);
		return;
	}
	/*ʱ�䵽��δ�忨������*/
	else 
	{
		/*��ʾ�忨*/
		if(TimeOut == timer_expired(&(S_WaitPageData->timer2)))
		{
			AddNumOfSongToList(11, 0);
			timer_restart(&(S_WaitPageData->timer2));
		}
	}
	
	//����Ŷ��У��п��ӽ�����ʱ�䣬��ɾ����ǰ���Դ������񣬷���
	if(GetMinWaitTime() < 40)
	{
		stopPlay();
		MotorMoveTo(MaxLocation, 1);
		DeleteCurrentTest();
		
		backToActivity(paiduiActivityName);
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

