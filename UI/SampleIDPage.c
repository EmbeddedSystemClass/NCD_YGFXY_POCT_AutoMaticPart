/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"SampleIDPage.h"
#include	"WaittingCardPage.h"
#include	"StringDefine.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"System_Data.h"
#include	"MyMem.h"
#include	"MyTools.h"
#include	"MyTest_Data.h"
#include	"CodeScanner_Driver.h"
#include	"MyTest_Data.h"
#include	"SelectUserPage.h"
#include	"WaittingCardPage.h"
#include	"ReadBarCode_Fun.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static SampleIDPage *S_SampleIDPage = NULL;
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

static void RefreshSampleID(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: 创建选择操作人界面
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:09
***************************************************************************************************/
MyRes createSampleActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SampleActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: 显示主界面
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:32
***************************************************************************************************/
static void activityStart(void)
{
	SelectPage(86);
	
	S_SampleIDPage->currenttestdata = GetCurrentTestItem();
	S_SampleIDPage->currenttestdata->statues = statusInputSampleID;
	
	S_SampleIDPage->sampleIdLen = 0;
	clearBarCodeQueue();
	
	S_SampleIDPage->motorAction.motorActionEnum = WaitCardPutInDef;
	S_SampleIDPage->motorAction.motorParm = S_SampleIDPage->currenttestdata->cardLocation;
	S_SampleIDPage->isMotorStartted = My_Fail;
		
	RefreshSampleID();	
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: 界面输入
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	S_SampleIDPage->lcdinput[0] = pbuf[4];
	S_SampleIDPage->lcdinput[0] = (S_SampleIDPage->lcdinput[0]<<8) + pbuf[5];
		
	//返回
	if(S_SampleIDPage->lcdinput[0] == 0x1300)
	{
		if(checkFatherActivityIs(paiduiActivityName))
			DeleteCurrentTest();
	
		backToFatherActivity();
	}
	//确定
	else if(S_SampleIDPage->lcdinput[0] == 0x1301)
	{
		if(S_SampleIDPage->sampleIdLen > 0)
			S_SampleIDPage->isConfirmed = true;
		else
			SendKeyCode(1);
	}
	//获取输入的id
	else if(S_SampleIDPage->lcdinput[0] == 0x1310)
	{
		memset(S_SampleIDPage->currenttestdata->testData.sampleid, 0, MaxSampleIDLen);
		S_SampleIDPage->sampleIdLen = GetBufLen(&pbuf[7] , 2*pbuf[6]);
		memcpy(S_SampleIDPage->currenttestdata->testData.sampleid, &pbuf[7], S_SampleIDPage->sampleIdLen);
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: 界面刷新
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:01:16
***************************************************************************************************/
static void activityFresh(void)
{
	S_SampleIDPage->cnt++;
	
	if(S_SampleIDPage->cnt % 5 == 0)
	{
		if(S_SampleIDPage->isMotorStartted == My_Pass)
		{
			if(isMotorMoveEnd(0))
				startActivity(createWaittingCardActivity, NULL, NULL);
		}
		else if(S_SampleIDPage->sampleIdLen > 0)
		{
			S_SampleIDPage->isMotorStartted = StartMotorAction(&S_SampleIDPage->motorAction, false);
			if(S_SampleIDPage->isMotorStartted == My_Pass)
				SendKeyCode(2);
		}
		else
		{
			//从条码枪读取样品编号
			S_SampleIDPage->tempv1 = ReadBarCodeFunction((char *)(S_SampleIDPage->tempbuf), 100);
			if(S_SampleIDPage->tempv1 > 0)
			{
				S_SampleIDPage->sampleIdLen = S_SampleIDPage->tempv1;
				sprintf(S_SampleIDPage->currenttestdata->testData.sampleid, "%.*s", S_SampleIDPage->sampleIdLen, S_SampleIDPage->tempbuf);
				
				RefreshSampleID();
			}
		}
	}
}

/***************************************************************************************************
*FunctionName: activityHide
*Description: 隐藏界面时要做的事
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:01:40
***************************************************************************************************/
static void activityHide(void)
{
	S_SampleIDPage->isMotorStartted = My_Fail;
	S_SampleIDPage->isConfirmed = false;
	SendKeyCode(16);
}

/***************************************************************************************************
*FunctionName: activityResume
*Description: 界面恢复显示时要做的事
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:01:58
***************************************************************************************************/
static void activityResume(void)
{
	RefreshSampleID();
	
	SelectPage(86);
}

/***************************************************************************************************
*FunctionName: activityDestroy
*Description: 界面销毁
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:02:15
***************************************************************************************************/
static void activityDestroy(void)
{
	SendKeyCode(16);
	activityBufferFree();
}

/***************************************************************************************************
*FunctionName: activityBufferMalloc
*Description: 界面数据内存申请
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static MyRes activityBufferMalloc(void)
{
	if(NULL == S_SampleIDPage)
	{
		S_SampleIDPage = MyMalloc(sizeof(SampleIDPage));
		
		if(S_SampleIDPage)
		{
			memset(S_SampleIDPage, 0, sizeof(SampleIDPage));
	
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
*Description: 界面内存释放
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:03:10
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(S_SampleIDPage);
	S_SampleIDPage = NULL;
}

static void RefreshSampleID(void)
{
	snprintf(S_SampleIDPage->tempbuf, MaxSampleIDLen, "%s", S_SampleIDPage->currenttestdata->testData.sampleid);
	DisText(0x1310, S_SampleIDPage->tempbuf, strlen(S_SampleIDPage->tempbuf)+1);
	S_SampleIDPage->sampleIdLen = strlen(S_SampleIDPage->currenttestdata->testData.sampleid);
}
