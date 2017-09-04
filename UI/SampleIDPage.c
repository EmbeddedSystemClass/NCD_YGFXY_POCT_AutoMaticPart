/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SampleIDPage.h"
#include	"WaittingCardPage.h"

#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"System_Data.h"
#include	"MyMem.h"
#include	"MyTools.h"
#include	"MyTest_Data.h"
#include	"CodeScanner_Driver.h"
#include	"MyTest_Data.h"
#include	"SelectUserPage.h"
#include	"WaittingCardPage.h"
#include	"ReadBarCode_Fun.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static SampleIDPage *S_SampleIDPage = NULL;
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

static void RefreshSampleID(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: ����ѡ������˽���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyRes createSampleActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SampleActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	S_SampleIDPage->currenttestdata = GetCurrentTestItem();
	
	while(ReadBarCodeFunction((char *)(S_SampleIDPage->tempbuf), 100) > 0)
		;
		
	RefreshSampleID();
	
	SelectPage(86);
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
	if(S_SampleIDPage)
	{
		/*����*/
		S_SampleIDPage->lcdinput[0] = pbuf[4];
		S_SampleIDPage->lcdinput[0] = (S_SampleIDPage->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_SampleIDPage->lcdinput[0] == 0x1300)
		{
			backToFatherActivity();
		}
		
		/*ȷ��*/
		else if(S_SampleIDPage->lcdinput[0] == 0x1301)
		{
			if(strlen(S_SampleIDPage->currenttestdata->testData.sampleid) == 0)
				SendKeyCode(1);
			else
				startActivity(createWaittingCardActivity, NULL, NULL);
		}
		/*��ȡ�����id*/
		else if(S_SampleIDPage->lcdinput[0] == 0x1310)
		{
			memset(S_SampleIDPage->currenttestdata->testData.sampleid, 0, MaxSampleIDLen);
			memcpy(S_SampleIDPage->currenttestdata->testData.sampleid, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
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
	//������ǹ��ȡ��Ʒ���
	if(ReadBarCodeFunction((char *)(S_SampleIDPage->tempbuf), 100) > 0)
	{
		memcpy(S_SampleIDPage->currenttestdata->testData.sampleid, S_SampleIDPage->tempbuf, MaxSampleIDLen);

		startActivity(createWaittingCardActivity, NULL, NULL);
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
	RefreshSampleID();
	
	SelectPage(86);
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
	if(NULL == S_SampleIDPage)
	{
		S_SampleIDPage = MyMalloc(sizeof(SampleIDPage));
		
		if(S_SampleIDPage)
		{
			memset(S_SampleIDPage, 0, sizeof(SampleIDPage));
	
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
	MyFree(S_SampleIDPage);
	S_SampleIDPage = NULL;
}



static void RefreshSampleID(void)
{
	snprintf(S_SampleIDPage->tempbuf, MaxSampleIDLen, "%s", S_SampleIDPage->currenttestdata->testData.sampleid);
	DisText(0x1310, S_SampleIDPage->tempbuf, strlen(S_SampleIDPage->tempbuf)+1);
}
