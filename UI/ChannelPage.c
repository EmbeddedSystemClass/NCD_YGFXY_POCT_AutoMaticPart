/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"ChannelPage.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"Motor_Fun.h"
#include	"MyMem.h"
#include	"Test_Fun.h"
#include	"Test_Task.h"
#include	"CodeScan_Task.h"
#include	"CardCheck_Driver.h"
#include	"StringDefine.h"
#include	"SystemSet_Data.h"
#include	"QueueUnits.h"
#include	"System_Data.h"
#include 	"Usart1_Driver.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static ChannelPageBuffer * pageBuf = NULL;
/******************************************************************************************/
/*****************************************局部函数声明*************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);

static MyRes activityBufferMalloc(void);
static void activityBufferFree(void);

static void clearPageText(void);
static void dspTestStatus(char * str);
static void showResult(void);
static void printfChannelTestResult(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName: createWelcomeActivity
*Description: 创建欢迎界面
*Input: thizActivity -- 当前界面
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月20日16:21:51
***************************************************************************************************/
MyRes createChannelTestActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, ChannelTestActivityName, activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

static void activityStart(void)
{
	pageBuf->paiduiUnitData.ledValue = getSystemTestLedLightIntensity();
	pageBuf->paiduiUnitData.statues = statusNull;
	clearPageText();
	dspTestStatus(StopString);

	SelectPage(112);
}
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	pageBuf->lcdinput[0] = pbuf[4];
	pageBuf->lcdinput[0] = (pageBuf->lcdinput[0]<<8) + pbuf[5];
		
	//开始
	if(pageBuf->lcdinput[0] == 0x3601)
	{
		if(pageBuf->paiduiUnitData.statues != statusNull || pageBuf->paiduiUnitData.cardLocation <= 0 || pageBuf->targetCnt <= 0)
		{
			;			//error, 不动作
		}
		else
		{
			for(pageBuf->i=0; pageBuf->i<MaxChannelTestCnt; pageBuf->i++)
			{
				pageBuf->T_Point[pageBuf->i].x = 0;
				pageBuf->T_Point[pageBuf->i].y = 0;
				pageBuf->C_Point[pageBuf->i].x = 0;
				pageBuf->C_Point[pageBuf->i].y = 0;
				pageBuf->t_c[pageBuf->i] = 0;
				pageBuf->testValue[pageBuf->i] = 0;
			}
			
			pageBuf->testIndex = 0;
			pageBuf->paiduiUnitData.testLocation = pageBuf->paiduiUnitData.cardLocation;
			pageBuf->paiduiUnitData.testLocation += PaiDuiWeiNum;
			if(pageBuf->paiduiUnitData.testLocation > PaiDuiWeiNum*2)
				pageBuf->paiduiUnitData.testLocation -= PaiDuiWeiNum*2;
			pageBuf->paiduiUnitData.statues = statusNone;
			
			dspTestStatus(QualityStatusPreparingString);
		}
	}
	//读取通道
	else if(pageBuf->lcdinput[0] == 0x3603)
	{
		pageBuf->paiduiUnitData.cardLocation = 2*pbuf[8]+1;
	}
	//读取次数
	else if(pageBuf->lcdinput[0] == 0x3604)
	{
		pageBuf->targetCnt = pbuf[8];
	}
	//打印
	else if(pageBuf->lcdinput[0] == 0x3602)
	{
		printfChannelTestResult();
	}
	//取消返回
	else if(pageBuf->lcdinput[0] == 0x3600)
	{
		if(pageBuf->paiduiUnitData.statues == statusNull)
			backToFatherActivity();
		else
			SendKeyCode(3);
	}
}

static void activityFresh(void)
{
	if(pageBuf->paiduiUnitData.statues == statusNone)
	{
		FormatParmAndStartMotorAction(&pageBuf->motorAction, WaitCardPutInDef, pageBuf->paiduiUnitData.cardLocation, false);
		pageBuf->paiduiUnitData.statues = statusMotorMoveWaitCard;
	}
	else if(pageBuf->paiduiUnitData.statues == statusMotorMoveWaitCard)
	{
		if(isMotorMoveEnd(FreeRTOSZeroDelay))
		{
			if(pageBuf->scancode == CardScanNone)
			{
				pageBuf->paiduiUnitData.statues = statusWaitCardPutIn;
				dspTestStatus(QualityStatusWaitCardString);
			}
			else
			{
				pageBuf->paiduiUnitData.statues = statusWaitCardPutOut;
				dspTestStatus(QualityStatusWaitCardOutString);
			}
		}
	}
	else if(pageBuf->paiduiUnitData.statues == statusWaitCardPutIn)
	{
		if(ReadCardCheckPin)
		{
			pageBuf->paiduiUnitData.statues = statusWaitScanQR;
			StartScanQRCode(&pageBuf->paiduiUnitData.testData.qrCode);
			dspTestStatus(QualityQRScanningString);
		}
	}
	else if(pageBuf->paiduiUnitData.statues == statusWaitCardPutOut)
	{
		if(!ReadCardCheckPin)
		{
			pageBuf->paiduiUnitData.statues = statusWaitCardPutIn;
			dspTestStatus(QualityStatusWaitCardString);
		}
	}
	else if(pageBuf->paiduiUnitData.statues == statusWaitScanQR)
	{
		if(My_Pass == TakeScanQRCodeResult(&pageBuf->scancode))
		{
			if(pageBuf->scancode == CardCodeScanOK)
			{
				dspTestStatus(QualityStatusTestString);
				pageBuf->paiduiUnitData.statues = statusPrepareTest;
				FormatParmAndStartMotorAction(&pageBuf->motorAction, StartTestDef, pageBuf->paiduiUnitData.testLocation, false);
			}
			else
			{
				pageBuf->paiduiUnitData.statues = statusMotorMoveWaitCard;
				FormatParmAndStartMotorAction(&pageBuf->motorAction, WaitCardPutInDef, pageBuf->paiduiUnitData.cardLocation, false);
				dspTestStatus(QRReadErrorAndChangeCardString);
			}
		}
	}
	else if(statusPrepareTest == pageBuf->paiduiUnitData.statues)
	{
		if(isMotorMoveEnd(FreeRTOSZeroDelay))
		{
			pageBuf->paiduiUnitData.statues = status_testting;
			StartTest(&pageBuf->paiduiUnitData);
		}
	}
	else if(status_testting == pageBuf->paiduiUnitData.statues)
	{
		while(pdTRUE == TakeTestPointData(&pageBuf->i));
	
		if(My_Pass == TakeTestResult(&pageBuf->paiduiUnitData.testData.testResultDesc))
		{
			showResult();
			
			pageBuf->testIndex++;
			if(pageBuf->testIndex < pageBuf->targetCnt)
			{
				FormatParmAndStartMotorAction(&pageBuf->motorAction, PutDownCardInTestPlaceDef, pageBuf->paiduiUnitData.testLocation, false);
				pageBuf->paiduiUnitData.statues = statusWaitPutCardInTestPlace;
			}
			else
			{
				pageBuf->paiduiUnitData.statues = statusPutCardOut;
			}
		}
	}
	else if(statusWaitPutCardInTestPlace == pageBuf->paiduiUnitData.statues)
	{
		if(isMotorMoveEnd(FreeRTOSZeroDelay))
		{
			pageBuf->paiduiUnitData.statues = statusPrepareTest;
			FormatParmAndStartMotorAction(&pageBuf->motorAction, StartTestDef, pageBuf->paiduiUnitData.testLocation, false);
		}
	}
	else if(statusPutCardOut == pageBuf->paiduiUnitData.statues)
	{
		FormatParmAndStartMotorAction(&pageBuf->motorAction, PutCardOutOfDeviceAfterTestDef, pageBuf->paiduiUnitData.testLocation, false);
		pageBuf->paiduiUnitData.statues = statusWaitCardOut;
	}
	else if(statusWaitCardOut == pageBuf->paiduiUnitData.statues)
	{
		if(isMotorMoveEnd(FreeRTOSZeroDelay))
		{
			pageBuf->paiduiUnitData.statues = statusNull;
			dspTestStatus(StopString);
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
	if(NULL == pageBuf)
	{
		pageBuf = MyMalloc(sizeof(ChannelPageBuffer));
		
		if(pageBuf)
		{
			memset(pageBuf, 0, sizeof(ChannelPageBuffer));
			
			return My_Pass;
		}
	}

	return My_Fail;
}

static void activityBufferFree(void)
{
	MyFree(pageBuf);
	pageBuf = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

static void clearPageText(void)
{
	for(pageBuf->i=0; pageBuf->i<MaxChannelTestCnt; pageBuf->i++)
	{
		ClearText(0x3620 + pageBuf->i*0x08);
		ClearText(0x3668 + pageBuf->i*0x08);
		ClearText(0x36b0 + pageBuf->i*0x08);
	}

	DspNum(0x3603, 0, 2);
	DspNum(0x3604, 0, 2);
	
	dspTestStatus(QualityClearString);
}

static void dspTestStatus(char * str)
{
	sprintf(pageBuf->buf, "%-15s", str);
	
	DisText(0x3610, pageBuf->buf, strlen(pageBuf->buf)+1);
}

static void showResult(void)
{
	if(pageBuf->paiduiUnitData.testData.testResultDesc == ResultIsOK)
	{
		//T
		pageBuf->T_Point[pageBuf->testIndex].x = pageBuf->paiduiUnitData.testData.testSeries.T_Point.x;
		pageBuf->T_Point[pageBuf->testIndex].y = pageBuf->paiduiUnitData.testData.testSeries.T_Point.y;		
		sprintf(pageBuf->buf, "(%d,%d)", pageBuf->paiduiUnitData.testData.testSeries.T_Point.x, pageBuf->paiduiUnitData.testData.testSeries.T_Point.y);
		DisText(0x3620 + pageBuf->testIndex*0x08, pageBuf->buf, strlen(pageBuf->buf)+1);
		
		//C
		pageBuf->C_Point[pageBuf->testIndex].x = pageBuf->paiduiUnitData.testData.testSeries.C_Point.x;
		pageBuf->C_Point[pageBuf->testIndex].y = pageBuf->paiduiUnitData.testData.testSeries.C_Point.y;
		sprintf(pageBuf->buf, "(%d,%d)", pageBuf->paiduiUnitData.testData.testSeries.C_Point.x, pageBuf->paiduiUnitData.testData.testSeries.C_Point.y);
		DisText(0x3668 + pageBuf->testIndex*0x08, pageBuf->buf, strlen(pageBuf->buf)+1);
		
		//RESULT
		pageBuf->t_c[pageBuf->testIndex] = pageBuf->paiduiUnitData.testData.testSeries.t_c;
		pageBuf->testValue[pageBuf->testIndex] = pageBuf->paiduiUnitData.testData.testSeries.result;
		sprintf(pageBuf->buf, "(%.3f,%.*f)", pageBuf->paiduiUnitData.testData.testSeries.t_c, pageBuf->paiduiUnitData.testData.qrCode.itemConstData.pointNum,
			pageBuf->paiduiUnitData.testData.testSeries.result);
		DisText(0x36b0 + pageBuf->testIndex*0x08, pageBuf->buf, strlen(pageBuf->buf)+1);
	}
	else
	{
		//T
		DisText(0x3620 + pageBuf->testIndex*0x08, NoneString, strlen(NoneString)+1);
		
		//C
		DisText(0x3668 + pageBuf->testIndex*0x08, NoneString, strlen(NoneString)+1);
		
		//RESULT
		DisText(0x36b0 + pageBuf->testIndex*0x08, TestErrorString, strlen(TestErrorString)+1);
	}
}
 
static void printfChannelTestResult(void)
{
	sprintf(pageBuf->buf, "------------------------------\n");
	SendDataToQueue(GetUsart1TXQueue(), NULL, pageBuf->buf, strlen(pageBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
	
	sprintf(pageBuf->buf, "No     T         C         结果\n");
	SendDataToQueue(GetUsart1TXQueue(), NULL, pageBuf->buf, strlen(pageBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
	
	for(pageBuf->i=0; pageBuf->i<pageBuf->testIndex; pageBuf->i++)
	{
		sprintf(pageBuf->buf, "%d  (%d,%d) (%d,%d) (%.3f,%.3f)\n", pageBuf->i+1, pageBuf->T_Point[pageBuf->i].x, pageBuf->T_Point[pageBuf->i].y,
			pageBuf->C_Point[pageBuf->i].x, pageBuf->C_Point[pageBuf->i].y, pageBuf->t_c[pageBuf->i], pageBuf->testValue[pageBuf->i]);
		SendDataToQueue(GetUsart1TXQueue(), NULL, pageBuf->buf, strlen(pageBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
	}
	
	getSystemTime(&pageBuf->dateTime);
	sprintf(pageBuf->buf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n", PrintTimeStr, pageBuf->dateTime.year, pageBuf->dateTime.month, pageBuf->dateTime.day
		, pageBuf->dateTime.hour, pageBuf->dateTime.min, pageBuf->dateTime.sec);
	SendDataToQueue(GetUsart1TXQueue(), NULL, pageBuf->buf, strlen(pageBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
	
	getSystemDeviceId(&pageBuf->buf[50]);
	sprintf(pageBuf->buf, "%s: %s\n", TestDeviceIdString, &pageBuf->buf[50]);
	SendDataToQueue(GetUsart1TXQueue(), NULL, pageBuf->buf, strlen(pageBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
	
	sprintf(pageBuf->buf, "------------------------------\n\n\n");
	SendDataToQueue(GetUsart1TXQueue(), NULL, pageBuf->buf, strlen(pageBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
}

