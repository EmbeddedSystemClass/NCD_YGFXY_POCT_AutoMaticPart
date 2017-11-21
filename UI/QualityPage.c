/***************************************************************************************************
*FileName: MaintenancePage
*Description: 设备维护
*Author:xsx
*Data: 2017年6月14日 15:51:52
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"QualityPage.h"

#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"MyTools.h"
#include	"StringDefine.h"

#include	"Test_Fun.h"
#include	"Quality_Data.h"
#include	"SystemSetPage.h"
#include	"SelectUserPage.h"
#include	"CodeScan_Task.h"
#include	"Test_Task.h"
#include	"System_Data.h"
#include	"DeviceQualityDao.h"
#include	"CardCheck_Driver.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/**************************************变量声明*************************************************/
/***************************************************************************************************/
static QualityPageBuffer * pageBuffer = NULL;
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyRes activityBufferMalloc(void);
static void activityBufferFree(void);

static void showStatus(const char * str, unsigned char index);
static void showThisTestResult(void);
static void showAllPianCha(void);
static void clearPageText(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: createLunchActivity
*Description: 创建主界面
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:09
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
*Description: 显示主界面
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:32
***************************************************************************************************/
static void activityStart(void)
{
	pageBuffer->deviceQuality = getGB_DeviceQuality();
	pageBuffer->testIndex = 0;
	pageBuffer->resultSum = 0;
	pageBuffer->resultNum = 0;
	pageBuffer->isCancel = false;

	clearPageText();

	SelectPage(158);
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
	/*命令*/
	pageBuffer->lcdinput[0] = pbuf[4];
	pageBuffer->lcdinput[0] = (pageBuffer->lcdinput[0]<<8) + pbuf[5];
		
	//返回
	if(pageBuffer->lcdinput[0] == 0x3400)
	{
		pageBuffer->isCancel = true;
		
	}
	//提交
	else if(pageBuffer->lcdinput[0] == 0x3401)
	{
		if(pageBuffer->resultNum > 0 && pageBuffer->deviceQuality->standardValue != 0)
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
	}
	//设置标准值
	else if(pageBuffer->lcdinput[0] == 0x350A)
	{
		memset(pageBuffer->tempBuf, 0, 20);
		memcpy(pageBuffer->tempBuf, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
		pageBuffer->deviceQuality->standardValue = strtod(pageBuffer->tempBuf , NULL);

		showAllPianCha();
	}
	//结果
	else if(pageBuffer->lcdinput[0] == 0x3402)
	{
		pageBuffer->lcdinput[1] = pbuf[7];
		pageBuffer->lcdinput[1] = (pageBuffer->lcdinput[1]<<8) + pbuf[8];
			
		if(pageBuffer->lcdinput[1] == 1)
			pageBuffer->deviceQuality->isOk = true;
		else if(pageBuffer->lcdinput[1] == 0x0000)
			pageBuffer->deviceQuality->isOk = false;
	}
	//维护说明
	else if(pageBuffer->lcdinput[0] == 0x3520)
	{
		snprintf(pageBuffer->deviceQuality->desc, GetBufLen(&pbuf[7], 2*pbuf[6])+1, "%s",&pbuf[7]);
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
	pageBuffer->ScheduleCount++;
	
	pageBuffer->minTime = 0xffff;
	pageBuffer->statuesNullNum = 0;
	for(pageBuffer->i=0; pageBuffer->i<PaiDuiWeiNum; pageBuffer->i++)
	{
		pageBuffer->tempPaiduiUnitData = &pageBuffer->paiduiUnitData[pageBuffer->i];
		
		if(pageBuffer->tempPaiduiUnitData->statues == statusNull)
		{
			pageBuffer->statuesNullNum++;
			showStatus(QualityClearString, pageBuffer->i);
		}
		else if(pageBuffer->tempPaiduiUnitData->statues != status_end)
		{
			if(pageBuffer->tempPaiduiUnitData->statues == status_start)
				pageBuffer->tempPaiduiUnitData->statues = status_timedown;
			
			if(pageBuffer->tempPaiduiUnitData->statues == status_timedown || pageBuffer->tempPaiduiUnitData->statues == status_timeup)
			{
				if(pageBuffer->isCancel)
				{
					if(pageBuffer->currentPaiduiUnitData == NULL)
					{
						pageBuffer->tempPaiduiUnitData->statues = statusPutCardOut;
						pageBuffer->currentPaiduiUnitData = pageBuffer->tempPaiduiUnitData;
						showStatus(QualityCancellingString, pageBuffer->tempPaiduiUnitData->index);
						break;
					}
					else
					{
						showStatus(QualityPrepareCancelString, pageBuffer->tempPaiduiUnitData->index);
					}
				}
				else
				{
					if(pageBuffer->tempPaiduiUnitData->statues == status_timedown)
					{
						pageBuffer->tempIntV1 = timer_surplus(&(pageBuffer->tempPaiduiUnitData->timeDown_timer));
						sprintf(pageBuffer->tempBuf, "%s %02d:%02d", QualityTimeDownString, pageBuffer->tempIntV1/60, pageBuffer->tempIntV1%60);
						if(pageBuffer->minTime > pageBuffer->tempIntV1)
							pageBuffer->minTime = pageBuffer->tempIntV1;
						
						if(0 == pageBuffer->tempIntV1)
						{
							pageBuffer->tempPaiduiUnitData->statues = status_timeup;
							timer_restart(&(pageBuffer->tempPaiduiUnitData->timeUp_timer));				//启动超时计时器
						}
						else if(pageBuffer->tempIntV1 <= 30)
						{
							if(NULL == pageBuffer->currentPaiduiUnitData || pageBuffer->tempPaiduiUnitData == pageBuffer->currentPaiduiUnitData)
							{
								pageBuffer->currentPaiduiUnitData = pageBuffer->tempPaiduiUnitData;
								pageBuffer->tempPaiduiUnitData->statues = statusTestMotor;
								FormatParmAndStartMotorAction(&pageBuffer->motorAction, StartTestDef, pageBuffer->currentPaiduiUnitData->testLocation, false, false);
								showStatus(QualityTestingString, pageBuffer->currentPaiduiUnitData->index);
							}
						}
						else if(pageBuffer->tempPaiduiUnitData == pageBuffer->currentPaiduiUnitData)
							pageBuffer->currentPaiduiUnitData = NULL;
					}
					else if(pageBuffer->tempPaiduiUnitData->statues == status_timeup)
					{
						pageBuffer->tempIntV1 = timer_Count(&(pageBuffer->tempPaiduiUnitData->timeUp_timer));
						sprintf(pageBuffer->tempBuf, "%s %02d:%02d", QualityTimeUpString, pageBuffer->tempIntV1/60, pageBuffer->tempIntV1%60);
						pageBuffer->minTime = 0;
						
						if(pageBuffer->currentPaiduiUnitData == NULL)
						{
							pageBuffer->currentPaiduiUnitData = pageBuffer->tempPaiduiUnitData;
							pageBuffer->tempPaiduiUnitData->statues = statusTestMotor;
							FormatParmAndStartMotorAction(&pageBuffer->motorAction, StartTestDef, pageBuffer->currentPaiduiUnitData->testLocation, false, false);
							showStatus(QualityTestingString, pageBuffer->currentPaiduiUnitData->index);
						} 
					}
					
					if( pageBuffer->ScheduleCount % 5 == 0)
					{
						showStatus(pageBuffer->tempBuf, pageBuffer->tempPaiduiUnitData->index);
					}
				}
			}
		}
	}
	
	if(pageBuffer->statuesNullNum >= MaxQualityCount && pageBuffer->isCancel)
	{
		deleteGB_DeviceQuality();
		backToActivity(SystemSetActivityName);
		return;
	}
	
	if(pageBuffer->currentPaiduiUnitData)
	{
		if(pageBuffer->minTime < 30 && (statusNone == pageBuffer->currentPaiduiUnitData->statues || statusWaitCardPutIn == pageBuffer->currentPaiduiUnitData->statues))
		{
			pageBuffer->currentPaiduiUnitData->statues = statusNull;
			pageBuffer->currentPaiduiUnitData = NULL;
		}
		else if(statusNone == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(pageBuffer->isCancel)
			{
				pageBuffer->currentPaiduiUnitData->statues = statusNull;
				pageBuffer->currentPaiduiUnitData = NULL;
				return;
			}
			
			FormatParmAndStartMotorAction(&pageBuffer->motorAction, WaitCardPutInDef, pageBuffer->currentPaiduiUnitData->cardLocation, false, false);
			pageBuffer->currentPaiduiUnitData->statues = statusMotorMoveQR;
		}
		else if(statusMotorMoveQR == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(isMotorMoveEnd(0 / portTICK_RATE_MS))
			{
				if(pageBuffer->cardScanResult == CardScanNone)
				{
					pageBuffer->currentPaiduiUnitData->statues = statusWaitCardPutIn;
					showStatus(QualityStatusWaitCardString, pageBuffer->currentPaiduiUnitData->index);
				}
				else
				{
					pageBuffer->currentPaiduiUnitData->statues = statusWaitCardPutOut;
					showStatus(QualityStatusWaitCardOutString, pageBuffer->currentPaiduiUnitData->index);
				}
			}
		}
		else if(statusWaitCardPutIn == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(ReadCardCheckPin)
			{
				if(pageBuffer->isCancel)
				{
					FormatParmAndStartMotorAction(&pageBuffer->motorAction, PutDownCardInPlaceDef, pageBuffer->currentPaiduiUnitData->cardLocation, false, false);
					pageBuffer->currentPaiduiUnitData->statues = statusMotorPutCardDown;
					return;
				}
			
				pageBuffer->currentPaiduiUnitData->statues = statusWaitScanQR;
				StartScanQRCode(&pageBuffer->currentPaiduiUnitData->testData.qrCode);
				showStatus(QualityQRScanningString, pageBuffer->currentPaiduiUnitData->index);
			}
			else if(pageBuffer->isCancel)
			{
				pageBuffer->currentPaiduiUnitData->statues = statusNull;
				pageBuffer->currentPaiduiUnitData = NULL;
				return;
			}
		}
		else if(statusWaitCardPutOut == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(!ReadCardCheckPin)
			{
				if(pageBuffer->isCancel)
				{
					pageBuffer->currentPaiduiUnitData->statues = statusNull;
					pageBuffer->currentPaiduiUnitData = NULL;
					return;
				}
				
				pageBuffer->currentPaiduiUnitData->statues = statusWaitCardPutIn;
				showStatus(QualityStatusWaitCardString, pageBuffer->currentPaiduiUnitData->index);
			}
			else if(pageBuffer->isCancel)
			{
				FormatParmAndStartMotorAction(&pageBuffer->motorAction, PutDownCardInPlaceDef, pageBuffer->currentPaiduiUnitData->cardLocation, false, false);
				pageBuffer->currentPaiduiUnitData->statues = statusMotorPutCardDown;
				return;
			}
		}
		else if(statusWaitScanQR == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(My_Pass == TakeScanQRCodeResult(&pageBuffer->cardScanResult))
			{
				if(pageBuffer->cardScanResult == CardCodeScanOK)
				{
					if(pageBuffer->testIndex == 0)
					{
						memcpy(pageBuffer->deviceQuality->itemName, pageBuffer->currentPaiduiUnitData->testData.qrCode.ItemName, ItemNameLen);
						DisText(0x3510, pageBuffer->currentPaiduiUnitData->testData.qrCode.ItemName, ItemNameLen);
						pageBuffer->motorAction.motorActionEnum = OriginLocationDef;
						pageBuffer->motorAction.motorParm = pageBuffer->currentPaiduiUnitData->cardLocation+1;
						pageBuffer->currentPaiduiUnitData->statues = statusMotorOrigin;
						timer_SetAndStart(&(pageBuffer->currentPaiduiUnitData->timeDown_timer), pageBuffer->currentPaiduiUnitData->testData.qrCode.CardWaitTime);
						showStatus(QRReadSuccessString, pageBuffer->currentPaiduiUnitData->index);
					}
					else if(CheckStrIsSame(pageBuffer->paiduiUnitData[0].testData.qrCode.ItemName, pageBuffer->currentPaiduiUnitData->testData.qrCode.ItemName, ItemNameLen))
					{
						pageBuffer->motorAction.motorActionEnum = OriginLocationDef;
						pageBuffer->motorAction.motorParm = pageBuffer->currentPaiduiUnitData->cardLocation+1;
						pageBuffer->currentPaiduiUnitData->statues = statusMotorOrigin;
						timer_SetAndStart(&(pageBuffer->currentPaiduiUnitData->timeDown_timer), pageBuffer->currentPaiduiUnitData->testData.qrCode.CardWaitTime);
						showStatus(QRReadSuccessString, pageBuffer->currentPaiduiUnitData->index);
					}
					else
					{
						pageBuffer->motorAction.motorActionEnum = WaitCardPutInDef;
						pageBuffer->motorAction.motorParm = pageBuffer->currentPaiduiUnitData->cardLocation;
						pageBuffer->currentPaiduiUnitData->statues = statusMotorMoveQR;
						showStatus(QRItemChangedString, pageBuffer->currentPaiduiUnitData->index);
					}
				}
				else
				{
					pageBuffer->motorAction.motorActionEnum = WaitCardPutInDef;
					pageBuffer->motorAction.motorParm = pageBuffer->currentPaiduiUnitData->cardLocation;
					pageBuffer->currentPaiduiUnitData->statues = statusMotorMoveQR;
					showStatus(QRReadErrorAndChangeCardString, pageBuffer->currentPaiduiUnitData->index);
				}
				
				StartMotorAction(&pageBuffer->motorAction, false, false);
			}
		}
		else if(statusMotorPutCardDown == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(isMotorMoveEnd(0 / portTICK_RATE_MS))
			{
				if(pageBuffer->isCancel)
				{
					pageBuffer->currentPaiduiUnitData->statues = statusPutCardOut;
					return;
				}
			}
		}
		else if(statusMotorOrigin == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(isMotorMoveEnd(0 / portTICK_RATE_MS))
			{
				if(pageBuffer->isCancel)
				{
					pageBuffer->currentPaiduiUnitData->statues = statusPutCardOut;
					return;
				}
				pageBuffer->currentPaiduiUnitData->statues = status_start;
				pageBuffer->testIndex++;
			}
		}
		else if(statusTestMotor == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(isMotorMoveEnd(0 / portTICK_RATE_MS))
			{
				if(pageBuffer->isCancel)
				{
					pageBuffer->currentPaiduiUnitData->statues = statusPutCardOut;
					return;
				}
				pageBuffer->currentPaiduiUnitData->statues = statusPrepareTest;
			}
		}
		else if(statusPrepareTest == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(pageBuffer->isCancel)
			{
				pageBuffer->currentPaiduiUnitData->statues = statusPutCardOut;
				return;
			}
			
			if(TimerOut == timer_expired(&pageBuffer->currentPaiduiUnitData->timeDown_timer))
			{
				pageBuffer->currentPaiduiUnitData->statues = status_testting;
				StartTest(&pageBuffer->currentPaiduiUnitData->testData);
			}
		}
		else if(status_testting == pageBuffer->currentPaiduiUnitData->statues)
		{
			while(pdTRUE == TakeTestPointData(&pageBuffer->tempShortV1));
	
			if(My_Pass == TakeTestResult(&pageBuffer->currentPaiduiUnitData->testData.testResultDesc))
			{
				pageBuffer->currentPaiduiUnitData->statues = statusPutCardOut;	
			}
		}
		else if(statusPutCardOut == pageBuffer->currentPaiduiUnitData->statues)
		{
			pageBuffer->currentPaiduiUnitData->statues = statusWaitCardOut;
			pageBuffer->motorAction.motorActionEnum = PutCardOutOfDeviceDef;
			pageBuffer->motorAction.motorParm = pageBuffer->currentPaiduiUnitData->testLocation;
			StartMotorAction(&pageBuffer->motorAction, false, false);
		}
		else if(statusWaitCardOut == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(isMotorMoveEnd(0 / portTICK_RATE_MS))
			{
				if(!pageBuffer->isCancel)
					showThisTestResult();
				pageBuffer->currentPaiduiUnitData->statues = status_end;
				showStatus(QualityOverString, pageBuffer->currentPaiduiUnitData->index);
				pageBuffer->currentPaiduiUnitData = NULL;
			}
		}
	}
	else if(pageBuffer->isCancel)
		;
	else if(pageBuffer->testIndex < MaxQualityCount)
	{
		//新建测试
		pageBuffer->currentPaiduiUnitData = &pageBuffer->paiduiUnitData[pageBuffer->testIndex];
		pageBuffer->currentPaiduiUnitData->index = pageBuffer->testIndex;
		pageBuffer->currentPaiduiUnitData->cardLocation = 2*pageBuffer->testIndex + 1;
		pageBuffer->currentPaiduiUnitData->testLocation = pageBuffer->currentPaiduiUnitData->cardLocation;
		pageBuffer->currentPaiduiUnitData->testLocation += PaiDuiWeiNum;
		if(pageBuffer->currentPaiduiUnitData->testLocation > PaiDuiWeiNum*2)
			pageBuffer->currentPaiduiUnitData->testLocation -= PaiDuiWeiNum*2;
			
		pageBuffer->currentPaiduiUnitData->statues = statusNone;
		pageBuffer->cardScanResult = CardScanNone;
			
		showStatus(QualityStatusPreparingString, pageBuffer->currentPaiduiUnitData->index);
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
	SelectPage(158);
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
*Description: 界面内存释放
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:03:10
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(pageBuffer);
	pageBuffer = NULL;
}

static void showStatus(const char * str, unsigned char index)
{
	snprintf(pageBuffer->tempBuf, 50, "%s", str);
	DisText(0x3470 + index*0x10, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
}

static void showThisTestResult(void)
{
	if(pageBuffer->currentPaiduiUnitData->testData.testResultDesc == ResultIsOK)
	{
		pageBuffer->deviceQuality->testResult[pageBuffer->currentPaiduiUnitData->index] = Bool_True;
		
		pageBuffer->deviceQuality->testValue[pageBuffer->currentPaiduiUnitData->index] = pageBuffer->currentPaiduiUnitData->testData.testSeries.result;
		sprintf(pageBuffer->tempBuf, "%.3f", pageBuffer->deviceQuality->testValue[pageBuffer->currentPaiduiUnitData->index]);
		DisText(0x3410 + pageBuffer->currentPaiduiUnitData->index*0x05, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
		
		pageBuffer->resultSum += pageBuffer->deviceQuality->testValue[pageBuffer->currentPaiduiUnitData->index];
		pageBuffer->resultNum++;
		
		showStatus(TestOkString, pageBuffer->currentPaiduiUnitData->index);
		
		showAllPianCha();
	}
	else
	{
		pageBuffer->deviceQuality->testResult[pageBuffer->currentPaiduiUnitData->index] = Bool_False;
		pageBuffer->deviceQuality->testValue[pageBuffer->currentPaiduiUnitData->index] = 0;
		
		showStatus(TestErrorString, pageBuffer->currentPaiduiUnitData->index);
	}
}

static void showAllPianCha(void)
{
	if(pageBuffer->deviceQuality->standardValue != 0)
	{
		pageBuffer->resultSum = 0;
		pageBuffer->resultNum = 0;
		
		for(pageBuffer->i=0; pageBuffer->i<MaxQualityCount; pageBuffer->i++)
		{
			if(pageBuffer->deviceQuality->testResult[pageBuffer->i] == Bool_True)
			{
				pageBuffer->deviceQuality->testPicha[pageBuffer->i] = (pageBuffer->deviceQuality->testValue[pageBuffer->i] - pageBuffer->deviceQuality->standardValue) / pageBuffer->deviceQuality->standardValue;
				sprintf(pageBuffer->tempBuf, "%.3f%%", pageBuffer->deviceQuality->testPicha[pageBuffer->i]*100);
				DisText(0x3440 + pageBuffer->i*0x05, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
			}
			else if(pageBuffer->deviceQuality->testResult[pageBuffer->i] == Bool_False)
			{
				sprintf(pageBuffer->tempBuf, "---");
				DisText(0x3440 + pageBuffer->i*0x05, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
			}
			else
			{
				ClearText(0x3440 + pageBuffer->i*0x05);
			}
		}
		
		//更新平均偏差率
		if(pageBuffer->resultNum > 0)
		{
			//平均值
			pageBuffer->deviceQuality->avgValue = pageBuffer->resultSum / pageBuffer->resultNum;
			sprintf(pageBuffer->tempBuf, "%.3f", pageBuffer->deviceQuality->avgValue);
			DisText(0x3500, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
			
			//平均偏差
			pageBuffer->deviceQuality->avgPicha = (pageBuffer->deviceQuality->avgValue - pageBuffer->deviceQuality->standardValue) / pageBuffer->deviceQuality->standardValue;
			sprintf(pageBuffer->tempBuf, "%.3f%%", pageBuffer->deviceQuality->avgPicha*100);
			DisText(0x3505, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
		}
	}
}

static void clearPageText(void)
{
	for(pageBuffer->i=0; pageBuffer->i<MaxQualityCount; pageBuffer->i++)
	{
		ClearText(0x3410 + pageBuffer->i*0x05);
		ClearText(0x3440 + pageBuffer->i*0x05);
		ClearText(0x3470 + pageBuffer->i*0x10);
	}

	ClearText(0x3500);
	ClearText(0x3505);
	ClearText(0x350a);
	
	ClearText(0x3510);
	ClearText(0x3520);
	
	pageBuffer->tempBuf[0] = 0x00;
	pageBuffer->tempBuf[1] = 0x00;
	WriteRadioData(0x3402, pageBuffer->tempBuf, 2);
}


/****************************************end of file************************************************/
