
/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"CheckQRPage.h"

#include	"CardCheck_Driver.h"
#include	"LCD_Driver.h"
#include	"Motor_Fun.h"
#include	"UI_Data.h"
#include	"Timer.h"
#include	"CodeScan_Task.h"
#include	"ItemConst_Data.h"
#include	"MyMem.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static CheckQRPageBuffer *S_CheckQRPageBuffer = NULL;
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
static void dspScanStatus(char * str);
static void DspPageText(void);
static void calculateResult(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName: createWelcomeActivity
*Description: ������ӭ����
*Input: thizActivity -- ��ǰ����
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:21:51
***************************************************************************************************/
MyRes createCheckQRActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "CheckQRActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

static void activityStart(void)
{
	if(S_CheckQRPageBuffer)
	{
		clearPageText();
		dspScanStatus("Waitting\0");
	}
	
	SelectPage(139);
}
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	if(S_CheckQRPageBuffer)
	{
		/*����*/
		S_CheckQRPageBuffer->lcdinput[0] = pbuf[4];
		S_CheckQRPageBuffer->lcdinput[0] = (S_CheckQRPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		//����
		if(S_CheckQRPageBuffer->lcdinput[0] == 0x25f0)
		{
			backToFatherActivity();
		}
		/*��ȡ�����id*/
		else if(S_CheckQRPageBuffer->lcdinput[0] == 0x25b8)
		{
			memset(S_CheckQRPageBuffer->buf, 0, 20);
			memcpy(S_CheckQRPageBuffer->buf, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			
			calculateResult();
		}
	}
}
static void activityFresh(void)
{
	if(S_CheckQRPageBuffer)
	{
		/*�Ƿ�忨*/
		if(CardPinIn)
		{
			if(S_CheckQRPageBuffer->isScanning == false)
			{
				clearPageText();
				S_CheckQRPageBuffer->isScanning = true;
				S_CheckQRPageBuffer->scancode = CardCodeScanning;
				StartScanQRCode(&(S_CheckQRPageBuffer->qrCode));
				
				dspScanStatus("Scanning\0");
			}
		}
		else
		{
			if(S_CheckQRPageBuffer->isScanning == true)
			{
				S_CheckQRPageBuffer->isScanning = false;
				dspScanStatus("Waitting\0");
			}
		}

		if(My_Pass == TakeScanQRCodeResult(&(S_CheckQRPageBuffer->scancode)))
		{	
			MotorMoveTo(MaxLocation, 1);
			
			//��ά���ȡʧ��
			if((S_CheckQRPageBuffer->scancode == CardCodeScanFail) || (S_CheckQRPageBuffer->scancode == CardCodeCardOut) ||
				(S_CheckQRPageBuffer->scancode == CardCodeScanTimeOut) || (S_CheckQRPageBuffer->scancode == CardCodeCRCError))
			{
				dspScanStatus("Scan Fail\0");
			}
			//��ȡ��ǰƷ�̶ֹ�����
			else if(S_CheckQRPageBuffer->scancode == CardUnsupported)
				dspScanStatus("Unsupported Item\0");
			//����
			else if(S_CheckQRPageBuffer->scancode == CardCodeTimeOut)
			{
				dspScanStatus("Out Of Date\0");
			}
			//��ȡ�ɹ�
			else if(S_CheckQRPageBuffer->scancode == CardCodeScanOK)
			{
				dspScanStatus("Success\0");
				DspPageText();
			}
		}
	}
}
static void activityHide(void)
{

}
static void activityResume(void)
{

}
static void activityDestroy(void)
{
	activityBufferFree();
}

static MyRes activityBufferMalloc(void)
{
	if(NULL == S_CheckQRPageBuffer)
	{
		S_CheckQRPageBuffer = MyMalloc(sizeof(CheckQRPageBuffer));
		
		if(S_CheckQRPageBuffer)
		{
			memset(S_CheckQRPageBuffer, 0, sizeof(CheckQRPageBuffer));
			
			return My_Pass;
		}
	}

	return My_Fail;
}

static void activityBufferFree(void)
{
	MyFree(S_CheckQRPageBuffer);
	S_CheckQRPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

static void clearPageText(void)
{
	ClearText(0x2500);
	ClearText(0x2508);
	ClearText(0x2510);
	ClearText(0x2518);
	ClearText(0x2520);
	ClearText(0x2528);
	ClearText(0x2530);
	ClearText(0x2538);
	ClearText(0x2540);
	ClearText(0x2548);
	ClearText(0x2550);
	ClearText(0x2558);
	ClearText(0x2568);
	ClearText(0x2578);
	ClearText(0x2580);
	ClearText(0x2590);
	ClearText(0x2598);
	ClearText(0x25b8);
	ClearText(0x25c0);
}

static void dspScanStatus(char * str)
{
	memset(S_CheckQRPageBuffer->buf, 0, 20);
	sprintf(S_CheckQRPageBuffer->buf, "%-15s", str);
	DisText(0x25b0, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
}

static void DspPageText(void)
{
	sprintf(S_CheckQRPageBuffer->buf, "%-20s", S_CheckQRPageBuffer->qrCode.PiHao);
	DisText(0x2500, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20s", S_CheckQRPageBuffer->qrCode.piNum);
	DisText(0x2508, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20s", S_CheckQRPageBuffer->qrCode.ItemName);
	DisText(0x2510, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20s", S_CheckQRPageBuffer->qrCode.itemConstData.normalResult);
	DisText(0x2518, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20f", S_CheckQRPageBuffer->qrCode.itemConstData.lowstResult);
	DisText(0x2520, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20f", S_CheckQRPageBuffer->qrCode.itemConstData.highestResult);
	DisText(0x2528, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20s", S_CheckQRPageBuffer->qrCode.itemConstData.itemMeasure);
	DisText(0x2530, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20d", S_CheckQRPageBuffer->qrCode.itemConstData.pointNum);
	DisText(0x2538, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20d", S_CheckQRPageBuffer->qrCode.ItemLocation);
	DisText(0x2540, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20d", S_CheckQRPageBuffer->qrCode.CLineLocation);
	DisText(0x2548, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20d", S_CheckQRPageBuffer->qrCode.CardWaitTime*60);
	DisText(0x2550, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "20%02d-%02d-%02d", S_CheckQRPageBuffer->qrCode.CardBaoZhiQi.year, S_CheckQRPageBuffer->qrCode.CardBaoZhiQi.month, 
		S_CheckQRPageBuffer->qrCode.CardBaoZhiQi.day);
	DisText(0x2558, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%d", S_CheckQRPageBuffer->qrCode.ChannelNum);
	DisText(0x2560, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	memset(S_CheckQRPageBuffer->buf, 0, 50);
	sprintf(S_CheckQRPageBuffer->buf, "%.4f*x^2+%.4f*x+%.4f", S_CheckQRPageBuffer->qrCode.ItemBiaoQu[0][0], S_CheckQRPageBuffer->qrCode.ItemBiaoQu[0][1], 
		S_CheckQRPageBuffer->qrCode.ItemBiaoQu[0][2]);
	DisText(0x2568, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20f", S_CheckQRPageBuffer->qrCode.ItemFenDuan[0]);
	DisText(0x2578, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	memset(S_CheckQRPageBuffer->buf, 0, 50);
	sprintf(S_CheckQRPageBuffer->buf, "%.4f*x^2+%.4f*x+%.4f", S_CheckQRPageBuffer->qrCode.ItemBiaoQu[1][0], S_CheckQRPageBuffer->qrCode.ItemBiaoQu[1][1], 
		S_CheckQRPageBuffer->qrCode.ItemBiaoQu[1][2]);
	DisText(0x2580, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	sprintf(S_CheckQRPageBuffer->buf, "%-20f", S_CheckQRPageBuffer->qrCode.ItemFenDuan[1]);
	DisText(0x2590, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
	
	memset(S_CheckQRPageBuffer->buf, 0, 50);
	sprintf(S_CheckQRPageBuffer->buf, "%.4f*x^2+%.4f*x+%.4f", S_CheckQRPageBuffer->qrCode.ItemBiaoQu[2][0], S_CheckQRPageBuffer->qrCode.ItemBiaoQu[2][1], 
		S_CheckQRPageBuffer->qrCode.ItemBiaoQu[2][2]);
	DisText(0x2598, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf));
}

static void calculateResult(void)
{
	S_CheckQRPageBuffer->inputTC = strtod(S_CheckQRPageBuffer->buf , NULL);
	
	/*���ݷֶΣ�����ԭʼ���*/
	if((S_CheckQRPageBuffer->inputTC < S_CheckQRPageBuffer->qrCode.ItemFenDuan[0]) || (S_CheckQRPageBuffer->qrCode.ItemFenDuan[0] == 0))
	{
		S_CheckQRPageBuffer->basicResult = S_CheckQRPageBuffer->inputTC * S_CheckQRPageBuffer->inputTC;
		S_CheckQRPageBuffer->basicResult *= S_CheckQRPageBuffer->qrCode.ItemBiaoQu[0][0];
					
		S_CheckQRPageBuffer->basicResult += (S_CheckQRPageBuffer->inputTC * S_CheckQRPageBuffer->qrCode.ItemBiaoQu[0][1]);
					
		S_CheckQRPageBuffer->basicResult += S_CheckQRPageBuffer->qrCode.ItemBiaoQu[0][2];
	}
	else if((S_CheckQRPageBuffer->inputTC < S_CheckQRPageBuffer->qrCode.ItemFenDuan[1]) || (S_CheckQRPageBuffer->qrCode.ItemFenDuan[1] == 0))
	{
		S_CheckQRPageBuffer->basicResult = S_CheckQRPageBuffer->inputTC * S_CheckQRPageBuffer->inputTC;
		S_CheckQRPageBuffer->basicResult *= S_CheckQRPageBuffer->qrCode.ItemBiaoQu[1][0];
					
		S_CheckQRPageBuffer->basicResult += (S_CheckQRPageBuffer->inputTC * S_CheckQRPageBuffer->qrCode.ItemBiaoQu[1][1]);
					
		S_CheckQRPageBuffer->basicResult += S_CheckQRPageBuffer->qrCode.ItemBiaoQu[1][2];
	}
	else
	{
		S_CheckQRPageBuffer->basicResult = S_CheckQRPageBuffer->inputTC * S_CheckQRPageBuffer->inputTC;
		S_CheckQRPageBuffer->basicResult *= S_CheckQRPageBuffer->qrCode.ItemBiaoQu[2][0];
					
		S_CheckQRPageBuffer->basicResult += (S_CheckQRPageBuffer->inputTC * S_CheckQRPageBuffer->qrCode.ItemBiaoQu[2][1]);
					
		S_CheckQRPageBuffer->basicResult += S_CheckQRPageBuffer->qrCode.ItemBiaoQu[2][2];
	}
			
	sprintf(S_CheckQRPageBuffer->buf, "%.*f\0", S_CheckQRPageBuffer->qrCode.itemConstData.pointNum, S_CheckQRPageBuffer->basicResult);
	DisText(0x25c0, S_CheckQRPageBuffer->buf, strlen(S_CheckQRPageBuffer->buf)+1);
}
