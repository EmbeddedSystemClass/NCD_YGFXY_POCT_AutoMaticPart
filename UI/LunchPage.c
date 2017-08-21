/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"LunchPage.h"

#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"
#include	"Usart2_Driver.h"
#include	"Motor1_Fun.h"
#include	"Motor2_Fun.h"
#include	"Motor4_Fun.h"

#include	"SystemSetPage.h"
#include	"SelectUserPage.h"
#include	"SampleIDPage.h"
#include	"PaiDuiPage.h"



#include	"QueueUnits.h"

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
	timer_SetAndStart(&page->timer, 2);//getGBSystemSetData()->ledSleepTime);
	
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
		//startActivity(createRecordActivity, NULL, NULL);
	}
	//�������
	else if(page->lcdinput[0] == 0x1100)
	{	
		
	}
	//��������
	else if(page->lcdinput[0] == 0x1101)
	{
		page->error = CreateANewTest(&page->currentTestDataBuffer);
		page->step = 0;
		//�����ɹ�
		if(Error_OK == page->error)
		{
			page->step = 1;
			motor4MoveTo(Motor4_OpenLocation, 1);
			//startActivity(createSelectUserActivity, NULL, createSampleActivity);
		}
		//����ʧ��
		else if(Error_PaiduiFull == page->error)
			startActivity(createPaiDuiActivity, NULL, NULL);
		else
		{
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
	if((page->step == 1) && (Motor4_OpenLocation == getMotorxLocation(Motor_4)))
	{
		motor2MoveTo(Motor2_MidLocation, 1);
		page->step = 2;
	}
	
	if((page->step == 2) && (Motor2_MidLocation == getMotorxLocation(Motor_2)))
	{
		page->step = 3;
		motor1MoveToNum(page->currentTestDataBuffer->testlocation, 1);
	}
	
	if((page->step == 3) && (page->currentTestDataBuffer->testlocation == getMotorxLocation(Motor_1)))
	{
		page->step = 4;
		motor2MoveTo(Motor2_WaitCardLocation, 1);
	}
		
	if((page->step == 4) && (Motor2_WaitCardLocation == getMotorxLocation(Motor_2)))
	{
		page->step = 0;
		startActivity(createSelectUserActivity, NULL, createSampleActivity);
	}
	
	if(TimerOut == timer_expired(&(page->timer)))
	{
		SendDataToQueue(GetUsart6TXQueue(), NULL, page->buf, 100, 1, 100/portTICK_RATE_MS, 0, EnableUsart6TXInterrupt);
		SendDataToQueue(GetUsart6TXQueue(), NULL, page->buf, 100, 1, 100/portTICK_RATE_MS, 0, EnableUsart6TXInterrupt);
		SendDataToQueue(GetUsart6TXQueue(), NULL, page->buf, 100, 1, 100/portTICK_RATE_MS, 0, EnableUsart6TXInterrupt);
		timer_restart(&(page->timer));
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
	page->currentTestDataBuffer = NULL;
	
	timer_restart(&(page->timer));
	
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
	unsigned char i=0;
	for(i=0; i<100; i++)
		page->buf[i] = 0x11;
	//sprintf(page->buf, "V%d.%d.%02d", GB_SoftVersion/1000, GB_SoftVersion%1000/100, GB_SoftVersion%100);
	
	//for(i=0; i<40; i++)
	//	DisText(0x1110, page->buf, 50);
}

