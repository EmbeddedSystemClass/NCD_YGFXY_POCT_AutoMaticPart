/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"SelectUserPage.h"
#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"MyTest_Data.h"
#include	"CRC16.h"
#include	"UserMPage.h"
#include	"DeviceDao.h"
#include	"Intent.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static UserPageBuffer * page = NULL;

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

static void ShowList(void);
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
MyRes createSelectUserActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SelectUserActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		if(pram)
		{
			readIntent(pram, &page->targetOperator);
		}
		
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
	/*读取设备信息*/
	ReadDeviceFromFile(&page->device);
	
	page->pageindex = 0;
	
	ShowList();
	
	SelectPage(84);
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
	if(page)
	{
		/*命令*/
		page->lcdinput[0] = pbuf[4];
		page->lcdinput[0] = (page->lcdinput[0]<<8) + pbuf[5];
		
		/*返回*/
		if(page->lcdinput[0] == 0x1200)
		{
			page->currenttestdata = GetCurrentTestItem();
			if(NULL != page->currenttestdata)
			{
				DeleteCurrentTest();
			}

			backToFatherActivity();
		}
		
		/*上翻也*/
		else if(page->lcdinput[0] == 0x1203)
		{			
			if(page->pageindex > 0)
			{
				page->pageindex--;
						
				ShowList();
			}
		}
		/*下翻页*/
		else if(page->lcdinput[0] == 0x1204)
		{			
			if((page->pageindex + 1) < MaxPageNum)
			{
				page->tempOperator = &page->device.operators[(page->pageindex + 1)*MaxPageShowOperatorSize];
			
				if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
				{
					page->pageindex++;
						
					ShowList();
				}
			}
		}
		/*确定*/
		else if(page->lcdinput[0] == 0x1201)
		{
			if(page->targetOperator->crc == CalModbusCRC16Fun(page->targetOperator, OneOperatorStructCrcSize, NULL))
				gotoChildActivity(NULL, NULL);
			else
				SendKeyCode(1);
		}
		/*选择操作人*/
		else if((page->lcdinput[0] >= 0x1205)&&(page->lcdinput[0] <= 0x1209))
		{			
			page->tempOperator = &page->device.operators[(page->pageindex)*MaxPageShowOperatorSize + page->lcdinput[0] - 0x1205];
		
			if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
			{
				page->tempV1 = page->lcdinput[0] - 0x1205+1;
				BasicPic(0x1240, 0, 137, 11, 10, 303, 79, 363, 141+(page->tempV1-1)*72);
				BasicPic(0x1240, 1, 137, 11, 10, 303, 79, 363, 141+(page->tempV1-1)*72);
				
				memcpy(page->targetOperator, page->tempOperator, OneOperatorStructSize);
			}
		}
		//编辑操作人
		if(page->lcdinput[0] == 0x120a)
		{
			startActivity(createUserManagerActivity, NULL, NULL);
		}
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
	/*读取所有操作人*/
	ReadDeviceFromFile(&page->device);
	
	page->pageindex = 0;
	
	ShowList();
	
	SelectPage(84);
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
	if(NULL == page)
	{
		page = MyMalloc(sizeof(UserPageBuffer));
		
		if(page)	
		{
			memset(page, 0, sizeof(UserPageBuffer));
	
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
	MyFree(page);
	page = NULL;
}



/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName：ShowList
*Description：显示列表内容
*Input：None
*Output：None
*Author：xsx
*Data：2016年6月29日08:44:00
***************************************************************************************************/
static void ShowList(void)
{
	page->tempV1 = page->pageindex*MaxPageShowOperatorSize;

	page->tempOperator = &(page->device.operators[page->tempV1]);
	
	//显示列表数据
	for(page->tempV1=0; page->tempV1<MaxPageShowOperatorSize; page->tempV1++)
	{
		if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
			snprintf(page->tempBuf, OperatorNameLen, "%s", page->tempOperator->name);
		else
			memset(page->tempBuf, 0, 10);
		
		DisText(0x1210+8*page->tempV1, page->tempBuf, strlen(page->tempBuf)+1);
		
		page->tempOperator++;
	}
	
	BasicPic(0x1240, 0, 137, 11, 10, 303, 79, 363, 141+(page->tempV1-1)*72);
}

