/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"PreReadCardPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"MyTest_Data.h"
#include	"LunchPage.h"
#include	"CodeScan_Task.h"
#include	"WaittingCardPage.h"
#include	"TimeDownNorPage.h"
#include	"PaiDuiPage.h"
#include	"MyTools.h"
#include	"CRC16.h"
#include	"System_Data.h"
#include	"Motor2_Fun.h"
#include	"Motor4_Fun.h"

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
		
	clearPageText();

	StartScanQRCode(&(S_PreReadPageBuffer->temperweima));
	S_PreReadPageBuffer->qrIsGet = false;
	
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
				/*//������Ŷ��е��ٴ�Ԥ�����򷵻��Ŷӽ��棬״̬�л���֮ǰ��״̬
				if(S_PreReadPageBuffer->currenttestdata->statues == status_prereadagain_n)
					S_PreReadPageBuffer->currenttestdata->statues = status_incard_n;
				else if(S_PreReadPageBuffer->currenttestdata->statues == status_prereadagain_o)
					S_PreReadPageBuffer->currenttestdata->statues = status_incard_o;
				//����ǵ�һ��Ԥ��
				else if(S_PreReadPageBuffer->currenttestdata->statues == status_wait1)
					S_PreReadPageBuffer->currenttestdata->statues = status_wait1;
				*/
				backToFatherActivity();
			}
			//ȡ������
			else if(S_PreReadPageBuffer->lcdinput[1] == 0x0000)
			{
				DeleteCurrentTest();
				
				backToActivity(lunchActivityName);
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
	if(S_PreReadPageBuffer->qrIsGet == false)
	{
		S_PreReadPageBuffer->scancode = getScanResult();
		
		if(CardCodeScanning != S_PreReadPageBuffer->scancode)
		{
			//��֧�ֵ�Ʒ��
			if(S_PreReadPageBuffer->scancode == CardUnsupported)
			{
				motor4MoveTo(Motor4_OpenLocation, 1);
				motor2MoveTo(Motor2_WaitCardLocation, 1);
				SendKeyCode(6);
			}
			//����
			else if(S_PreReadPageBuffer->scancode == CardCodeTimeOut)
			{
				motor4MoveTo(Motor4_OpenLocation, 1);
				motor2MoveTo(Motor2_WaitCardLocation, 1);
				SendKeyCode(4);
			}
			//��ȡ�ɹ�
			else if(S_PreReadPageBuffer->scancode == CardCodeScanOK)
			{
				ShowCardInfo();
				
				//����ȡ�Ķ�ά�����ݿ������������ݰ���
				memcpy(&(S_PreReadPageBuffer->currenttestdata->testData.qrCode), &(S_PreReadPageBuffer->temperweima), sizeof(QRCode));
					
				//���õ���ʱʱ��
				timer_SetAndStart(&(S_PreReadPageBuffer->currenttestdata->timeDown_timer), S_PreReadPageBuffer->currenttestdata->testData.qrCode.CardWaitTime*60);
				
				//��ȡУ׼����
				memcpy(S_PreReadPageBuffer->currenttestdata->testData.adjustData.ItemName, S_PreReadPageBuffer->currenttestdata->testData.qrCode.ItemName, ItemNameLen);
				getAdjPram(getGBSystemSetData(), &(S_PreReadPageBuffer->currenttestdata->testData.adjustData));

				motor2MoveTo(Motor2_PutDownCardLocation, 10000);
				motor4MoveTo(Motor4_OpenLocation, 5000);
				motor2MoveTo(Motor2_MidLocation, 1);
				
				S_PreReadPageBuffer->currenttestdata->statues = status_start;
				startActivity(createPaiDuiActivity, NULL, NULL);
			}
			//��������CardCodeScanFail, CardCodeCardOut, CardCodeScanTimeOut, CardCodeCRCError
			else
			{
				motor4MoveTo(Motor4_OpenLocation, 1);
				motor2MoveTo(Motor2_WaitCardLocation, 1);
				SendKeyCode(1);
			}
			
			S_PreReadPageBuffer->qrIsGet = true;
		}
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
	sprintf(S_PreReadPageBuffer->buf, "%s\0", S_PreReadPageBuffer->temperweima.ItemName);
	DisText(0x1420, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
		
	sprintf(S_PreReadPageBuffer->buf, "%s-%s\0", S_PreReadPageBuffer->temperweima.PiHao, S_PreReadPageBuffer->temperweima.piNum);
	DisText(0x1430, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
	
	sprintf(S_PreReadPageBuffer->buf, "%d S\0", S_PreReadPageBuffer->temperweima.CardWaitTime*60);
	DisText(0x1440, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
		
	sprintf(S_PreReadPageBuffer->buf, "20%02d��%02d��%02d��\0", S_PreReadPageBuffer->temperweima.CardBaoZhiQi.year, 
		S_PreReadPageBuffer->temperweima.CardBaoZhiQi.month, S_PreReadPageBuffer->temperweima.CardBaoZhiQi.day);
	DisText(0x1450, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
}


