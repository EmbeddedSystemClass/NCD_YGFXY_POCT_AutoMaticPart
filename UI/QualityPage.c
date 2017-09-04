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

#include	"Motor1_Fun.h"
#include	"Motor2_Fun.h"
#include	"Motor3_Fun.h"
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

static void showStatus(const char * str);
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
	pageBuffer->testCnt = 0;
	pageBuffer->testStep = 0;
	pageBuffer->resultSum = 0.0;
	pageBuffer->motorAction.motorActionName = WaitPutInCard;
	pageBuffer->motorAction.motorActionParm = 1;
	StartMotorAction(&pageBuffer->motorAction);
	pageBuffer->isStartted = false;
	
	timer_SetAndStart(&pageBuffer->cardTimer, 99999);
	
	clearPageText();
	
	showStatus("请等待\0");
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
		deleteGB_DeviceQuality();
		backToActivity(SystemSetActivityName);
	}
	//提交
	else if(pageBuffer->lcdinput[0] == 0x3401)
	{
		if(pageBuffer->testCnt > 0)
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
	else if(pageBuffer->lcdinput[0] == 0x3410)
	{
		pageBuffer->deviceQuality->standardValue = strtod((char *)&pbuf[7], NULL);
		showAllPianCha();
	}
	//结果
	else if(pageBuffer->lcdinput[0] == 0x3402)
	{
		pageBuffer->lcdinput[1] = pbuf[7];
		pageBuffer->lcdinput[1] = (pageBuffer->lcdinput[1]<<8) + pbuf[8];
			
		if(pageBuffer->lcdinput[1] == 0x8000)
			pageBuffer->deviceQuality->isOk = true;
		else if(pageBuffer->lcdinput[1] == 0x0000)
			pageBuffer->deviceQuality->isOk = false;
	}
	//维护说明
	else if(pageBuffer->lcdinput[0] == 0x3420)
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
	if(pageBuffer->testStep == 0 && isMotorActionOver())
	{
		if(pageBuffer->testCnt >= DeviceQualityMaxTestCount)
			showStatus("测试结束\0");
		else
			showStatus("请插卡\0");
		pageBuffer->testStep = 1;
	}
	
	if(pageBuffer->testStep == 1 && readCaedCheckStatus() == ON)
	{
		StartScanQRCode(&(pageBuffer->testData.qrCode));
		pageBuffer->testStep = 2;
		showStatus("读取二维码\0");
	}
	
	if(pageBuffer->testStep == 2 && My_Pass == TakeScanQRCodeResult(&pageBuffer->cardScanResult))
	{
		if(pageBuffer->cardScanResult == CardCodeScanOK)
		{
			pageBuffer->testStep = 3;
			pageBuffer->motorAction.motorActionName = MoveToStartTestLocation;
			pageBuffer->motorAction.motorActionParm = 5;
			StartMotorAction(&pageBuffer->motorAction);
			showStatus("开始测试\0");
		}
		else
		{
			showStatus("错误，请更换");
			pageBuffer->testStep = 7;
		}
	}
	
	if(pageBuffer->testStep == 3 && isMotorActionOver())
	{
		pageBuffer->testStep = 4;
		StartTest(&pageBuffer->testData);
	}
	
	if(pageBuffer->testStep == 4 && My_Pass == TakeTestResult(&pageBuffer->testData.testResultDesc))
	{
		pageBuffer->motorAction.motorActionName = OutOutCard;
		StartMotorAction(&pageBuffer->motorAction);
		pageBuffer->testStep = 5;
		
		if(pageBuffer->testData.testResultDesc == ResultIsOK)
		{
			pageBuffer->testCnt++;
			
			//保存测试结果
			pageBuffer->deviceQuality->testValue[pageBuffer->testCnt-1] = pageBuffer->testData.testSeries.BasicResult;
			showThisTestResult();

			showStatus("当前测试完成\0");
		}
		else
		{
			showStatus("错误，请更换");
		}
	}
	
	if(pageBuffer->testStep == 5 && isMotorActionOver())
	{
		pageBuffer->testStep = 8;
	}
	
	if(pageBuffer->testStep == 7 && OFF == readCaedCheckStatus())
		pageBuffer->testStep = 8;
	
	if(pageBuffer->testStep == 8)
	{
		pageBuffer->motorAction.motorActionName = WaitPutInCard;
		pageBuffer->motorAction.motorActionParm = 1;
		StartMotorAction(&pageBuffer->motorAction);
		pageBuffer->testStep = 0;
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

static void showStatus(const char * str)
{
	snprintf(pageBuffer->tempBuf, 50, "%s", str);
	DisText(0x34a0, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
}

static void showThisTestResult(void)
{
	pageBuffer->tempValue1 = pageBuffer->deviceQuality->testValue[pageBuffer->testCnt-1];
	//显示当前测试结果
	snprintf(pageBuffer->tempBuf, 15, "%.*f", pageBuffer->testData.qrCode.itemConstData.pointNum, pageBuffer->tempValue1);
	DisText(0x3430+((pageBuffer->testCnt-1)*0x05), pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
	
	pageBuffer->resultSum += pageBuffer->tempValue1;
	
	showAllPianCha();
}

static void showAllPianCha(void)
{
	unsigned char i=0;
	if(pageBuffer->deviceQuality->standardValue != 0)
	{
		for(i=0; i<pageBuffer->testCnt; i++)
		{
			pageBuffer->tempValue1 = 
				(pageBuffer->deviceQuality->testValue[i] - pageBuffer->deviceQuality->standardValue) 
				/ pageBuffer->deviceQuality->standardValue;
			
			pageBuffer->tempValue1 *= 100;
			
			snprintf(pageBuffer->tempBuf, 15, "%.2f%%", pageBuffer->tempValue1);
			DisText(0x3465+(i*0x05), pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
		}
		
		//更新平均偏差率
		if(pageBuffer->testCnt > 0)
		{
			pageBuffer->tempValue2 = pageBuffer->resultSum / pageBuffer->testCnt;
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
