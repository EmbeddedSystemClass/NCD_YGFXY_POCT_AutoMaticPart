/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"WelcomePage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"

#include	"LunchPage.h"
#include	"Test_Task.h"
#include	"CodeScan_Task.h"
#include	"Paidui_Task.h"
#include 	"netconf.h"

#include	<string.h>
#include	"stdio.h"
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static WelcomePageBuffer * page = NULL;
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
*FunctionName: createWelcomeActivity
*Description: ������ӭ����
*Input: thizActivity -- ��ǰ����
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:21:51
***************************************************************************************************/
MyRes createWelcomeActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "WelcomeActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: ��ʾ��ǰ����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:22:23
***************************************************************************************************/
static void activityStart(void)
{
	timer_SetAndStart(&(page->timer), 2);

	page->canAram = true;
	SelectPage(1);
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: ��ǰ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:22:42
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	if(page)
	{
		/*����*/
		page->lcdinput[0] = pbuf[4];
		page->lcdinput[0] = (page->lcdinput[0]<<8) + pbuf[5];
		
		page->lcdinput[1] = pbuf[6];
		page->lcdinput[1] = (page->lcdinput[1]<<8) + pbuf[7];
		
		if(0x81 == pbuf[3])
		{
			//ҳ��id
			if(0x03 == pbuf[4])
			{
				page->currentPageId = page->lcdinput[1];	
			}
		}
		else if(0x83 == pbuf[3])
		{
			if((page->lcdinput[0] >= 0x1010) && (page->lcdinput[0] <= 0x1014))
				while(1);
		}
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: ��ǰ����ˢ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:22:56
***************************************************************************************************/
static void activityFresh(void)
{
	if(80 == page->currentPageId)
	{
		page->selfTestStatus = getSelfTestStatus();
		
		if(page->canAram == false)
			return;
		
		if(page->selfTestStatus != SelfTestting)
			page->canAram = false;

		//�Լ����
		if(SelfTest_OK == page->selfTestStatus)
		{					
			SetLEDLight(getGBSystemSetData()->ledLightIntensity);
			
			StartvTestTask();
				
			//��������ά������
			StartCodeScanTask();
			
			//��ʼ�Ŷ�����
			StartPaiduiTask();
			
			StartEthernet();
					
			destroyTopActivity();
			startActivity(createLunchActivity, NULL, NULL);
				
			return;
		}
		//�������ݴ���˵��sd�쳣
		else if(SD_ERROR == page->selfTestStatus)
		{
			SelectPage(81);
				
			vTaskDelay(1000 / portTICK_RATE_MS);
				
			SendKeyCode(5);
		}
		//led�쳣���澯����ģ�����
		else if(Light_Error == page->selfTestStatus)
		{
			SelectPage(81);
			vTaskDelay(1000 / portTICK_RATE_MS);
			SendKeyCode(4);
		}
		//�ɼ��쳣���澯�ɼ�ģ�����
		else if(AD_ERROR == page->selfTestStatus)
		{
			SelectPage(81);
			vTaskDelay(1000 / portTICK_RATE_MS);
			SendKeyCode(3);
		}
		//�����쳣���澯����ģ�����
		else if(Motol_ERROR == page->selfTestStatus)
		{
			SelectPage(81);
			vTaskDelay(1000 / portTICK_RATE_MS);
			SendKeyCode(1);
		}
	}
	else if(TimerOut == timer_expired(&(page->timer)))
	{
		ReadCurrentPageId();
		
		timer_restart(&(page->timer));
	}
}


static void activityHide(void)
{

}

static void activityResume(void)
{
	
}

/***************************************************************************************************
*FunctionName: activityFinish
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static void activityDestroy(void)
{
	activityBufferFree();
}


/***************************************************************************************************
*FunctionName��PageBufferMalloc
*Description����ǰ������ʱ��������
*Input��None
*Output��MyRes -- ���سɹ����
*Author��xsx
*Data��2016��6��27��08:56:02
***************************************************************************************************/
static MyRes activityBufferMalloc(void)
{
	if(NULL == page)
	{
		page = (WelcomePageBuffer *)MyMalloc(sizeof(WelcomePageBuffer));
			
		if(page)
		{
			memset(page, 0, sizeof(WelcomePageBuffer));
		
			return My_Pass;
			
		}
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName��PageBufferFree
*Description����ǰ������ʱ�����ͷ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��08:56:21
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(page);
	page = NULL;
}


