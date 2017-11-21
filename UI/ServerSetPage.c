/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"ServerSetPage.h"
#include	"LCD_Driver.h"
#include	"SystemSet_Dao.h"
#include	"NetPreSetPage.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"MyTools.h"
#include	"SleepPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static ServerSetPageBuffer * S_ServerSetPageBuffer = NULL;
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
MyRes createServerSetActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "ServerSetActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	memcpy(&(S_ServerSetPageBuffer->serverSet), &(getGBSystemSetData()->serverSet), ServerSetStructSize);
		
	/*����ip*/
	snprintf((S_ServerSetPageBuffer->buf), 16, "%d.%d.%d.%d", S_ServerSetPageBuffer->serverSet.serverIP.ip_1, S_ServerSetPageBuffer->serverSet.serverIP.ip_2, 
		S_ServerSetPageBuffer->serverSet.serverIP.ip_3, S_ServerSetPageBuffer->serverSet.serverIP.ip_4);
	DisText(0x1fb0, S_ServerSetPageBuffer->buf, strlen(S_ServerSetPageBuffer->buf)+1);
		
	/*���¶˿ں�*/
	snprintf((S_ServerSetPageBuffer->buf), 5, "%d", S_ServerSetPageBuffer->serverSet.serverPort);
	DisText(0x1fc0, S_ServerSetPageBuffer->buf, strlen(S_ServerSetPageBuffer->buf)+1);
	
	SelectPage(148);
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
	S_ServerSetPageBuffer->lcdinput[0] = pbuf[4];
	S_ServerSetPageBuffer->lcdinput[0] = (S_ServerSetPageBuffer->lcdinput[0]<<8) + pbuf[5];

	/*����IP*/
	if(S_ServerSetPageBuffer->lcdinput[0] == 0x1FB0)
	{
		memset(S_ServerSetPageBuffer->buf, 0, 50);
		memcpy(S_ServerSetPageBuffer->buf, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
		
		if(My_Pass == parseIpString(&S_ServerSetPageBuffer->serverSet.serverIP, S_ServerSetPageBuffer->buf))
			S_ServerSetPageBuffer->ischanged = 1;
		else
		{
			memset(&S_ServerSetPageBuffer->serverSet.serverIP, 0, 4);
			ClearText(0x1FB0);
			SendKeyCode(2);
		}
	}
	/*���ö˿ں�*/
	else if(S_ServerSetPageBuffer->lcdinput[0] == 0x1FC0)
	{
		memset(S_ServerSetPageBuffer->buf, 0, 50);
		memcpy(S_ServerSetPageBuffer->buf, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
		
		if(My_Pass == parsePortString(&S_ServerSetPageBuffer->serverSet.serverPort, S_ServerSetPageBuffer->buf))
			S_ServerSetPageBuffer->ischanged = 1;
		else
		{
			S_ServerSetPageBuffer->serverSet.serverPort = 0;
			ClearText(0x1FC0);
			SendKeyCode(2);
		}
	}
	/*ȷ���޸�*/
	else if(S_ServerSetPageBuffer->lcdinput[0] == 0x1FA2)
	{
		if(1 == S_ServerSetPageBuffer->ischanged)
		{
			readGbSystemSetData(&S_ServerSetPageBuffer->systemSetData);

			memcpy(&S_ServerSetPageBuffer->systemSetData.serverSet, &S_ServerSetPageBuffer->serverSet, ServerSetStructSize);
				
			if(My_Pass == SaveSystemSetData(&(S_ServerSetPageBuffer->systemSetData)))
			{
				SendKeyCode(1);

				S_ServerSetPageBuffer->ischanged = 0;
			}
			else
				SendKeyCode(2);
		}
	}
	/*����*/
	else if(S_ServerSetPageBuffer->lcdinput[0] == 0x1FA1)
	{
		backToFatherActivity();
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

	SelectPage(148);
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
	if(NULL == S_ServerSetPageBuffer)
	{
		S_ServerSetPageBuffer = MyMalloc(sizeof(ServerSetPageBuffer));
		
		if(S_ServerSetPageBuffer)
		{
			memset(S_ServerSetPageBuffer, 0, sizeof(ServerSetPageBuffer));
	
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
	MyFree(S_ServerSetPageBuffer);
	S_ServerSetPageBuffer = NULL;
}

