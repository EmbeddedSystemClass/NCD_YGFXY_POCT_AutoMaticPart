/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"LunchPage.h"

#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"SystemSetPage.h"
#include	"SelectUserPage.h"
#include	"SampleIDPage.h"
#include	"PaiDuiPage.h"
#include	"RecordPage.h"
#include	"SleepPage.h"
#include	"PaiDuiPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static LunchPageBuffer * page = NULL;
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

static void DspPageText(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName: createLunchActivity
*Description: ����������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyRes createLunchActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, lunchActivityName, activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	timer_SetAndStart(&page->timer, getGBSystemSetData()->ledSleepTime);
	
	SelectPage(82);
	
	DspPageText();
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
	/*����*/
	page->lcdinput[0] = pbuf[4];
	page->lcdinput[0] = (page->lcdinput[0]<<8) + pbuf[5];
	
	//��������ʱ��
	timer_restart(&(page->timer));
		
	//����
	if(page->lcdinput[0] == 0x1103)
	{
		startActivity(createSystemSetActivity, NULL, NULL);
	}
	//�鿴����
	else if(page->lcdinput[0] == 0x1102)
	{
		startActivity(createRecordActivity, NULL, NULL);
	}
	//���ڰ�����һ�ΰ���
	else if(page->lcdinput[0] == 0x1106)
		page->presscount = 0;
	//���ڰ�����������
	else if(page->lcdinput[0] == 0x1107)
		page->presscount++;
	//���ڰ����ɿ�
	else if(page->lcdinput[0] == 0x1108)
	{
		if(page->presscount > 10)
			startActivity(createPaiDuiActivity, NULL, NULL);
		else
		{
			page->error = CreateANewTest(&page->currentTestDataBuffer);
			//�����ɹ�
			if(Error_OK == page->error)
			{
				page->motorAction.motorActionEnum = WaitCardPutInDef;
				page->motorAction.motorParm = page->currentTestDataBuffer->cardLocation;
				if(My_Pass != StartMotorAction(&page->motorAction, true, 1, 0/portTICK_RATE_MS))
					SendKeyCode(2);
				else
				{
					SendKeyCode(1);
					page->isCreate = true;
				}
			}
			else if(Error_PaiduiFull == page->error)
				startActivity(createPaiDuiActivity, NULL, NULL);
			else
				SendKeyCode(2);
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
	if(page->isCreate && isMotorMoveEnd(0 / portTICK_RATE_MS))
	{
		page->isCreate = false;
		page->tempOperator = &page->currentTestDataBuffer->testData.operator;
		startActivity(createSelectUserActivity, createIntent(&(page->tempOperator), 4), createSampleActivity);
	}
	
	if(TimerOut == timer_expired(&(page->timer)))
	{
		//startActivity(createSleepActivity, NULL, NULL);
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
page->isCreate = false;
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
	page->currentTestDataBuffer = NULL;
	
	timer_restart(&(page->timer));
	page->isCreate = false;
	SelectPage(82);
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
	//�����ǰҳ��ĸ澯������
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
	if(NULL == page)
	{
		page = MyMalloc(sizeof(LunchPageBuffer));
		if(page)	
		{
			memset(page, 0, sizeof(LunchPageBuffer));
	
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
	MyFree(page);
	page = NULL;
}

/***************************************************************************************************
*FunctionName: DspPageText
*Description: ��ǰ����������ʾ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:03:42
***************************************************************************************************/
static void DspPageText(void)
{
	sprintf(page->buf, "V%d.%d.%02d", GB_SoftVersion/1000, GB_SoftVersion%1000/100, GB_SoftVersion%100);
	DisText(0x1110, page->buf, strlen(page->buf)+1);
}

