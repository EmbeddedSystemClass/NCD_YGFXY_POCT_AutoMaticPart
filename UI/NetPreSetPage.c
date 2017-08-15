/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"NetPreSetPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"MyMem.h"

#include	"SystemSetPage.h"
#include	"ServerSetPage.h"
#include	"NetSetPage.h"
#include	"WifiSetPage.h"
#include	"NetInfoPage.h"
#include	"SleepPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static NetPrePageBuffer * S_NetPrePageBuffer = NULL;
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
*FunctionName: createSelectUserActivity
*Description: ����ѡ������˽���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyRes createNetPreActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "NetPreActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	if(S_NetPrePageBuffer)
	{

	}
	
	SelectPage(108);
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
	if(S_NetPrePageBuffer)
	{
		/*����*/
		S_NetPrePageBuffer->lcdinput[0] = pbuf[4];
		S_NetPrePageBuffer->lcdinput[0] = (S_NetPrePageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_NetPrePageBuffer->lcdinput[0] == 0x1E00)
		{
			backToFatherActivity();
		}
		
		/*����������*/
		else if(S_NetPrePageBuffer->lcdinput[0] == 0x1E01)
		{
			startActivity(createNetSetActivity, NULL, NULL);
		}
		/*wifi����*/
		else if(S_NetPrePageBuffer->lcdinput[0] == 0x1E02)
		{
			startActivity(createWifiSetActivity, NULL, NULL);
		}
		//����������
		else if(S_NetPrePageBuffer->lcdinput[0] == 0x1FA0)
		{
			startActivity(createServerSetActivity, NULL, NULL);
		}
		//�鿴������Ϣ
		else if(S_NetPrePageBuffer->lcdinput[0] == 0x1E03)
		{
			startActivity(createNetInfoActivity, NULL, NULL);
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
	if(S_NetPrePageBuffer)
	{

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
	if(S_NetPrePageBuffer)
	{

	}
	
	SelectPage(108);
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
	if(NULL == S_NetPrePageBuffer)
	{
		S_NetPrePageBuffer = MyMalloc(sizeof(NetPrePageBuffer));
		
		if(S_NetPrePageBuffer)
		{
			memset(S_NetPrePageBuffer, 0, sizeof(NetPrePageBuffer));
	
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
	MyFree(S_NetPrePageBuffer);
	S_NetPrePageBuffer = NULL;
}



