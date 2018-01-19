/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"AdjustLedPage.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"System_Data.h"
#include	"Motor_Fun.h"
#include	"MyMem.h"
#include	"Test_Fun.h"
#include	"Test_Task.h"
#include	"SystemSet_Data.h"
#include	"SystemSet_Dao.h"
#include	"CardCheck_Driver.h"
#include	"StringDefine.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static AdjustLedPageBuffer *S_AdjustLedPageBuffer = NULL;
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
static void analysisTestData(void);
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
MyRes createAdjustLedActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "AdjustLedActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

static void activityStart(void)
{
	S_AdjustLedPageBuffer->currentLedValue = getSystemTestLedLightIntensity();
	S_AdjustLedPageBuffer->paiduiUnitData.statues = statusNull;
	clearPageText();
	dspTestStatus(StopString);

	SelectPage(140);
}
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	S_AdjustLedPageBuffer->lcdinput[0] = pbuf[4];
	S_AdjustLedPageBuffer->lcdinput[0] = (S_AdjustLedPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
	//开始校准
	if(S_AdjustLedPageBuffer->lcdinput[0] == 0x2602)
	{
		if(S_AdjustLedPageBuffer->paiduiUnitData.statues != statusNull || S_AdjustLedPageBuffer->targetValue <= 0 || S_AdjustLedPageBuffer->targetRange <= 0)
		{
			SendKeyCode(3);
		}
		else
		{
			S_AdjustLedPageBuffer->paiduiUnitData.ledValue = MiddleLedValue;
			S_AdjustLedPageBuffer->testIndex = 0;
			
			//在1号位测试
			S_AdjustLedPageBuffer->paiduiUnitData.cardLocation = 1;
			S_AdjustLedPageBuffer->paiduiUnitData.testLocation = 10;
			
			//在通道0校准
			S_AdjustLedPageBuffer->paiduiUnitData.testData.qrCode.ChannelNum = 0;
			S_AdjustLedPageBuffer->paiduiUnitData.statues = statusNone;
			
			DspNum(0x2605, S_AdjustLedPageBuffer->testIndex, 2);
			DspNum(0x2604, S_AdjustLedPageBuffer->paiduiUnitData.ledValue, 2);
				
			dspTestStatus(QualityStatusPreparingString);
		}
	}
	//读取目标值
	else if(S_AdjustLedPageBuffer->lcdinput[0] == 0x2610)
	{
		getLcdInputData(S_AdjustLedPageBuffer->buf, &pbuf[7]);	
		S_AdjustLedPageBuffer->targetValue = strtol(S_AdjustLedPageBuffer->buf , NULL, 10);
	}
	//读取误差值
	else if(S_AdjustLedPageBuffer->lcdinput[0] == 0x2640)
	{
		getLcdInputData(S_AdjustLedPageBuffer->buf, &pbuf[7]);
		S_AdjustLedPageBuffer->targetRange = strtol(S_AdjustLedPageBuffer->buf , NULL, 10);
	}
	//保存校准值
	else if(S_AdjustLedPageBuffer->lcdinput[0] == 0x2600)
	{
		if((S_AdjustLedPageBuffer->paiduiUnitData.ledValue >= MinLedValue) && (S_AdjustLedPageBuffer->paiduiUnitData.ledValue <= MaxLedValue))
		{
			//获取最新的系统参数
			readGbSystemSetData(&S_AdjustLedPageBuffer->systemSetData);
			
			//修改系统参数副本中值
			S_AdjustLedPageBuffer->systemSetData.testLedLightIntensity = S_AdjustLedPageBuffer->paiduiUnitData.ledValue;
				
			if(My_Pass == SaveSystemSetData(&(S_AdjustLedPageBuffer->systemSetData)))
			{
				SendKeyCode(1);
				S_AdjustLedPageBuffer->currentLedValue = getSystemTestLedLightIntensity();
				DspNum(0x2609, S_AdjustLedPageBuffer->currentLedValue, 2);
			}
			else
				SendKeyCode(2);
		}
	}
	//取消返回
	else if(S_AdjustLedPageBuffer->lcdinput[0] == 0x2601)
	{
		if(S_AdjustLedPageBuffer->paiduiUnitData.statues == statusNull)
			backToFatherActivity();
		else
			SendKeyCode(3);
	}
}

static void activityFresh(void)
{
	if(S_AdjustLedPageBuffer->paiduiUnitData.statues == statusNone)
	{
		FormatParmAndStartMotorAction(&S_AdjustLedPageBuffer->motorAction, WaitCardPutInDef, S_AdjustLedPageBuffer->paiduiUnitData.cardLocation, false);
		S_AdjustLedPageBuffer->paiduiUnitData.statues = statusMotorMoveWaitCard;
	}
	else if(S_AdjustLedPageBuffer->paiduiUnitData.statues == statusMotorMoveWaitCard)
	{
		if(isMotorMoveEnd(FreeRTOSZeroDelay))
		{
			S_AdjustLedPageBuffer->paiduiUnitData.statues = statusWaitCardPutIn;
			dspTestStatus(QualityStatusWaitCardString);
		}
	}
	else if(S_AdjustLedPageBuffer->paiduiUnitData.statues == statusWaitCardPutIn)
	{
		if(ReadCardCheckPin)
		{
			dspTestStatus(QualityStatusTestString);
			FormatParmAndStartMotorAction(&S_AdjustLedPageBuffer->motorAction, PutDownCardInPlaceDef, S_AdjustLedPageBuffer->paiduiUnitData.cardLocation, false);
			S_AdjustLedPageBuffer->paiduiUnitData.statues = statusMotorPutCardDown;
		}
	}
	else if(statusMotorPutCardDown == S_AdjustLedPageBuffer->paiduiUnitData.statues)
	{
		if(isMotorMoveEnd(FreeRTOSZeroDelay))
		{
			S_AdjustLedPageBuffer->paiduiUnitData.statues = statusPrepareTest;
			FormatParmAndStartMotorAction(&S_AdjustLedPageBuffer->motorAction, StartTestDef, S_AdjustLedPageBuffer->paiduiUnitData.testLocation, false);
		}
	}
	else if(statusPrepareTest == S_AdjustLedPageBuffer->paiduiUnitData.statues)
	{
		if(isMotorMoveEnd(FreeRTOSZeroDelay))
		{
			S_AdjustLedPageBuffer->paiduiUnitData.statues = status_testting;
			StartTest(&S_AdjustLedPageBuffer->paiduiUnitData);
		}
	}
	else if(status_testting == S_AdjustLedPageBuffer->paiduiUnitData.statues)
	{
		while(pdTRUE == TakeTestPointData(&S_AdjustLedPageBuffer->i));
	
		if(My_Pass == TakeTestResult(&S_AdjustLedPageBuffer->paiduiUnitData.testData.testResultDesc))
		{
			FormatParmAndStartMotorAction(&S_AdjustLedPageBuffer->motorAction, PutDownCardInTestPlaceDef, S_AdjustLedPageBuffer->paiduiUnitData.testLocation, false);
			S_AdjustLedPageBuffer->paiduiUnitData.statues = statusWaitPutCardInTestPlace;
		}
	}
	else if(statusWaitPutCardInTestPlace == S_AdjustLedPageBuffer->paiduiUnitData.statues)
	{
		if(isMotorMoveEnd(FreeRTOSZeroDelay))
		{
			analysisTestData();
		}
	}
	else if(statusPutCardOut == S_AdjustLedPageBuffer->paiduiUnitData.statues)
	{
		FormatParmAndStartMotorAction(&S_AdjustLedPageBuffer->motorAction, PutCardOutOfDeviceAfterTestDef, S_AdjustLedPageBuffer->paiduiUnitData.testLocation, false);
		S_AdjustLedPageBuffer->paiduiUnitData.statues = statusWaitCardOut;
	}
	else if(statusWaitCardOut == S_AdjustLedPageBuffer->paiduiUnitData.statues)
	{
		if(isMotorMoveEnd(FreeRTOSZeroDelay))
		{
			S_AdjustLedPageBuffer->paiduiUnitData.statues = statusNull;
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
	if(NULL == S_AdjustLedPageBuffer)
	{
		S_AdjustLedPageBuffer = MyMalloc(sizeof(AdjustLedPageBuffer));
		
		if(S_AdjustLedPageBuffer)
		{
			memset(S_AdjustLedPageBuffer, 0, sizeof(AdjustLedPageBuffer));
			
			return My_Pass;
		}
	}

	return My_Fail;
}

static void activityBufferFree(void)
{
	MyFree(S_AdjustLedPageBuffer);
	S_AdjustLedPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

static void clearPageText(void)
{
	ClearText(0x2610);
	ClearText(0x2630);
	ClearText(0x2640);
	
	DspNum(0x2603, 0, 2);
	DspNum(0x2604, 0, 2);
	DspNum(0x2605, 0, 2);
	
	//显示当前校准值
	DspNum(0x2609, S_AdjustLedPageBuffer->currentLedValue, 2);
}

static void dspTestStatus(char * str)
{
	sprintf(S_AdjustLedPageBuffer->buf, "%-15s", str);
	
	DisText(0x2620, S_AdjustLedPageBuffer->buf, strlen(S_AdjustLedPageBuffer->buf)+1);
}

static void analysisTestData(void)
{
	//找最大值
	S_AdjustLedPageBuffer->maxPoint[0] = S_AdjustLedPageBuffer->paiduiUnitData.testData.testSeries.TestPoint[0];

	for(S_AdjustLedPageBuffer->i=0; S_AdjustLedPageBuffer->i<MaxPointLen; S_AdjustLedPageBuffer->i++)
	{
		if(S_AdjustLedPageBuffer->maxPoint[0] < S_AdjustLedPageBuffer->paiduiUnitData.testData.testSeries.TestPoint[S_AdjustLedPageBuffer->i])
		{
			S_AdjustLedPageBuffer->maxPoint[0] = S_AdjustLedPageBuffer->paiduiUnitData.testData.testSeries.TestPoint[S_AdjustLedPageBuffer->i];
			S_AdjustLedPageBuffer->maxPoint[1] = S_AdjustLedPageBuffer->i;
		}
	}
	
	sprintf(S_AdjustLedPageBuffer->buf, "%d", S_AdjustLedPageBuffer->maxPoint[0]);
	DisText(0x2630, S_AdjustLedPageBuffer->buf, strlen(S_AdjustLedPageBuffer->buf)+1);
	
	DspNum(0x2603, S_AdjustLedPageBuffer->maxPoint[1], 2);
	
	if(abs(S_AdjustLedPageBuffer->targetValue - S_AdjustLedPageBuffer->maxPoint[0]) < S_AdjustLedPageBuffer->targetRange)
	{
		;
	}
	else if(S_AdjustLedPageBuffer->targetValue > S_AdjustLedPageBuffer->maxPoint[0])
	{
		//继续调节
		if(S_AdjustLedPageBuffer->paiduiUnitData.ledValue < MaxLedValue)
		{
			S_AdjustLedPageBuffer->paiduiUnitData.ledValue += AdjustLedStep;
			S_AdjustLedPageBuffer->testIndex++;
			
			if(S_AdjustLedPageBuffer->testIndex < MaxAdjustCnt)
			{
				DspNum(0x2605, S_AdjustLedPageBuffer->testIndex, 2);
				DspNum(0x2604, S_AdjustLedPageBuffer->paiduiUnitData.ledValue, 2);
						
				S_AdjustLedPageBuffer->paiduiUnitData.statues = statusPrepareTest;
				FormatParmAndStartMotorAction(&S_AdjustLedPageBuffer->motorAction, StartTestDef, S_AdjustLedPageBuffer->paiduiUnitData.testLocation, false);
					
				return;
			}
		}
	}
	else
	{
		//继续调节
		if(S_AdjustLedPageBuffer->paiduiUnitData.ledValue > MinLedValue)
		{
			S_AdjustLedPageBuffer->paiduiUnitData.ledValue -= AdjustLedStep;
			S_AdjustLedPageBuffer->testIndex++;
			
			if(S_AdjustLedPageBuffer->testIndex < MaxAdjustCnt)
			{
				DspNum(0x2605, S_AdjustLedPageBuffer->testIndex, 2);
				DspNum(0x2604, S_AdjustLedPageBuffer->paiduiUnitData.ledValue, 2);
						
				S_AdjustLedPageBuffer->paiduiUnitData.statues = statusPrepareTest;
				FormatParmAndStartMotorAction(&S_AdjustLedPageBuffer->motorAction, StartTestDef, S_AdjustLedPageBuffer->paiduiUnitData.testLocation, false);
					
				return;
			}
		}
	}
	
	DspNum(0x2605, S_AdjustLedPageBuffer->testIndex, 2);
	DspNum(0x2604, S_AdjustLedPageBuffer->paiduiUnitData.ledValue, 2);
	dspTestStatus(StopString);
	S_AdjustLedPageBuffer->paiduiUnitData.statues = statusPutCardOut;
}

