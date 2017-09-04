/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"OtherSetPage.h"
#include	"SystemSetPage.h"
#include	"SleepPage.h"

#include	"SystemSet_Dao.h"
#include	"LCD_Driver.h"
#include	"RX8025_Driver.h"
#include	"Define.h"
#include	"MyMem.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static OtherSetPageBuffer *S_OtherSetPageBuffer = NULL;
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

static void SetGB_Time(char *buf, unsigned char len);
static void showPrintfIco(void);
static void showLcdLightNum(void);
static void showLcdSleepTime(void);
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
MyRes createOtherSetActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "OtherSetActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	showPrintfIco();
	showLcdLightNum();
	showLcdSleepTime();

	SelectPage(122);
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
	if(S_OtherSetPageBuffer)
	{
		/*命令*/
		S_OtherSetPageBuffer->lcdinput[0] = pbuf[4];
		S_OtherSetPageBuffer->lcdinput[0] = (S_OtherSetPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*重启*/
		if(S_OtherSetPageBuffer->lcdinput[0] == 0x2400)
		{
			while(1);
		}
		/*返回*/
		else if(S_OtherSetPageBuffer->lcdinput[0] == 0x2401)
		{
			backToFatherActivity();
		}
		/*设置时间*/
		else if(S_OtherSetPageBuffer->lcdinput[0] == 0x2410)
		{
			SetGB_Time((char *)(&pbuf[7]), GetBufLen(&pbuf[7] , 2*pbuf[6]));
		}
		//自动打印设置
		else if(S_OtherSetPageBuffer->lcdinput[0] == 0x2402)
		{
			//如果当前是自动打印，则禁止，否则打开
			memcpy(&(S_OtherSetPageBuffer->tempSystemSetData), getGBSystemSetData(), SystemSetDataStructSize);
			if(S_OtherSetPageBuffer->tempSystemSetData.isAutoPrint)
				S_OtherSetPageBuffer->tempSystemSetData.isAutoPrint = false;
			else
				S_OtherSetPageBuffer->tempSystemSetData.isAutoPrint = true;
			
			if(My_Pass == SaveSystemSetData(&(S_OtherSetPageBuffer->tempSystemSetData)))
			{
				SendKeyCode(1);
			}
			else
			{
				SendKeyCode(2);
			}
			
			showPrintfIco();
		}
		//静音
		else if(S_OtherSetPageBuffer->lcdinput[0] == 0x2403)
		{

		}
		//进入休眠时间
		else if((S_OtherSetPageBuffer->lcdinput[0] >= 0x2407) && (S_OtherSetPageBuffer->lcdinput[0] <= 0x240a))
		{
			S_OtherSetPageBuffer->tempValue = S_OtherSetPageBuffer->lcdinput[0] - 0x2407;
			
			memcpy(&(S_OtherSetPageBuffer->tempSystemSetData), getGBSystemSetData(), SystemSetDataStructSize);
			if(S_OtherSetPageBuffer->tempValue == 0)
				S_OtherSetPageBuffer->tempSystemSetData.ledSleepTime = 60;
			else if(S_OtherSetPageBuffer->tempValue == 1)
				S_OtherSetPageBuffer->tempSystemSetData.ledSleepTime = 180;
			else if(S_OtherSetPageBuffer->tempValue == 2)
				S_OtherSetPageBuffer->tempSystemSetData.ledSleepTime = 300;
			else
				S_OtherSetPageBuffer->tempSystemSetData.ledSleepTime = 60000;
			
			if(My_Pass == SaveSystemSetData(&(S_OtherSetPageBuffer->tempSystemSetData)))
			{
				SendKeyCode(1);
			}
			else
			{
				SendKeyCode(2);
			}
			
			showLcdSleepTime();
		}	
		//进入屏幕亮度
		else if(S_OtherSetPageBuffer->lcdinput[0] == 0x240b)
		{	
			memcpy(&(S_OtherSetPageBuffer->tempSystemSetData), getGBSystemSetData(), SystemSetDataStructSize);
			S_OtherSetPageBuffer->tempSystemSetData.ledLightIntensity = pbuf[8];
	
			if(My_Pass == SaveSystemSetData(&(S_OtherSetPageBuffer->tempSystemSetData)))
			{
				SetLEDLight(S_OtherSetPageBuffer->tempSystemSetData.ledLightIntensity);
			}
			else
			{
				SendKeyCode(2);
				
				showLcdLightNum();
			}
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
	SelectPage(122);
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
	if(NULL == S_OtherSetPageBuffer)
	{
		S_OtherSetPageBuffer = MyMalloc(sizeof(OtherSetPageBuffer));
		
		if(S_OtherSetPageBuffer)
		{
			memset(S_OtherSetPageBuffer, 0, sizeof(OtherSetPageBuffer));
	
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
	MyFree(S_OtherSetPageBuffer);
	S_OtherSetPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

static void SetGB_Time(char *buf, unsigned char len)
{
	if(len < 14)
		goto END;

	snprintf(S_OtherSetPageBuffer->buf, 5, "%s", buf);
	S_OtherSetPageBuffer->tempValue = strtol(S_OtherSetPageBuffer->buf, NULL, 10);
	if(S_OtherSetPageBuffer->tempValue < 2016 || S_OtherSetPageBuffer->tempValue > 2099)
		goto END;
	S_OtherSetPageBuffer->temptime.year = S_OtherSetPageBuffer->tempValue - 2000;
	
	snprintf(S_OtherSetPageBuffer->buf, 3, "%s", buf+4);
	S_OtherSetPageBuffer->tempValue = strtol(S_OtherSetPageBuffer->buf, NULL, 10);
	if(S_OtherSetPageBuffer->tempValue < 1 || S_OtherSetPageBuffer->tempValue > 12)
		goto END;
	S_OtherSetPageBuffer->temptime.month = S_OtherSetPageBuffer->tempValue;
	
	snprintf(S_OtherSetPageBuffer->buf, 3, "%s", buf+6);
	S_OtherSetPageBuffer->tempValue = strtol(S_OtherSetPageBuffer->buf, NULL, 10);
	if(S_OtherSetPageBuffer->tempValue < 1 || S_OtherSetPageBuffer->tempValue > 31)
		goto END;
	S_OtherSetPageBuffer->temptime.day = S_OtherSetPageBuffer->tempValue;
	
	snprintf(S_OtherSetPageBuffer->buf, 3, "%s", buf+8);
	S_OtherSetPageBuffer->tempValue = strtol(S_OtherSetPageBuffer->buf, NULL, 10);
	if(S_OtherSetPageBuffer->tempValue > 23)
		goto END;
	S_OtherSetPageBuffer->temptime.hour = S_OtherSetPageBuffer->tempValue;
	
	snprintf(S_OtherSetPageBuffer->buf, 3, "%s", buf+10);
	S_OtherSetPageBuffer->tempValue = strtol(S_OtherSetPageBuffer->buf, NULL, 10);
	if(S_OtherSetPageBuffer->tempValue > 59)
		goto END;
	S_OtherSetPageBuffer->temptime.min = S_OtherSetPageBuffer->tempValue;
	
	snprintf(S_OtherSetPageBuffer->buf, 3, "%s", buf+12);
	S_OtherSetPageBuffer->tempValue = strtol(S_OtherSetPageBuffer->buf, NULL, 10);
	if(S_OtherSetPageBuffer->tempValue > 59)
		goto END;
	S_OtherSetPageBuffer->temptime.sec = S_OtherSetPageBuffer->tempValue;
	
	if(My_Pass == RTC_SetTimeData(&(S_OtherSetPageBuffer->temptime)))
		SendKeyCode(1);
	else
		SendKeyCode(2);
			
	END:
		SendKeyCode(3);
}

static void showPrintfIco(void)
{
	S_OtherSetPageBuffer->ico.ICO_ID = 23;
	S_OtherSetPageBuffer->ico.X = 176;
	S_OtherSetPageBuffer->ico.Y = 185;
			
	if(getGBSystemSetData()->isAutoPrint)
		BasicUI(0x2440 ,0x1807 , 1, &(S_OtherSetPageBuffer->ico) , sizeof(Basic_ICO));
	else
		BasicUI(0x2440 ,0x1807 , 0, &(S_OtherSetPageBuffer->ico) , sizeof(Basic_ICO));
}

static void showLcdLightNum(void)
{
	DspNum(0x240b, getGBSystemSetData()->ledLightIntensity, 2);
}

static void showLcdSleepTime(void)
{
	S_OtherSetPageBuffer->ico.ICO_ID = 23;
	S_OtherSetPageBuffer->ico.X = 385;
	S_OtherSetPageBuffer->ico.Y = 312;

	S_OtherSetPageBuffer->tempValue = getGBSystemSetData()->ledSleepTime;
	if(S_OtherSetPageBuffer->tempValue == 60)
		BasicUI(0x2420 ,0x1807 , 1, &(S_OtherSetPageBuffer->ico) , sizeof(Basic_ICO));
	else if(S_OtherSetPageBuffer->tempValue == 180)
	{
		S_OtherSetPageBuffer->ico.X = 490;
		BasicUI(0x2420 ,0x1807 , 1, &(S_OtherSetPageBuffer->ico) , sizeof(Basic_ICO));
	}
	else if(S_OtherSetPageBuffer->tempValue == 300)
	{
		S_OtherSetPageBuffer->ico.X = 595;
		BasicUI(0x2420 ,0x1807 , 1, &(S_OtherSetPageBuffer->ico) , sizeof(Basic_ICO));
	}
	else if(S_OtherSetPageBuffer->tempValue == 60000)
	{
		S_OtherSetPageBuffer->ico.X = 700;
		BasicUI(0x2420 ,0x1807 , 1, &(S_OtherSetPageBuffer->ico) , sizeof(Basic_ICO));
	}
	else
		BasicUI(0x2420 ,0x1807 , 0, &(S_OtherSetPageBuffer->ico) , sizeof(Basic_ICO));
}

