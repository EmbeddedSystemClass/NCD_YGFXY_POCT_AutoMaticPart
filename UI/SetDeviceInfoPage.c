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
static void clearDeviceInfoText(void);
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
	page->myDeviceLock = getMyDeviceLock();
	page->isLocked = false;
	timer_SetAndStart(&page->timer, 1);
	
	clearDeviceInfoText();
	
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
		if(page->ismodify && page->isLocked)
		{		
			memcpy(&(page->device.operator), &(page->operator), OneOperatorStructSize);
		
			memcpy(page->device.addr, page->deviceunit, DeviceAddrLen);
			
			page->device.crc = CalModbusCRC16Fun(&(page->device), DeviceStructCrcSize, NULL);
			if(My_Pass == SaveDeviceToFile(&page->device))
			{
				SendKeyCode(1);
				page->ismodify = false;
				backToFatherActivity();
			}
			else
				SendKeyCode(2);
		}
	}
	/*����*/
	else if(page->lcdinput[0] == 0x1B10)
	{
		getLcdInputData(page->operator.name, &pbuf[7]);
		page->ismodify = true;
	}
	/*����*/
	else if(page->lcdinput[0] == 0x1B20)
	{
		getLcdInputData(page->operator.age, &pbuf[7]);
		page->ismodify = true;
	}
	/*�Ա�*/
	else if(page->lcdinput[0] == 0x1B30)
	{
		getLcdInputData(page->operator.sex, &pbuf[7]);
		page->ismodify = true;
	}
	/*��ϵ��ʽ*/
	else if(page->lcdinput[0] == 0x1B40)
	{
		getLcdInputData(page->operator.phone, &pbuf[7]);
		page->ismodify = true;
	}
	/*ְλ*/
	else if(page->lcdinput[0] == 0x1B50)
	{
		getLcdInputData(page->operator.job, &pbuf[7]);
		page->ismodify = true;
	}
	/*��ע*/
	else if(page->lcdinput[0] == 0x1B60)
	{
		getLcdInputData(page->operator.department, &pbuf[7]);
		page->ismodify = true;
	}
	/*�豸ʹ�õ�ַ*/
	else if(page->lcdinput[0] == 0x1B70)
	{
		getLcdInputData(page->deviceunit, &pbuf[7]);
		page->ismodify = true;
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
	if(TimerOut == timer_expired(&page->timer))
	{
		//�ȸ��豸��Ϣ�ļ���������ֹ�����߳��޸�����
		if((page->isLocked == false) && (My_Pass == LockObject(page->myDeviceLock, &page, 1)))
		{
			/*��ȡ���в�����*/
			ReadDeviceFromFile(&(page->device));
			
			//�����ݿ�������ʱ������
			memcpy(&page->operator, &page->device.operator, OneOperatorStructSize);
			memcpy(page->deviceunit, page->device.addr, DeviceAddrLen);
			
			showDeviceInfoText();
			
			page->isLocked = true;
		}
		
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
	snprintf(page->tempBuf, 50, "%s", page->device.operator.name);
	DisText(0x1b10, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 5, "%s", page->device.operator.age);
	DisText(0x1b20, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 6, "%s", page->device.operator.sex);
	DisText(0x1b30, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 15, "%s", page->device.operator.phone);
	DisText(0x1b40, page->tempBuf, strlen(page->tempBuf)+1);
	
	snprintf(page->tempBuf, 20, "%s", page->device.operator.job);
	DisText(0x1b50, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 50, "%s", page->device.operator.department);
	DisText(0x1b60, page->tempBuf, strlen(page->tempBuf)+1);
	
	snprintf(page->tempBuf, 100, "%s", page->device.addr);
	DisText(0x1b70, page->tempBuf, strlen(page->tempBuf)+1);
}

static void clearDeviceInfoText(void)
{
	ClearText(0x1b10);

	ClearText(0x1b20);

	ClearText(0x1b30);

	ClearText(0x1b40);
	
	ClearText(0x1b50);

	ClearText(0x1b60);
	
	ClearText(0x1b70);
}

