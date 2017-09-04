/***************************************************************************************************
*FileName: MaintenancePage
*Description: �豸ά��
*Author:xsx
*Data: 2017��6��14�� 15:51:52
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"MaintenancePage.h"

#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"SystemSetPage.h"
#include	"SelectUserPage.h"
#include	"System_Data.h"
#include	"DeviceMaintenanceDao.h"
#include	"Maintenance_Data.h"

#include	"CRC16.h"

#include	<string.h>
#include	"stdio.h"

/***************************************************************************************************/
/**************************************��������*************************************************/
/***************************************************************************************************/
static MaintenancePageBuffer * pageBuffer = NULL;
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);

static MyRes activityBufferMalloc(void);
static void activityBufferFree(void);
static void clearPageText(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: createLunchActivity
*Description: ����������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyRes createMaintenanceActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, MaintenanceActivityName, activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	pageBuffer->deviceMaintenance = getGB_DeviceMaintenance();
	
	clearPageText();
	
	SelectPage(156);
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
	pageBuffer->lcdinput[0] = pbuf[4];
	pageBuffer->lcdinput[0] = (pageBuffer->lcdinput[0]<<8) + pbuf[5];
		
	//����
	if(pageBuffer->lcdinput[0] == 0x3300)
	{
		backToActivity(SystemSetActivityName);
	}
	//�ύ
	else if(pageBuffer->lcdinput[0] == 0x3301)
	{
		getSystemTime(&pageBuffer->deviceMaintenance->dateTime);
		pageBuffer->deviceMaintenance->crc = CalModbusCRC16Fun(pageBuffer->deviceMaintenance, DeviceMaintenanceStructCrcSize, NULL);

		if(My_Pass == writeDeviceMaintenanceToFile(pageBuffer->deviceMaintenance))
		{
			deleteGB_DeviceMaintenance();
			backToActivity(SystemSetActivityName);
		}
		else
			SendKeyCode(2);
	}
	//���
	else if(pageBuffer->lcdinput[0] == 0x3302)
	{
		/*����*/
		pageBuffer->lcdinput[1] = pbuf[7];
		pageBuffer->lcdinput[1] = (pageBuffer->lcdinput[1]<<8) + pbuf[8];
			
		/*�Զ���ȡip*/
		if(pageBuffer->lcdinput[1] == 0x8000)
			pageBuffer->deviceMaintenance->isOk = true;
		/*ʹ�����õ�ip*/
		else if(pageBuffer->lcdinput[1] == 0x0000)
			pageBuffer->deviceMaintenance->isOk = false;
	}
	//ά��˵��
	else if(pageBuffer->lcdinput[0] == 0x3310)
	{
		snprintf(pageBuffer->deviceMaintenance->desc, GetBufLen(&pbuf[7], 2*pbuf[6])+1, "%s", &pbuf[7]);
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
	if(NULL == pageBuffer)
	{
		pageBuffer = MyMalloc(MaintenancePageBufferStructSize);
		if(pageBuffer)	
		{
			memset(pageBuffer, 0, MaintenancePageBufferStructSize);
	
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
	MyFree(pageBuffer);
	pageBuffer = NULL;
}

static void clearPageText(void)
{
	ClearText(0x3310);
	
	pageBuffer->tempBuf[0] = 0x00;
	pageBuffer->tempBuf[1] = 0x00;
	WriteRadioData(0x3302, pageBuffer->tempBuf, 2);
}

/****************************************end of file************************************************/
