/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SetDeviceIDPage.h"

#include	"LCD_Driver.h"
#include	"System_Data.h"
#include	"MyMem.h"
#include	"ShowDeviceInfoPage.h"
#include	"SleepPage.h"
#include	"ReadBarCode_Fun.h"
#include	"SystemSet_Dao.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static SetDeviceIDPage * S_SetDeviceIDPage = NULL;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void dspDeviceId(char * idStr);

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
MyRes createSetDeviceIDActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SetDeviceIDActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	if(S_SetDeviceIDPage)
	{
		memcpy(&(S_SetDeviceIDPage->systemSetData), getGBSystemSetData(), SystemSetDataStructSize);
		
		while(ReadBarCodeFunction((char *)(S_SetDeviceIDPage->tempbuf), 100) > 0)
			;
		dspDeviceId(S_SetDeviceIDPage->systemSetData.deviceId);
	}
	
	SelectPage(104);
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
	if(S_SetDeviceIDPage)
	{
		/*����*/
		S_SetDeviceIDPage->lcdinput[0] = pbuf[4];
		S_SetDeviceIDPage->lcdinput[0] = (S_SetDeviceIDPage->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_SetDeviceIDPage->lcdinput[0] == 0x1c00)
		{
			backToFatherActivity();
		}
		/*ȷ��*/
		else if(S_SetDeviceIDPage->lcdinput[0] == 0x1C01)
		{
			if(S_SetDeviceIDPage->ismodify == 1)
			{
				//��ȡ���µ�ϵͳ����
				memcpy(&(S_SetDeviceIDPage->systemSetData), getGBSystemSetData(), SystemSetDataStructSize);
				
				//���¸����е�is
				memcpy(S_SetDeviceIDPage->systemSetData.deviceId, S_SetDeviceIDPage->deviceId, DeviceIdLen);
				if(My_Pass == SaveSystemSetData(&(S_SetDeviceIDPage->systemSetData)))
				{
					SendKeyCode(1);

					S_SetDeviceIDPage->ismodify = 0;
				}
				else
					SendKeyCode(2);
			}
		}
		/*id����*/
		else if(S_SetDeviceIDPage->lcdinput[0] == 0x1C10)
		{
			memset(S_SetDeviceIDPage->deviceId, 0 , DeviceIdLen);
			
			if(DeviceIdLen >= GetBufLen(&pbuf[7] , 2*pbuf[6]))
				memcpy(S_SetDeviceIDPage->deviceId, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			else
				memcpy(S_SetDeviceIDPage->deviceId, &pbuf[7], DeviceIdLen);
				
			S_SetDeviceIDPage->ismodify = 1;
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
	//��ȡ�豸id����
	if(ReadBarCodeFunction((char *)(S_SetDeviceIDPage->tempbuf), 100) > 0)
	{
		memcpy(S_SetDeviceIDPage->deviceId, S_SetDeviceIDPage->tempbuf, DeviceIdLen);
			
		dspDeviceId(S_SetDeviceIDPage->deviceId);
			
		S_SetDeviceIDPage->ismodify = 1;
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
	SelectPage(104);
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
	if(NULL == S_SetDeviceIDPage)
	{
		S_SetDeviceIDPage = MyMalloc(sizeof(SetDeviceIDPage));
		
		if(S_SetDeviceIDPage)
		{
			memset(S_SetDeviceIDPage, 0, sizeof(SetDeviceIDPage));
	
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
	MyFree(S_SetDeviceIDPage);
	S_SetDeviceIDPage = NULL;
}


static void dspDeviceId(char * idStr)
{
	sprintf(S_SetDeviceIDPage->tempbuf, "%s\0", idStr);
	DisText(0x1C10, S_SetDeviceIDPage->tempbuf, strlen(S_SetDeviceIDPage->tempbuf)+1);
}
