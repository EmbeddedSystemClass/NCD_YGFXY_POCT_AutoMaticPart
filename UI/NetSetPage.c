/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"NetSetPage.h"
#include	"LCD_Driver.h"
#include	"SystemSet_Dao.h"
#include	"System_Data.h"
#include	"NetPreSetPage.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"SleepPage.h"
#include	"MyTools.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static NetSetPageBuffer *S_NetSetPageBuffer = NULL;
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

static void UpPageValue(void);
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
MyRes createNetSetActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "NetSetActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	updateSystemWorkStatus(SystemNetInfoModifying, ON);
	memcpy(&(S_NetSetPageBuffer->wireNetSet), &(getGBSystemSetData()->wireNetSet), sizeof(WireNetSet));
		
	UpPageValue();
	
	SelectPage(110);
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
	if(S_NetSetPageBuffer)
	{
		/*命令*/
		S_NetSetPageBuffer->lcdinput[0] = pbuf[4];
		S_NetSetPageBuffer->lcdinput[0] = (S_NetSetPageBuffer->lcdinput[0]<<8) + pbuf[5];

		/*有线网的ip获取模式*/
		if(S_NetSetPageBuffer->lcdinput[0] == 0x1E09)
		{
			/*数据*/
			S_NetSetPageBuffer->lcdinput[1] = pbuf[7];
			S_NetSetPageBuffer->lcdinput[1] = (S_NetSetPageBuffer->lcdinput[1]<<8) + pbuf[8];
			
			/*自动获取ip*/
			if(S_NetSetPageBuffer->lcdinput[1] == 0x8000)
				S_NetSetPageBuffer->wireNetSet.isStaticIp = false;
			/*使用设置的ip*/
			else if(S_NetSetPageBuffer->lcdinput[1] == 0x0000)
				S_NetSetPageBuffer->wireNetSet.isStaticIp = true;
		}
		/*设置IP*/
		else if(S_NetSetPageBuffer->lcdinput[0] == 0x1E10)
		{
			memset(S_NetSetPageBuffer->buf, 0, 50);
			memcpy(S_NetSetPageBuffer->buf, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			if(My_Pass != parseIpString(&S_NetSetPageBuffer->wireNetSet.staticIP, S_NetSetPageBuffer->buf))
			{
				memset(&S_NetSetPageBuffer->wireNetSet.staticIP, 0, 4);
				ClearText(0x1E10);
				SendKeyCode(3);
			}
		}
		//gateway
		else if(S_NetSetPageBuffer->lcdinput[0] == 0x1E20)
		{
			memset(S_NetSetPageBuffer->buf, 0, 50);
			memcpy(S_NetSetPageBuffer->buf, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			if(My_Pass != parseIpString(&S_NetSetPageBuffer->wireNetSet.NetMask, S_NetSetPageBuffer->buf))
			{
				memset(&S_NetSetPageBuffer->wireNetSet.NetMask, 0, 4);
				ClearText(0x1E10);
				SendKeyCode(3);
			}
		}
		//dns
		else if(S_NetSetPageBuffer->lcdinput[0] == 0x1E30)
		{
			memset(S_NetSetPageBuffer->buf, 0, 50);
			memcpy(S_NetSetPageBuffer->buf, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			if(My_Pass != parseIpString(&S_NetSetPageBuffer->wireNetSet.staticGateWay, S_NetSetPageBuffer->buf))
			{
				memset(&S_NetSetPageBuffer->wireNetSet.staticGateWay, 0, 4);
				ClearText(0x1E10);
				SendKeyCode(3);
			}
		}
		/*确认修改*/
		else if(S_NetSetPageBuffer->lcdinput[0] == 0x1E05)
		{
			readGbSystemSetData(&S_NetSetPageBuffer->systemSetData);
			
			memcpy(&(S_NetSetPageBuffer->systemSetData.wireNetSet), &(S_NetSetPageBuffer->wireNetSet), sizeof(WireNetSet));
				
			if(My_Pass == SaveSystemSetData(&(S_NetSetPageBuffer->systemSetData)))
				SendKeyCode(1);
			else
				SendKeyCode(2);
		}
		/*返回*/
		else if(S_NetSetPageBuffer->lcdinput[0] == 0x1E04)
		{
			backToFatherActivity();
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
	SelectPage(110);
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
	updateSystemWorkStatus(SystemNetInfoModifying, OFF);
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
	if(NULL == S_NetSetPageBuffer)
	{
		S_NetSetPageBuffer = MyMalloc(sizeof(NetSetPageBuffer));
		
		if(S_NetSetPageBuffer)
		{
			memset(S_NetSetPageBuffer, 0, sizeof(NetSetPageBuffer));
	
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
	MyFree(S_NetSetPageBuffer);
	S_NetSetPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void UpPageValue(void)
{
	memset(S_NetSetPageBuffer->buf, 0, 50);
		
	/*更新ip获取方式*/
	if(S_NetSetPageBuffer->wireNetSet.isStaticIp)
		S_NetSetPageBuffer->buf[0] = 0x00;	
	else
		S_NetSetPageBuffer->buf[0] = 0x80;

	WriteRadioData(0x1E09, S_NetSetPageBuffer->buf, 2);
			
	/*更新ip*/
	if(S_NetSetPageBuffer->wireNetSet.isStaticIp)
	{
		sprintf(S_NetSetPageBuffer->buf, "%03d.%03d.%03d.%03d", S_NetSetPageBuffer->wireNetSet.staticIP.ip_1, S_NetSetPageBuffer->wireNetSet.staticIP.ip_2, 
			S_NetSetPageBuffer->wireNetSet.staticIP.ip_3, S_NetSetPageBuffer->wireNetSet.staticIP.ip_4);
		DisText(0x1E10, S_NetSetPageBuffer->buf, 15);
	}
	else
		ClearText(0x1E10);
}


