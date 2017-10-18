/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"PaiDuiPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"

#include	"SystemSetPage.h"
#include	"SelectUserPage.h"
#include	"MyMem.h"
#include	"TimeDownNorPage.h"
#include	"PreReadCardPage.h"
#include	"SampleIDPage.h"
#include	"CardCheck_Driver.h"
#include	"MyTools.h"
#include	"MyTest_Data.h"
#include	"LunchPage.h"
#include	"System_Data.h"
#include	"Motor_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static PaiDuiPageBuffer * S_PaiDuiPageBuffer = NULL;
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

static void displayPageText(void);
static void displayTimeDownText(void);
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
MyRes createPaiDuiActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, paiduiActivityName, activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	S_PaiDuiPageBuffer->isMotorStartted = false;
	SelectPage(93);
	displayPageText();
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
	S_PaiDuiPageBuffer->lcdinput[0] = pbuf[4];
	S_PaiDuiPageBuffer->lcdinput[0] = (S_PaiDuiPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
	/*返回*/
	if(S_PaiDuiPageBuffer->lcdinput[0] == 0x1500)
	{
		//测试中，不允许返回
		if(NULL != GetCurrentTestItem())
		{
			SendKeyCode(4);
		}
		//即将测试，不允许返回
		else if(GetMinWaitTime() < 40)
		{
			SendKeyCode(3);
		}
		else if(true == isSomePaiduiInOutTimeStatus())
		{
			SendKeyCode(3);
		}
		else
		{				
			backToActivity(lunchActivityName);
		}
	}
	//继续测试
	else if(S_PaiDuiPageBuffer->lcdinput[0] == 0x1501)
	{
		S_PaiDuiPageBuffer->error = CreateANewTest(&S_PaiDuiPageBuffer->currentTestDataBuffer);
		//创建成功
		if(Error_OK == S_PaiDuiPageBuffer->error)
		{
			S_PaiDuiPageBuffer->motorAction.motorActionEnum = WaitCardPutInDef;
			S_PaiDuiPageBuffer->motorAction.motorParm = S_PaiDuiPageBuffer->currentTestDataBuffer->cardLocation;
			if(My_Pass == StartMotorAction(&S_PaiDuiPageBuffer->motorAction, true, 1, 0/portTICK_RATE_MS))
				S_PaiDuiPageBuffer->isCreate = true;
			else
				SendKeyCode(1);
		}
		//排队位置满，不允许
		else if(Error_PaiduiFull == S_PaiDuiPageBuffer->error)
			SendKeyCode(2);
		//创建失败
		else if(Error_Mem == S_PaiDuiPageBuffer->error)
			SendKeyCode(1);
		//有卡即将测试
		else if(Error_PaiDuiBusy == S_PaiDuiPageBuffer->error)
			SendKeyCode(3);
		//测试中禁止添加
		else if(Error_PaiduiTesting == S_PaiDuiPageBuffer->error)
			SendKeyCode(4);
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
	if(S_PaiDuiPageBuffer->isCreate && isMotorMoveEnd(0 / portTICK_RATE_MS))
		startActivity(createSampleActivity, NULL, NULL);
						
	if(S_PaiDuiPageBuffer->count % 5 == 0)
	{
		S_PaiDuiPageBuffer->currentTestDataBuffer = GetCurrentTestItem();
		if(S_PaiDuiPageBuffer->currentTestDataBuffer)
		{
			if(S_PaiDuiPageBuffer->currentTestDataBuffer->statues == status_testting)
			{
				if(S_PaiDuiPageBuffer->isMotorStartted)
				{
					if(isMotorMoveEnd(0 / portTICK_RATE_MS))
						startActivity(createTimeDownActivity, NULL, NULL);
					else
					{
						if(S_PaiDuiPageBuffer->count % 2 == 0)
							DspNum(0x1530 + S_PaiDuiPageBuffer->currentTestDataBuffer->index*0x10, 0x1520+S_PaiDuiPageBuffer->currentTestDataBuffer->index, 2);
						else
							DspNum(0x1530 + S_PaiDuiPageBuffer->currentTestDataBuffer->index*0x10, 0xff00, 2);
					}
				}
				else
				{
					S_PaiDuiPageBuffer->motorAction.motorActionEnum = StartTestDef;
					S_PaiDuiPageBuffer->motorAction.motorParm = S_PaiDuiPageBuffer->currentTestDataBuffer->testLocation;
					if(My_Pass == StartMotorAction(&S_PaiDuiPageBuffer->motorAction, true, 1, 100/portTICK_RATE_MS))
						S_PaiDuiPageBuffer->isMotorStartted = true;

					//显示转盘
					if(S_PaiDuiPageBuffer->currentTestDataBuffer->testLocation % 2 == 1)
						BasicPic(0x1510, 1, 94, 10, 10, 457, 461, 37, 69);
					else
						BasicPic(0x1510, 1, 94, 477, 10, 924, 461, 37, 69);
				
					//显示数字转盘
					DspNum(0x1506, S_PaiDuiPageBuffer->currentTestDataBuffer->testLocation-1, 2);
				
					for(S_PaiDuiPageBuffer->index=0; S_PaiDuiPageBuffer->index<9; S_PaiDuiPageBuffer->index++)
					{
						S_PaiDuiPageBuffer->tempPaiduiUnitData = GetTestItemByIndex(S_PaiDuiPageBuffer->index);

						if(S_PaiDuiPageBuffer->tempPaiduiUnitData)
						{
							S_PaiDuiPageBuffer->tempvalue1 = 9 - S_PaiDuiPageBuffer->index;
							S_PaiDuiPageBuffer->tempvalue1 *= 2;
							S_PaiDuiPageBuffer->tempvalue1 += S_PaiDuiPageBuffer->currentTestDataBuffer->testLocation;
							S_PaiDuiPageBuffer->tempvalue1 -= 1;
							if(S_PaiDuiPageBuffer->tempvalue1 >= 18)
								S_PaiDuiPageBuffer->tempvalue1 -= 18;

							WriteVarIcoNum(0x1520 + S_PaiDuiPageBuffer->index, S_PaiDuiPageBuffer->tempvalue1);
						}
					}
				}
			}
			
			DspNum(0x1507, 0, 2);											//显示禁止插卡图标
		}
		else
			DspNum(0x1507, 1, 2);											//显示可以插卡图标
		
		displayTimeDownText();
	}

	S_PaiDuiPageBuffer->count++;
	if(S_PaiDuiPageBuffer->count >= 60000)
		S_PaiDuiPageBuffer->count = 1;	
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
	//清除当前页面的告警弹出框
	SendKeyCode(16);
	S_PaiDuiPageBuffer->isCreate = false;
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
	SelectPage(93);
	S_PaiDuiPageBuffer->isCreate = false;
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
	//清除当前页面的告警弹出框
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
	if(NULL == S_PaiDuiPageBuffer)
	{
		S_PaiDuiPageBuffer = MyMalloc(sizeof(PaiDuiPageBuffer));
		
		if(S_PaiDuiPageBuffer)
		{
			memset(S_PaiDuiPageBuffer, 0, sizeof(PaiDuiPageBuffer));
	
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
	MyFree(S_PaiDuiPageBuffer);
	S_PaiDuiPageBuffer = NULL;
}

static void freshPage(void)
{
	S_PaiDuiPageBuffer->motor1Location = getMotorxLocation(Motor_1);
	if(S_PaiDuiPageBuffer->lastMotorLocation != S_PaiDuiPageBuffer->motor1Location)
	{
		S_PaiDuiPageBuffer->lastMotorLocation = S_PaiDuiPageBuffer->motor1Location;
		//显示转盘
		if(S_PaiDuiPageBuffer->motor1Location % 2 == 1)
			BasicPic(0x1510, 1, 94, 10, 10, 457, 461, 37, 69);
		else
			BasicPic(0x1510, 1, 94, 477, 10, 924, 461, 37, 69);
	
		//显示数字转盘
		DspNum(0x1506, S_PaiDuiPageBuffer->motor1Location-1, 2);
	
		for(S_PaiDuiPageBuffer->index=0; S_PaiDuiPageBuffer->index<9; S_PaiDuiPageBuffer->index++)
		{
			S_PaiDuiPageBuffer->tempPaiduiUnitData = GetTestItemByIndex(S_PaiDuiPageBuffer->index);

			if(S_PaiDuiPageBuffer->tempPaiduiUnitData)
			{
				S_PaiDuiPageBuffer->tempvalue1 = 9 - S_PaiDuiPageBuffer->index;
				S_PaiDuiPageBuffer->tempvalue1 *= 2;
				S_PaiDuiPageBuffer->tempvalue1 += S_PaiDuiPageBuffer->motor1Location;
				S_PaiDuiPageBuffer->tempvalue1 -= 1;
				if(S_PaiDuiPageBuffer->tempvalue1 >= 18)
					S_PaiDuiPageBuffer->tempvalue1 -= 18;

				WriteVarIcoNum(0x1520 + S_PaiDuiPageBuffer->index, S_PaiDuiPageBuffer->tempvalue1);
			}
		}
	}
}

static void displayPageText(void)
{
	S_PaiDuiPageBuffer->motor1Location = getMotorxLocation(Motor_1);
	//显示转盘
	if(S_PaiDuiPageBuffer->motor1Location % 2 == 1)
		BasicPic(0x1510, 1, 94, 10, 10, 457, 461, 37, 69);
	else
		BasicPic(0x1510, 1, 94, 477, 10, 924, 461, 37, 69);
	
	//显示数字转盘
	DspNum(0x1506, S_PaiDuiPageBuffer->motor1Location-1, 2);
	
	for(S_PaiDuiPageBuffer->index=0; S_PaiDuiPageBuffer->index<9; S_PaiDuiPageBuffer->index++)
	{
		S_PaiDuiPageBuffer->tempPaiduiUnitData = GetTestItemByIndex(S_PaiDuiPageBuffer->index);

		if(S_PaiDuiPageBuffer->tempPaiduiUnitData)
		{
			//如果有卡，则显示卡
			DspNum(0x1530 + S_PaiDuiPageBuffer->index*0x10, 0x1520+S_PaiDuiPageBuffer->index, 2);

			//卡旋转
			S_PaiDuiPageBuffer->tempvalue1 = 9 - S_PaiDuiPageBuffer->index;
			S_PaiDuiPageBuffer->tempvalue1 *= 2;
			S_PaiDuiPageBuffer->tempvalue1 += S_PaiDuiPageBuffer->motor1Location;
			S_PaiDuiPageBuffer->tempvalue1 -= 1;
			if(S_PaiDuiPageBuffer->tempvalue1 >= 18)
				S_PaiDuiPageBuffer->tempvalue1 -= 18;

			WriteVarIcoNum(0x1520 + S_PaiDuiPageBuffer->index, S_PaiDuiPageBuffer->tempvalue1);
				
			//sample id
			snprintf(S_PaiDuiPageBuffer->buf, MaxSampleIDLen, "%s", S_PaiDuiPageBuffer->tempPaiduiUnitData->testData.sampleid);
			DisText(0x15c0 + S_PaiDuiPageBuffer->index*0x10, S_PaiDuiPageBuffer->buf, strlen(S_PaiDuiPageBuffer->buf)+1);
			//item name
			snprintf(S_PaiDuiPageBuffer->buf, ItemNameLen, "%s", S_PaiDuiPageBuffer->tempPaiduiUnitData->testData.qrCode.ItemName);
			DisText(0x1650 + S_PaiDuiPageBuffer->index*0x10, S_PaiDuiPageBuffer->buf, strlen(S_PaiDuiPageBuffer->buf)+1);
		}
		else
		{
			//隐藏卡
			DspNum(0x1530 + S_PaiDuiPageBuffer->index*0x10, 0xff00, 2);
			
			//sample id
			ClearText(0x15c0 + S_PaiDuiPageBuffer->index*0x10);
			//item name
			ClearText(0x1650 + S_PaiDuiPageBuffer->index*0x10);
			//time
			DspNum(0x16e0 + S_PaiDuiPageBuffer->index, 0, 2);
		}
	}
}

static void displayTimeDownText(void)
{
	for(S_PaiDuiPageBuffer->index=0; S_PaiDuiPageBuffer->index<9; S_PaiDuiPageBuffer->index++)
	{
		S_PaiDuiPageBuffer->tempPaiduiUnitData = GetTestItemByIndex(S_PaiDuiPageBuffer->index);

		if(S_PaiDuiPageBuffer->tempPaiduiUnitData)
		{
			//time
			if(isInTimeOutStatus(S_PaiDuiPageBuffer->tempPaiduiUnitData))
				S_PaiDuiPageBuffer->tempvalue1 = timer_Count(&(S_PaiDuiPageBuffer->tempPaiduiUnitData->timeUp_timer));
			else
				S_PaiDuiPageBuffer->tempvalue1 = timer_surplus(&(S_PaiDuiPageBuffer->tempPaiduiUnitData->timeDown_timer));						
			DspNum(0x16e0 + S_PaiDuiPageBuffer->index, S_PaiDuiPageBuffer->tempvalue1, 2);
		}
		else
			DspNum(0x16e0 + S_PaiDuiPageBuffer->index, 0, 2);
	}
}
