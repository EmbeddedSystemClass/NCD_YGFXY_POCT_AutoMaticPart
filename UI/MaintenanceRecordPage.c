/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"MaintenanceRecordPage.h"

#include	"LCD_Driver.h"
#include	"DeviceMaintenanceDao.h"

#include	"Define.h"
#include	"MyMem.h"
#include	"CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static MaintenanceRecordPageBuffer * pageBuffer = NULL;
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

static void ShowRecord(void);
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
MyRes createMaintenanceRecordActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, MaintenanceRecordActivityName, activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	pageBuffer->pageRequest.pageIndex = 0;
	pageBuffer->pageRequest.pageSize = DeviceMaintenanceRecordPageShowNum;
	pageBuffer->pageRequest.orderType = ASC;
	
	for(pageBuffer->i=0; pageBuffer->i<DeviceMaintenanceRecordPageShowNum ; pageBuffer->i++)
		pageBuffer->page.content[pageBuffer->i] = &pageBuffer->deviceMaintenance[pageBuffer->i];
	
	ShowRecord();
	
	SelectPage(155);
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
		
	/*����*/
	if(pageBuffer->lcdinput[0] == 0x3100)
	{
		backToFatherActivity();
	}
	/*��һҳ*/
	else if(pageBuffer->lcdinput[0] == 0x3102)
	{
		if(pageBuffer->page.totalPageSize > 0)
		{
			if(pageBuffer->pageRequest.pageIndex >= 1)
				pageBuffer->pageRequest.pageIndex -= 1;
			else
				pageBuffer->pageRequest.pageIndex = pageBuffer->page.totalPageSize - 1;
				
			ShowRecord();
		}
	}
	/*��һҳ*/
	else if(pageBuffer->lcdinput[0] == 0x3103)
	{
		if(pageBuffer->page.totalPageSize > 0)
		{
			if((pageBuffer->pageRequest.pageIndex + 1) >= pageBuffer->page.totalPageSize)
				pageBuffer->pageRequest.pageIndex = 0;
			else
				pageBuffer->pageRequest.pageIndex += 1;
				
			ShowRecord();
		}
	}
	//ѡ������
	else if((pageBuffer->lcdinput[0] >= 0x3104)&&(pageBuffer->lcdinput[0] <= 0x310b))
	{
		pageBuffer->tempvalue1 = pageBuffer->lcdinput[0] - 0x3104;
			
		if(pageBuffer->tempvalue1 < pageBuffer->page.readItemSize)
			BasicPic(0x3120, 1, 137, 11, 355, 940, 392, 38, 149+(pageBuffer->tempvalue1)*40);
	}
	//��ҳ
	else if(pageBuffer->lcdinput[0] == 0x3110)
	{
		pageBuffer->tempvalue1 = strtol((char *)(&pbuf[7]), NULL, 10);
		if( (pageBuffer->tempvalue1 > 0) && (pageBuffer->tempvalue1 <= pageBuffer->page.totalPageSize))
		{
			pageBuffer->pageRequest.pageIndex = pageBuffer->tempvalue1;

			ShowRecord();
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
	SelectPage(155);
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
		pageBuffer = MyMalloc(sizeof(MaintenanceRecordPageBuffer));
		
		if(pageBuffer)
		{
			memset(pageBuffer, 0, sizeof(MaintenanceRecordPageBuffer));
	
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

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

static void ShowRecord(void)
{
	//��ȡ����
	memset(pageBuffer->deviceMaintenance, 0, DeviceMaintenanceRecordPageShowNum * DeviceMaintenanceStructSize);
	readDeviceMaintenanceFromFileByPageRequest(&(pageBuffer->pageRequest), &(pageBuffer->deviceRecordHeader), &pageBuffer->page);
		
	BasicPic(0x3120, 0, 100, 11, 142, 940, 179, 39, 140);
	
	vTaskDelay(10 / portTICK_RATE_MS);

	for(pageBuffer->i=0; pageBuffer->i<DeviceMaintenanceRecordPageShowNum; pageBuffer->i++)
	{
		pageBuffer->tempDeviceMaintenance = &pageBuffer->deviceMaintenance[pageBuffer->i];
		pageBuffer->tempvalue1 = pageBuffer->i*0x30;
		
		if(pageBuffer->tempDeviceMaintenance->crc == CalModbusCRC16Fun(pageBuffer->tempDeviceMaintenance, DeviceMaintenanceStructCrcSize, NULL))
		{
			//��ʾ����
			if(pageBuffer->pageRequest.orderType == ASC)
				snprintf(pageBuffer->buf, 10, "%d", pageBuffer->page.totalItemSize - pageBuffer->pageRequest.pageIndex*DeviceMaintenanceRecordPageShowNum-pageBuffer->i);
			else
				snprintf(pageBuffer->buf, 10, "%d", pageBuffer->pageRequest.pageIndex*DeviceMaintenanceRecordPageShowNum+pageBuffer->i+1);
			DisText(0x3130+pageBuffer->tempvalue1, pageBuffer->buf, strlen(pageBuffer->buf)+1);
				
			//��ʾ���
			if(pageBuffer->tempDeviceMaintenance->isOk)
				snprintf(pageBuffer->buf, 10, "OK");
			else
				snprintf(pageBuffer->buf, 10, "ERROR");
			DisText(0x3135+pageBuffer->tempvalue1, pageBuffer->buf, strlen(pageBuffer->buf)+1);
				
			//��ʾ��ע
			if(strlen(pageBuffer->tempDeviceMaintenance->desc) <= 22)
				snprintf(pageBuffer->buf, 50, "%s", pageBuffer->tempDeviceMaintenance->desc);
			else
				snprintf(pageBuffer->buf, 50, "%.22s ...", pageBuffer->tempDeviceMaintenance->desc);
			DisText(0x313a+pageBuffer->tempvalue1, pageBuffer->buf, strlen(pageBuffer->buf)+1);
				
			//��ʾʱ��
			snprintf(pageBuffer->buf, 30, "%02d-%02d-%02d %02d:%02d", pageBuffer->tempDeviceMaintenance->dateTime.year, 
				pageBuffer->tempDeviceMaintenance->dateTime.month, pageBuffer->tempDeviceMaintenance->dateTime.day,
				pageBuffer->tempDeviceMaintenance->dateTime.hour, pageBuffer->tempDeviceMaintenance->dateTime.min);
			DisText(0x314a+pageBuffer->tempvalue1, pageBuffer->buf, strlen(pageBuffer->buf)+1);

			//��ʾ������
			snprintf(pageBuffer->buf, 30, "%s", pageBuffer->tempDeviceMaintenance->operator.name);
			DisText(0x3155+pageBuffer->tempvalue1, pageBuffer->buf, strlen(pageBuffer->buf)+1);

			pageBuffer->tempDeviceMaintenance--;
		}
		else
		{
			ClearText(0x3130+pageBuffer->tempvalue1);
			ClearText(0x3135+pageBuffer->tempvalue1);
			ClearText(0x313a+pageBuffer->tempvalue1);
			ClearText(0x314a+pageBuffer->tempvalue1);
			ClearText(0x3155+pageBuffer->tempvalue1);
		}
		
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}

