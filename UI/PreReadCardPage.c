/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"PreReadCardPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"MyTest_Data.h"
#include	"CodeScan_Task.h"
#include	"WaittingCardPage.h"
#include	"TimeDownNorPage.h"
#include	"PaiDuiPage.h"
#include	"MyTools.h"
#include	"CRC16.h"
#include	"System_Data.h"
#include	"StringDefine.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static PreReadPageBuffer * S_PreReadPageBuffer = NULL;
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

static void clearPageText(void);
static void CheckQRCode(void);
static void ShowCardInfo(void);
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
MyRes createPreReadCardActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "PreReadCardActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	S_PreReadPageBuffer->currenttestdata = GetCurrentTestItem();
	StartScanQRCode(&(S_PreReadPageBuffer->temperweima));
	
	clearPageText();

	SelectPage(92);
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
	if(S_PreReadPageBuffer)
	{
		/*����*/
		S_PreReadPageBuffer->lcdinput[0] = pbuf[4];
		S_PreReadPageBuffer->lcdinput[0] = (S_PreReadPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*��ά���ȡʧ�ܣ����ڣ���ʹ��*/
		if((S_PreReadPageBuffer->lcdinput[0] >= 0x1400) && (S_PreReadPageBuffer->lcdinput[0] <= 0x1405))
		{
			/*����*/
			S_PreReadPageBuffer->lcdinput[1] = pbuf[7];
			S_PreReadPageBuffer->lcdinput[1] = (S_PreReadPageBuffer->lcdinput[1]<<8) + pbuf[8];
			
			/*������⿨*/
			if(S_PreReadPageBuffer->lcdinput[1] == 0x0001)
			{
				backToFatherActivity();
			}
			//ȡ������
			else if(S_PreReadPageBuffer->lcdinput[1] == 0x0000)
			{
				DeleteCurrentTest();

				backToActivity(lunchActivityName);
				
				if(IsPaiDuiTestting())
					startActivity(createPaiDuiActivity, NULL, NULL);
			}
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
	CheckQRCode();
	
	if(S_PreReadPageBuffer->scancode != CardScanNone)
	{
		if(isMotorMoveEnd(0 / portTICK_RATE_MS))
		{
			if(S_PreReadPageBuffer->scancode == CardCodeScanOK)
			{
				S_PreReadPageBuffer->currenttestdata->statues = status_start;
				//�������µĲ�����
				upDateLastOperator(&S_PreReadPageBuffer->currenttestdata->testData.operator);
				startActivity(createPaiDuiActivity, NULL, NULL);
			}
			else
			{
				if(S_PreReadPageBuffer->scancode == CardUnsupported)										//��֧�ֵ�Ʒ��
					SendKeyCode(6);
				else if(S_PreReadPageBuffer->scancode == CardCodeTimeOut)									//����
					SendKeyCode(4);
				else
					SendKeyCode(1);
			}
		}
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
	if(NULL == S_PreReadPageBuffer)
	{
		S_PreReadPageBuffer = MyMalloc(sizeof(PreReadPageBuffer));
		
		if(S_PreReadPageBuffer)
		{
			memset(S_PreReadPageBuffer, 0, sizeof(PreReadPageBuffer));
	
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
	MyFree(S_PreReadPageBuffer);
	S_PreReadPageBuffer = NULL;
}



static void CheckQRCode(void)
{
	if(My_Pass == TakeScanQRCodeResult(&S_PreReadPageBuffer->scancode))
	{
		if(S_PreReadPageBuffer->scancode == CardCodeScanOK)
		{
			ShowCardInfo();
				
			//����ȡ�Ķ�ά�����ݿ������������ݰ���
			memcpy(&(S_PreReadPageBuffer->currenttestdata->testData.qrCode), &(S_PreReadPageBuffer->temperweima), sizeof(QRCode));
					
			//���õ���ʱʱ��
			timer_SetAndStart(&(S_PreReadPageBuffer->currenttestdata->timeDown_timer), S_PreReadPageBuffer->currenttestdata->testData.qrCode.CardWaitTime);
			
			S_PreReadPageBuffer->motorAction.motorActionEnum = OriginLocationDef;
			S_PreReadPageBuffer->motorAction.motorParm = S_PreReadPageBuffer->currenttestdata->cardLocation+1;
		}
		else
		{
			S_PreReadPageBuffer->motorAction.motorActionEnum = WaitCardPutInDef;
			S_PreReadPageBuffer->motorAction.motorParm = S_PreReadPageBuffer->currenttestdata->cardLocation;
		}
		
		StartMotorAction(&S_PreReadPageBuffer->motorAction, false);
	}
}

static void clearPageText(void)
{
	ClearText(0x1420);
	ClearText(0x1430);
	ClearText(0x1440);
	ClearText(0x1450);
	ClearText(0x1460);
	ClearText(0x1470);
}

static void ShowCardInfo(void)
{
	sprintf(S_PreReadPageBuffer->buf, "%s", S_PreReadPageBuffer->temperweima.itemConstData.itemName);
	DisText(0x1420, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
		
	sprintf(S_PreReadPageBuffer->buf, "%s-%s", S_PreReadPageBuffer->temperweima.PiHao, S_PreReadPageBuffer->temperweima.piNum);
	DisText(0x1430, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
	
	sprintf(S_PreReadPageBuffer->buf, "%d S", S_PreReadPageBuffer->temperweima.CardWaitTime);
	DisText(0x1440, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
		
	sprintf(S_PreReadPageBuffer->buf, "20%02d��%02d��%02d��", S_PreReadPageBuffer->temperweima.CardBaoZhiQi.year, 
		S_PreReadPageBuffer->temperweima.CardBaoZhiQi.month, S_PreReadPageBuffer->temperweima.CardBaoZhiQi.day);
	DisText(0x1450, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
}


