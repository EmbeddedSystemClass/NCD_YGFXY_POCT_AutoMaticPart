/***************************************************************************************************
*FileName: MaintenancePage
*Description: �豸ά��
*Author:xsx
*Data: 2017��6��14�� 15:51:52
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"QualityPage.h"

#include	"CardLimit_Driver.h"
#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"MyTools.h"

#include	"Motor_Fun.h"
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
static QualityPageBuffer * pageBuffer = NULL;
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
static void checkQRCode(void);
static void checkCardTestResult(void);
static void showStatus(const char * str);
static void showThisTestResult(void);
static void showAllPianCha(void);
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
MyRes createQualityActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, QualityActivityName, activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	pageBuffer->deviceQuality = getGB_DeviceQuality();
	pageBuffer->testIndex = 0;
	pageBuffer->canTestNow = false;
	pageBuffer->isTestting = false;
	pageBuffer->resultSum = 0.0;
	
	clearPageText();
	
	showStatus("������Լ���\0");
	SelectPage(158);
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
		deleteGB_DeviceQuality();
		backToActivity(SystemSetActivityName);
	}
	//�ύ
	else if(pageBuffer->lcdinput[0] == 0x3401)
	{
		memcpy(&(pageBuffer->deviceQuality->dateTime), &(getSystemRunTimeData()->systemDateTime), DateTimeStructSize);
		pageBuffer->deviceQuality->crc = CalModbusCRC16Fun(pageBuffer->deviceQuality, DeviceQualityStructCrcSize, NULL);
		
		if(My_Pass == writeDeviceQualityToFile(pageBuffer->deviceQuality))
		{
			deleteGB_DeviceQuality();
			backToActivity(SystemSetActivityName);
		}
		else
			SendKeyCode(2);
	}
	//���ñ�׼ֵ
	else if(pageBuffer->lcdinput[0] == 0x3410)
	{
		pageBuffer->deviceQuality->standardValue = strtod((char *)&pbuf[7], NULL);
		showAllPianCha();
	}
	//���
	else if(pageBuffer->lcdinput[0] == 0x3302)
	{
		/*����*/
		pageBuffer->lcdinput[1] = pbuf[7];
		pageBuffer->lcdinput[1] = (pageBuffer->lcdinput[1]<<8) + pbuf[8];
			
		/*�Զ���ȡip*/
		if(pageBuffer->lcdinput[1] == 0x8000)
			pageBuffer->deviceQuality->isOk = true;
		/*ʹ�����õ�ip*/
		else if(pageBuffer->lcdinput[1] == 0x0000)
			pageBuffer->deviceQuality->isOk = false;
	}
	//ά��˵��
	else if(pageBuffer->lcdinput[0] == 0x3310)
	{
		snprintf(pageBuffer->deviceQuality->desc, GetBufLen(&pbuf[7], 2*pbuf[6])+1, "%s",&pbuf[7]);
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
	//���û�ڲ��ԣ��ҿ����޿����������������
	if((pageBuffer->isTestting == false) && (MaxLocation == getSystemRunTimeData()->motorData.location) && (CardPinIn == NoCard))
		pageBuffer->canTestNow = true;
	
	//�������������ԣ��Ҳ��Դ���δ�������������п����룬��ʼ����
	if((pageBuffer->canTestNow) && (pageBuffer->testIndex < DeviceQualityMaxTestCount) && (CardPinIn == CardIN))
	{
		pageBuffer->canTestNow = false;
		pageBuffer->isTestting = true;
		
		memset(&(pageBuffer->testData), 0, TestDataStructSize);
		
		//��һ��ɨ���ά��
		StartScanQRCode(&(pageBuffer->testData.qrCode));
		showStatus("��ʼɨ���ά��\0");
	}
	
	if(pageBuffer->isTestting)
	{
		checkQRCode();
		
		checkCardTestResult();
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
	SelectPage(158);
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
		pageBuffer = MyMalloc(QualityPageBufferStructSize);
		if(pageBuffer)	
		{
			memset(pageBuffer, 0, QualityPageBufferStructSize);
	
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


static void checkQRCode(void)
{
	if(My_Pass == TakeScanQRCodeResult(&(pageBuffer->cardScanResult)))
	{
		//��֧�ֵ�Ʒ��
		if(pageBuffer->cardScanResult == CardUnsupported)
		{
			showStatus("��֧�ִ���Ŀ\0");
			MotorMoveTo(MaxLocation, 1);
			SendKeyCode(6);
			pageBuffer->isTestting = false;
		}
		//����
		else if(pageBuffer->cardScanResult == CardCodeTimeOut)
		{
			showStatus("����\0");
			MotorMoveTo(MaxLocation, 1);
			SendKeyCode(4);
			pageBuffer->isTestting = false;
		}
		//��ȡ�ɹ�
		else if(pageBuffer->cardScanResult == CardCodeScanOK)
		{
			//��ȡУ׼����
			memcpy(pageBuffer->testData.adjustData.ItemName, pageBuffer->testData.qrCode.ItemName, ItemNameLen);
			getAdjPram(getGBSystemSetData(), &(pageBuffer->testData.adjustData));
			
			//�����ʿ���Ŀ����
			if(strlen(pageBuffer->deviceQuality->itemName) == 0)
			{
				showStatus("��ʼɨ���Լ���\0");
				memcpy(pageBuffer->deviceQuality->itemName, pageBuffer->testData.qrCode.ItemName, ItemNameLen);
				StartTest(&(pageBuffer->testData));
			}
			else if(CheckStrIsSame(pageBuffer->deviceQuality->itemName, pageBuffer->testData.qrCode.ItemName, ItemNameLen))
			{
				showStatus("��ʼɨ���Լ���\0");
				StartTest(&(pageBuffer->testData));
			}
			else
			{
				showStatus("��Ŀ���������\0");
				MotorMoveTo(MaxLocation, 1);
				pageBuffer->isTestting = false;
			}
		}
		/*��������CardCodeScanFail, CardCodeCardOut, CardCodeScanTimeOut, CardCodeCRCError*/
		else
		{
			showStatus("��ά�����\0");
			MotorMoveTo(MaxLocation, 1);
			SendKeyCode(1);
			pageBuffer->isTestting = false;
		}
	}
}

static void checkCardTestResult(void)
{
	if(My_Pass == TakeTestResult(&(pageBuffer->testData.testResultDesc)))
	{
		if(pageBuffer->testData.testResultDesc == NoSample)
		{
			showStatus("δ����\0");
		}
		else if(pageBuffer->testData.testResultDesc == PeakError)
		{
			showStatus("����\0");
		}
		else if(pageBuffer->testData.testResultDesc == ResultIsOK)
		{
			//������Խ��
			pageBuffer->deviceQuality->testValue[pageBuffer->testIndex] = pageBuffer->testData.testSeries.AdjustResult;
			showThisTestResult();
			
			pageBuffer->testIndex++;
			
			showStatus("��ǰ�������\0");
		}
		
		MotorMoveTo(MaxLocation, 1);
		pageBuffer->isTestting = false;
	}
}

static void showStatus(const char * str)
{
	snprintf(pageBuffer->tempBuf, 50, "%s", str);
	DisText(0x34a0, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
}

static void showThisTestResult(void)
{
	//��ʾ��ǰ���Խ��
	snprintf(pageBuffer->tempBuf, 15, "%.*f", pageBuffer->testData.qrCode.itemConstData.pointNum, pageBuffer->testData.testSeries.AdjustResult);
	DisText(0x3430+(pageBuffer->testIndex*0x05), pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
	
	pageBuffer->resultSum += pageBuffer->testData.testSeries.AdjustResult;
	
	if(pageBuffer->deviceQuality->standardValue != 0)
	{
		//��ʾ��ǰ����ƫ����	
		pageBuffer->tempValue1 = (pageBuffer->testData.testSeries.AdjustResult - pageBuffer->deviceQuality->standardValue) 
					/ pageBuffer->deviceQuality->standardValue;
		pageBuffer->tempValue1 *= 100;
			
		snprintf(pageBuffer->tempBuf, 15, "%.2f%%", pageBuffer->tempValue1);
		DisText(0x3465+(pageBuffer->testIndex*0x05), pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
		
		//����ƽ��ƫ��
		pageBuffer->tempValue2 = pageBuffer->resultSum / (pageBuffer->testIndex+1);
		pageBuffer->tempValue2 = (pageBuffer->tempValue2 - pageBuffer->deviceQuality->standardValue)/pageBuffer->deviceQuality->standardValue;
			
		pageBuffer->tempValue1 = pageBuffer->tempValue2*100;
				
		snprintf(pageBuffer->tempBuf, 15, "%.2f%%", pageBuffer->tempValue1);
		DisText(0x349a, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
	}
}

static void showAllPianCha(void)
{
	unsigned char i=0;
	if(pageBuffer->deviceQuality->standardValue != 0)
	{
		for(i=0; i<pageBuffer->testIndex; i++)
		{
			pageBuffer->tempValue1 = 
				(pageBuffer->deviceQuality->testValue[i] - pageBuffer->deviceQuality->standardValue) 
				/ pageBuffer->deviceQuality->standardValue;
			
			pageBuffer->tempValue1 *= 100;
			
			snprintf(pageBuffer->tempBuf, 15, "%.2f%%", pageBuffer->tempValue1);
			DisText(0x3465+(i*0x05), pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
		}
		
		//����ƽ��ƫ����
		if(pageBuffer->testIndex > 0)
		{
			pageBuffer->tempValue2 = pageBuffer->resultSum / pageBuffer->testIndex;
			pageBuffer->tempValue2 = (pageBuffer->tempValue2 - pageBuffer->deviceQuality->standardValue)/pageBuffer->deviceQuality->standardValue;
			
			pageBuffer->tempValue1 = pageBuffer->tempValue2*100;
				
			snprintf(pageBuffer->tempBuf, 15, "%.2f%%", pageBuffer->tempValue1);
			DisText(0x349a, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
		}
	}
}

static void clearPageText(void)
{
	unsigned char i=0;
	
	for(i=0; i<DeviceQualityMaxTestCount; i++)
	{
		ClearText(0x3430+(i*0x05));
		ClearText(0x3465+(i*0x05));
	}
	
	ClearText(0x349a);
	ClearText(0x3410);
	
	ClearText(0x3420);
	
	pageBuffer->tempBuf[0] = 0x00;
	pageBuffer->tempBuf[1] = 0x00;
	WriteRadioData(0x3402, pageBuffer->tempBuf, 2);
}


/****************************************end of file************************************************/
