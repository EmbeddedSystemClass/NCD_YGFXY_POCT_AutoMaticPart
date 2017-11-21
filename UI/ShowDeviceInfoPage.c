/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"ShowDeviceInfoPage.h"
#include	"SetDeviceIDPage.h"
#include	"SetDeviceInfoPage.h"
#include	"SystemSetPage.h"
#include	"SleepPage.h"
#include	"DeviceDao.h"
#include	"StringDefine.h"

#include	"LCD_Driver.h"
#include	"SystemSet_Dao.h"
#include	"MyMem.h"
#include	"MyTools.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static ShowDeviceInfoPageBuffer * S_ShowDeviceInfoPageBuffer = NULL;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void showDeviceInfo(void);

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
MyRes createDeviceInfoActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "DeviceInfoActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	S_ShowDeviceInfoPageBuffer->tempDevice = &S_ShowDeviceInfoPageBuffer->device;
	ReadDeviceFromFile(&S_ShowDeviceInfoPageBuffer->device);
		
	showDeviceInfo();
	
	SelectPage(100);
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
	if(S_ShowDeviceInfoPageBuffer)
	{
		/*����*/
		S_ShowDeviceInfoPageBuffer->lcdinput[0] = pbuf[4];
		S_ShowDeviceInfoPageBuffer->lcdinput[0] = (S_ShowDeviceInfoPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*������Ϣ*/
		if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a03)
			S_ShowDeviceInfoPageBuffer->presscount = 0;
		
		else if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a04)
			S_ShowDeviceInfoPageBuffer->presscount++;
		
		else if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a05)
		{
			if(S_ShowDeviceInfoPageBuffer->presscount > 10)
				SendKeyCode(2);
		}
		/*��ȡ����*/
		else if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a10)
		{
			S_ShowDeviceInfoPageBuffer->tempValue = GetBufLen(&pbuf[7] , 2*pbuf[6]);
			if((S_ShowDeviceInfoPageBuffer->tempValue == 6) && ( pdPASS == CheckStrIsSame(&pbuf[7], AdminPassWord, 6)))
			{
				startActivity(createSetDeviceIDActivity, createIntent(&S_ShowDeviceInfoPageBuffer->tempDevice, 4), NULL);
			}
			else
				SendKeyCode(1);
		}
		/*����*/
		else if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a00)
		{
			backToFatherActivity();
		}
		/*�޸�*/
		else if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a01)
		{
			startActivity(createSetDeviceInfoActivity, createIntent(&S_ShowDeviceInfoPageBuffer->tempDevice, 4), NULL);
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
	ReadDeviceFromFile(&S_ShowDeviceInfoPageBuffer->device);

	showDeviceInfo();
	
	SelectPage(100);
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
	if(NULL == S_ShowDeviceInfoPageBuffer)
	{
		S_ShowDeviceInfoPageBuffer = MyMalloc(sizeof(ShowDeviceInfoPageBuffer));
		
		if(S_ShowDeviceInfoPageBuffer)
		{
			memset(S_ShowDeviceInfoPageBuffer, 0, sizeof(ShowDeviceInfoPageBuffer));
	
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
	MyFree(S_ShowDeviceInfoPageBuffer);
	S_ShowDeviceInfoPageBuffer = NULL;
}


static void showDeviceInfo(void)
{
	/*��ʾ�豸id*/
	DisText(0x1a40, S_ShowDeviceInfoPageBuffer->device.deviceId, DeviceIdLen);
		
	/*��ʾ�豸����*/
	DisText(0x1a50, "ӫ�����߶���������\0", 19);
			
	/*��ʾʹ�õ�λ*/
	snprintf(S_ShowDeviceInfoPageBuffer->tempBuf, 20, "%s", S_ShowDeviceInfoPageBuffer->device.addr);
	DisText(0x1a60, S_ShowDeviceInfoPageBuffer->tempBuf, strlen(S_ShowDeviceInfoPageBuffer->tempBuf)+1);

	/*��ʾ������*/
	snprintf(S_ShowDeviceInfoPageBuffer->tempBuf, 10, "%s", S_ShowDeviceInfoPageBuffer->device.operator.name);
	DisText(0x1a90, S_ShowDeviceInfoPageBuffer->tempBuf, strlen(S_ShowDeviceInfoPageBuffer->tempBuf)+1);
		
	/*��ʾ��������ϵ��ʽ*/
	snprintf(S_ShowDeviceInfoPageBuffer->tempBuf, 20, "%s", S_ShowDeviceInfoPageBuffer->device.operator.phone);
	DisText(0x1a80, S_ShowDeviceInfoPageBuffer->tempBuf, strlen(S_ShowDeviceInfoPageBuffer->tempBuf)+1);
}

