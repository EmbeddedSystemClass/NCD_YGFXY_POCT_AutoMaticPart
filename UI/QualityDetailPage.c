/***************************************************************************************************
*FileName: MaintenancePage
*Description: �豸ά��
*Author:xsx
*Data: 2017��6��14�� 15:51:52
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"QualityDetailPage.h"

#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"MyTools.h"

#include	"Quality_Data.h"
#include	"SystemSetPage.h"
#include	"SelectUserPage.h"
#include	"CodeScan_Task.h"
#include	"Test_Task.h"
#include	"System_Data.h"
#include	"DeviceQualityDao.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/**************************************��������*************************************************/
/***************************************************************************************************/
static QualityDetailPageBuffer * pageBuffer = NULL;
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
static void showPageValue(void);
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
MyRes createQualityDetailActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, QualityDetailRecordActivityName, activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		if(pram != NULL)
			readIntent(pram, &pageBuffer->deviceQuality);
		
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
	showPageValue();
	
	SelectPage(159);
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
	if(pageBuffer->lcdinput[0] == 0x3400)
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
	SelectPage(159);
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
		pageBuffer = MyMalloc(QualityDetailPageBufferStructSize);
		if(pageBuffer)	
		{
			memset(pageBuffer, 0, QualityDetailPageBufferStructSize);
	
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

static void showPageValue(void)
{
	unsigned char i=0;
	pageBuffer->resultSum = 0.0;
	pageBuffer->tempValue3 = 0;
	
	//��ʾ��׼ֵ
	snprintf(pageBuffer->tempBuf, 15, "%.2f", pageBuffer->deviceQuality.standardValue);
	DisText(0x3410, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
	
	//��ʾÿ������ֵ�Ͷ�Ӧƫ��
	for(i=0; i<DeviceQualityMaxTestCount; i++)
	{
		//����������ʾ
		if(pageBuffer->deviceQuality.testValue[i] != 0)
		{
			snprintf(pageBuffer->tempBuf, 15, "%.2f", pageBuffer->deviceQuality.testValue[i]);
			DisText(0x3430+(i*0x05), pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
			
			pageBuffer->tempValue1 = (pageBuffer->deviceQuality.testValue[i] - pageBuffer->deviceQuality.standardValue) 
				/ pageBuffer->deviceQuality.standardValue;
				
			pageBuffer->tempValue1 *= 100;
				
			snprintf(pageBuffer->tempBuf, 15, "%.2f%%", pageBuffer->tempValue1);
			DisText(0x3465+(i*0x05), pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
			
			pageBuffer->resultSum += pageBuffer->deviceQuality.testValue[i];
			pageBuffer->tempValue3++;
		}
		//�����������
		else
		{
			ClearText(0x3430+(i*0x05));
			ClearText(0x3465+(i*0x05));
		}
	}
		
	//����ƽ��ƫ����
	if(i > 0)
	{
		pageBuffer->tempValue2 = pageBuffer->resultSum / pageBuffer->tempValue3;
		pageBuffer->tempValue2 = (pageBuffer->tempValue2 - pageBuffer->deviceQuality.standardValue)/pageBuffer->deviceQuality.standardValue;
			
		pageBuffer->tempValue1 = pageBuffer->tempValue2*100;
				
		snprintf(pageBuffer->tempBuf, 15, "%.2f%%", pageBuffer->tempValue1);
		DisText(0x349a, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
	}
	
	//��ʾ���
	pageBuffer->tempBuf[0] = 0x00;
	pageBuffer->tempBuf[1] = 0x00;
	if(pageBuffer->deviceQuality.isOk)
	{
		pageBuffer->tempBuf[1] = 0x01;
	}
	WriteRadioData(0x3402, pageBuffer->tempBuf, 2);
	
	snprintf(pageBuffer->tempBuf, 40, "%s", pageBuffer->deviceQuality.desc);
	DisText(0x3420, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
}



/****************************************end of file************************************************/
