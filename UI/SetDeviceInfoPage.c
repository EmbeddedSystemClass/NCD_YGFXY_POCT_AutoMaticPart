/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"SetDeviceInfoPage.h"

#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"System_Data.h"
#include	"DeviceDao.h"
#include	"CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static SetDeviceInfoPageBuffer * page = NULL;
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

static void showDeviceInfoText(void);
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
MyRes createSetDeviceInfoActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SetDeviceInfoActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		if(pram)
		{
			readIntent(pram, &page->device);
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
	showDeviceInfoText();
	
	updateSystemWorkStatus(SystemDeviceInfoModifying, ON);
	
	SelectPage(102);
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
	page->lcdinput[0] = pbuf[4];
	page->lcdinput[0] = (page->lcdinput[0]<<8) + pbuf[5];
		
	/*返回*/
	if(page->lcdinput[0] == 0x1B00)
	{
		backToFatherActivity();
	}
	/*确认*/
	else if(page->lcdinput[0] == 0x1B01)
	{			
		if(My_Pass == SaveDeviceToFile(page->device))
		{
			setSystemDeviceAddr(page->device->addr);
			setSystemDeviceIsNew(true);
			
			SendKeyCode(1);
		}
		else
			SendKeyCode(2);
	}
	/*姓名*/
	else if(page->lcdinput[0] == 0x1B10)
	{
		getLcdInputData(page->device->operator.name, &pbuf[7]);
	}
	/*年龄*/
	else if(page->lcdinput[0] == 0x1B20)
	{
		getLcdInputData(page->device->operator.age, &pbuf[7]);
	}
	/*性别*/
	else if(page->lcdinput[0] == 0x1B30)
	{
		getLcdInputData(page->device->operator.sex, &pbuf[7]);
	}
	/*联系方式*/
	else if(page->lcdinput[0] == 0x1B40)
	{
		getLcdInputData(page->device->operator.phone, &pbuf[7]);
	}
	/*职位*/
	else if(page->lcdinput[0] == 0x1B50)
	{
		getLcdInputData(page->device->operator.job, &pbuf[7]);
	}
	/*备注*/
	else if(page->lcdinput[0] == 0x1B60)
	{
		getLcdInputData(page->device->operator.department, &pbuf[7]);
	}
	/*设备使用地址*/
	else if(page->lcdinput[0] == 0x1B70)
	{
		getLcdInputData(page->device->addr, &pbuf[7]);
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
	SelectPage(102);
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
	updateSystemWorkStatus(SystemDeviceInfoModifying, OFF);
	UnLockObject(page->myDeviceLock, &page);
	
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
		page = MyMalloc(sizeof(SetDeviceInfoPageBuffer));
		
		if(page)
		{
			memset(page, 0, sizeof(SetDeviceInfoPageBuffer));
	
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

static void showDeviceInfoText(void)
{
	snprintf(page->tempBuf, 50, "%s", page->device->operator.name);
	DisText(0x1b10, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 5, "%s", page->device->operator.age);
	DisText(0x1b20, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 6, "%s", page->device->operator.sex);
	DisText(0x1b30, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 15, "%s", page->device->operator.phone);
	DisText(0x1b40, page->tempBuf, strlen(page->tempBuf)+1);
	
	snprintf(page->tempBuf, 20, "%s", page->device->operator.job);
	DisText(0x1b50, page->tempBuf, strlen(page->tempBuf)+1);

	snprintf(page->tempBuf, 50, "%s", page->device->operator.department);
	DisText(0x1b60, page->tempBuf, strlen(page->tempBuf)+1);
	
	snprintf(page->tempBuf, 100, "%s", page->device->addr);
	DisText(0x1b70, page->tempBuf, strlen(page->tempBuf)+1);
}

