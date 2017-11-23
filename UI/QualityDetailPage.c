/***************************************************************************************************
*FileName: MaintenancePage
*Description: 设备维护
*Author:xsx
*Data: 2017年6月14日 15:51:52
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"QualityDetailPage.h"

#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"MyTools.h"
#include	"StringDefine.h"
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
/**************************************变量声明*************************************************/
/***************************************************************************************************/
static QualityDetailPageBuffer * pageBuffer = NULL;
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
static void showPageValue(void);
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
MyRes createQualityDetailActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, QualityDetailRecordActivityName, activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		if(pram != NULL)
			readIntent(pram, &pageBuffer->deviceQuality);
		
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
	showPageValue();
	
	SelectPage(159);
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
		backToFatherActivity();
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
	SelectPage(159);
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
		pageBuffer = MyMalloc(QualityDetailPageBufferStructSize);
		if(pageBuffer)	
		{
			memset(pageBuffer, 0, QualityDetailPageBufferStructSize);
	
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

static void showPageValue(void)
{
	//操作人
	sprintf(pageBuffer->tempBuf, "%s", pageBuffer->deviceQuality.operator.name);
	DisText(0x3540, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
	
	//时间
	sprintf(pageBuffer->tempBuf, "20%02d-%02d-%02d %02d:%02d:%02d", pageBuffer->deviceQuality.dateTime.year, pageBuffer->deviceQuality.dateTime.month, 
		pageBuffer->deviceQuality.dateTime.day, pageBuffer->deviceQuality.dateTime.hour, pageBuffer->deviceQuality.dateTime.min, pageBuffer->deviceQuality.dateTime.sec);
	DisText(0x3530, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
	
	//项目
	sprintf(pageBuffer->tempBuf, "%s", pageBuffer->deviceQuality.itemName);
	DisText(0x3510, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
	
	//显示标准值
	sprintf(pageBuffer->tempBuf, "%.3f", pageBuffer->deviceQuality.standardValue);
	DisText(0x350a, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
	
	//平均值
	sprintf(pageBuffer->tempBuf, "%.3f", pageBuffer->deviceQuality.avgValue);
	DisText(0x3500, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
			
	//平均偏差
	sprintf(pageBuffer->tempBuf, "%.3f%%", pageBuffer->deviceQuality.avgPicha*100);
	DisText(0x3505, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
	
	//显示每个测试值和对应偏差
	for(pageBuffer->i=0; pageBuffer->i<MaxQualityCount; pageBuffer->i++)
	{
		if(pageBuffer->deviceQuality.testResult[pageBuffer->i] == Bool_True)
		{
			sprintf(pageBuffer->tempBuf, "%.3f", pageBuffer->deviceQuality.testValue[pageBuffer->i]);
			DisText(0x3410 + pageBuffer->i*0x05, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
			
			sprintf(pageBuffer->tempBuf, "%.3f%%", pageBuffer->deviceQuality.testPicha[pageBuffer->i]*100);
			DisText(0x3440 + pageBuffer->i*0x05, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
		}
		else if(pageBuffer->deviceQuality.testResult[pageBuffer->i] == Bool_False)
		{
			sprintf(pageBuffer->tempBuf, "Error");
			DisText(0x3410 + pageBuffer->i*0x05, pageBuffer->tempBuf, 6);
			
			sprintf(pageBuffer->tempBuf, "---");
			DisText(0x3440 + pageBuffer->i*0x05, pageBuffer->tempBuf, 4);
		}
		else
		{
			ClearText(0x3410 + pageBuffer->i*0x05);
			ClearText(0x3440 + pageBuffer->i*0x05);
		}
	}
	
	//显示结果
	pageBuffer->tempBuf[0] = 0x00;
	pageBuffer->tempBuf[1] = 0x00;
	if(pageBuffer->deviceQuality.isOk)
	{
		pageBuffer->tempBuf[1] = 0x01;
	}
	WriteRadioData(0x3402, pageBuffer->tempBuf, 2);
	
	snprintf(pageBuffer->tempBuf, 40, "%s", pageBuffer->deviceQuality.desc);
	DisText(0x3520, pageBuffer->tempBuf, strlen(pageBuffer->tempBuf)+1);
}



/****************************************end of file************************************************/
