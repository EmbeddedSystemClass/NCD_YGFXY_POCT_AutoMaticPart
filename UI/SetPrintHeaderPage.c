/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SetPrintHeaderPage.h"

#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"System_Data.h"
#include	"SystemSet_Dao.h"
#include	"CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static SetPrintHeaderPageBuffer * page = NULL;
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

static void showDeviceInfoText(void);
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
MyRes createSetPrintHeaderActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SetPrintHeaderActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
    readPrintInfo(&page->printInfo);
    
	showDeviceInfoText();

	SelectPage(102);
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
		
	/*����*/
	if(page->lcdinput[0] == 0x1B00)
	{
		backToFatherActivity();
	}
	/*ȷ��*/
	else if(page->lcdinput[0] == 0x1B01)
	{			
        if(Bool_True == page->ismodify)
        {
            page->printInfo.crc = CalModbusCRC16Fun(&page->printInfo, sizeof(PrintInfo)-2, NULL);
            
            //��ȡ���µ�ϵͳ����
			readGbSystemSetData(&page->systemSetData);
            memcpy(&page->systemSetData.printInfo, &page->printInfo, sizeof(PrintInfo));
            
            if(My_Pass == SaveSystemSetData(&page->systemSetData))
			{
				SendKeyCode(1);
				page->ismodify = Bool_False;
			}
			else
				SendKeyCode(2);
        }
	}
	/*�豸ʹ�õ�ַ*/
	else if(page->lcdinput[0] == 0x1B70)
	{
        page->tempShort = GetBufLen(&pbuf[7] , 2*pbuf[6]);
        if(page->tempShort > 30)
            page->tempShort = 30;
        
        memset(page->printInfo.header, 0, 30);
        memcpy(page->printInfo.header, &pbuf[7], page->tempShort);
        page->printInfo.crc = CalModbusCRC16Fun(&page->printInfo, sizeof(PrintInfo)-2, NULL);
        
        page->ismodify = Bool_True;
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
	SelectPage(102);
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
		page = MyMalloc(sizeof(SetPrintHeaderPageBuffer));
		
		if(page)
		{
			memset(page, 0, sizeof(SetPrintHeaderPageBuffer));
	
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

static void showDeviceInfoText(void)
{
    DisText(0x1b70, page->printInfo.header, 30);
}

