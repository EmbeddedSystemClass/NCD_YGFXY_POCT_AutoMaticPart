/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SetDeviceInfoPage.h"

#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"System_Data.h"
#include	"DeviceDao.h"
#include	"CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static SetDeviceInfoPageBuffer * page = NULL;
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
MyRes createSetDeviceInfoActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SetDeviceInfoActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		if(pram)
		{
			readIntent(pram, &page->device);
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
	showDeviceInfoText();
	
	updateSystemWorkStatus(SystemDeviceInfoModifying, ON);
	
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
		if(My_Pass == SaveDeviceToFile(page->device))
		{
			setSystemDeviceAddr(page->device->addr);
			setSystemDeviceIsNew(true);
			
			SendKeyCode(1);
		}
		else
			SendKeyCode(2);
	}
	/*����*/
	else if(page->lcdinput[0] == 0x1B10)
	{
		getLcdInputData(page->device->operator.name, &pbuf[7]);
	}
	/*����*/
	else if(page->lcdinput[0] == 0x1B20)
	{
		getLcdInputData(page->device->operator.age, &pbuf[7]);
	}
	/*�Ա�*/
	else if(page->lcdinput[0] == 0x1B30)
	{
		getLcdInputData(page->device->operator.sex, &pbuf[7]);
	}
	/*��ϵ��ʽ*/
	else if(page->lcdinput[0] == 0x1B40)
	{
		getLcdInputData(page->device->operator.phone, &pbuf[7]);
	}
	/*ְλ*/
	else if(page->lcdinput[0] == 0x1B50)
	{
		getLcdInputData(page->device->operator.job, &pbuf[7]);
	}
	/*��ע*/
	else if(page->lcdinput[0] == 0x1B60)
	{
		getLcdInputData(page->device->operator.department, &pbuf[7]);
	}
	/*�豸ʹ�õ�ַ*/
	else if(page->lcdinput[0] == 0x1B70)
	{
		getLcdInputData(page->device->addr, &pbuf[7]);
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
	updateSystemWorkStatus(SystemDeviceInfoModifying, OFF);
	UnLockObject(page->myDeviceLock, &page);
	
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
		page = MyMalloc(sizeof(SetDeviceInfoPageBuffer));
		
		if(page)
		{
			memset(page, 0, sizeof(SetDeviceInfoPageBuffer));
	
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
	snprintf(page->tempBuf, 50, "%s", page->device->operator.name);
	DisText(0x1b10, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 5, "%s", page->device->operator.age);
	DisText(0x1b20, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 6, "%s", page->device->operator.sex);
	DisText(0x1b30, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 15, "%s", page->device->operator.phone);
	DisText(0x1b40, page->tempBuf, strlen(page->tempBuf)+1);
	
	snprintf(page->tempBuf, 20, "%s", page->device->operator.job);
	DisText(0x1b50, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 50, "%s", page->device->operator.department);
	DisText(0x1b60, page->tempBuf, strlen(page->tempBuf)+1);
	
	snprintf(page->tempBuf, 100, "%s", page->device->addr);
	DisText(0x1b70, page->tempBuf, strlen(page->tempBuf)+1);
}

