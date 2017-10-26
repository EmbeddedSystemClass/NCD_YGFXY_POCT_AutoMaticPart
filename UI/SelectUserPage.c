/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SelectUserPage.h"
#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"MyTest_Data.h"
#include	"CRC16.h"
#include	"UserMPage.h"
#include	"DeviceDao.h"
#include	"Intent.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static UserPageBuffer * page = NULL;

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

static void ShowList(void);
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
MyRes createSelectUserActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SelectUserActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		if(pram)
		{
			readIntent(pram, &page->targetOperator);
		}
		
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
	/*��ȡ�豸��Ϣ*/
	ReadDeviceFromFile(&page->device);
	
	page->pageindex = 0;
	
	ShowList();
	
	SelectPage(84);
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
	if(page)
	{
		/*����*/
		page->lcdinput[0] = pbuf[4];
		page->lcdinput[0] = (page->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(page->lcdinput[0] == 0x1200)
		{
			page->currenttestdata = GetCurrentTestItem();
			if(NULL != page->currenttestdata)
			{
				DeleteCurrentTest();
			}

			backToFatherActivity();
		}
		
		/*�Ϸ�Ҳ*/
		else if(page->lcdinput[0] == 0x1203)
		{			
			if(page->pageindex > 0)
			{
				page->pageindex--;
						
				ShowList();
			}
		}
		/*�·�ҳ*/
		else if(page->lcdinput[0] == 0x1204)
		{			
			if((page->pageindex + 1) < MaxPageNum)
			{
				page->tempOperator = &page->device.operators[(page->pageindex + 1)*MaxPageShowOperatorSize];
			
				if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
				{
					page->pageindex++;
						
					ShowList();
				}
			}
		}
		/*ȷ��*/
		else if(page->lcdinput[0] == 0x1201)
		{
			if(page->targetOperator->crc == CalModbusCRC16Fun(page->targetOperator, OneOperatorStructCrcSize, NULL))
				gotoChildActivity(NULL, NULL);
			else
				SendKeyCode(1);
		}
		/*ѡ�������*/
		else if((page->lcdinput[0] >= 0x1205)&&(page->lcdinput[0] <= 0x1209))
		{			
			page->tempOperator = &page->device.operators[(page->pageindex)*MaxPageShowOperatorSize + page->lcdinput[0] - 0x1205];
		
			if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
			{
				page->tempV1 = page->lcdinput[0] - 0x1205+1;
				BasicPic(0x1240, 0, 137, 11, 10, 303, 79, 363, 141+(page->tempV1-1)*72);
				BasicPic(0x1240, 1, 137, 11, 10, 303, 79, 363, 141+(page->tempV1-1)*72);
				
				memcpy(page->targetOperator, page->tempOperator, OneOperatorStructSize);
			}
		}
		//�༭������
		if(page->lcdinput[0] == 0x120a)
		{
			startActivity(createUserManagerActivity, NULL, NULL);
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
	/*��ȡ���в�����*/
	ReadDeviceFromFile(&page->device);
	
	page->pageindex = 0;
	
	ShowList();
	
	SelectPage(84);
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
		page = MyMalloc(sizeof(UserPageBuffer));
		
		if(page)	
		{
			memset(page, 0, sizeof(UserPageBuffer));
	
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



/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��ShowList
*Description����ʾ�б�����
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:44:00
***************************************************************************************************/
static void ShowList(void)
{
	page->tempV1 = page->pageindex*MaxPageShowOperatorSize;

	page->tempOperator = &(page->device.operators[page->tempV1]);
	
	//��ʾ�б�����
	for(page->tempV1=0; page->tempV1<MaxPageShowOperatorSize; page->tempV1++)
	{
		if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
			snprintf(page->tempBuf, OperatorNameLen, "%s", page->tempOperator->name);
		else
			memset(page->tempBuf, 0, 10);
		
		DisText(0x1210+8*page->tempV1, page->tempBuf, strlen(page->tempBuf)+1);
		
		page->tempOperator++;
	}
	
	BasicPic(0x1240, 0, 137, 11, 10, 303, 79, 363, 141+(page->tempV1-1)*72);
}

