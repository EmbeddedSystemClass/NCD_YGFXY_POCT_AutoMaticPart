/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"AboutUsPage.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"RemoteSoft_Data.h"
#include	"System_Data.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static AboutUsPageBuffer * page = NULL;
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

static void dspPageText(void);
static void freshRemoteFirmwareVersion(void);
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
MyRes createAboutUsActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "AboutUsActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	timer_SetAndStart(&(page->timer), 5);
	
	dspPageText();
		
	freshRemoteFirmwareVersion();
		
	SelectPage(116);
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
		
	//����
	if(page->lcdinput[0] == 0x2900)
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
	if(TimerOut == timer_expired(&(page->timer)))
	{
		freshRemoteFirmwareVersion();
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
	dspPageText();
	
	freshRemoteFirmwareVersion();
	
	timer_restart(&(page->timer));
	
	SelectPage(116);
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
		page = MyMalloc(sizeof(AboutUsPageBuffer));
		
		if(page)
		{
			memset(page, 0, sizeof(AboutUsPageBuffer));
	
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

/***************************************************************************************************
*FunctionName:  dspPageText
*Description:  ��ʾ��������
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��6�� 13:56:23
***************************************************************************************************/
static void dspPageText(void)
{
	snprintf(page->buf, 20, "%s", GB_SoftVersion_Build);
	DisText(0x2920, page->buf, strlen(page->buf)+1);
	
	//���ö�ά��x,y,���ص��С
	page->buf[0] = 0;
	page->buf[1] = 180;
	page->buf[2] = 0x01;
	page->buf[3] = 0x42;
	page->buf[4] = 0;
	page->buf[5] = 0x02;
	writeDataToLcd(0x2941, page->buf, 6);

	//��ʾ��ά��
	sprintf(page->buf, "http://www.whnewcando.com/?");
	getSystemDeviceId(page->buf + strlen(page->buf));
	writeDataToLcd(0x2950, page->buf, strlen(page->buf)+1);
}

/***************************************************************************************************
*FunctionName:  freshRemoteFirmwareVersion
*Description:  ˢ��Զ������汾
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��6�� 14:01:36
***************************************************************************************************/
static void freshRemoteFirmwareVersion(void)
{
	if((getIsSuccessDownloadFirmware() == true) && (getGbRemoteFirmwareVersion() > GB_SoftVersion))
	{
		page->tempV = getGbRemoteFirmwareVersion();
		snprintf(page->buf, 50, "V%d.%d.%02d (�°汾V%d.%d.%02d)", GB_SoftVersion/1000, GB_SoftVersion%1000/100, GB_SoftVersion%100,
			page->tempV/1000, page->tempV%1000/100, page->tempV%100);
	}
	else
		snprintf(page->buf, 50, "V%d.%d.%02d", GB_SoftVersion/1000, GB_SoftVersion%1000/100, GB_SoftVersion%100);
	DisText(0x2910, page->buf, strlen(page->buf)+1);
}
