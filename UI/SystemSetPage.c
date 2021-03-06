/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"SystemSetPage.h"
#include	"Define.h"
#include	"StringDefine.h"
#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"MyTools.h"
#include	"UserMPage.h"
#include	"AboutUsPage.h"
#include	"RecordPage.h"
#include	"OtherSetPage.h"
#include	"NetPreSetPage.h"
#include	"ShowDeviceInfoPage.h"
#include	"ReTestPage.h"
#include	"SelectUserPage.h"
#include	"MaintenancePage.h"
#include	"MaintenanceRecordPage.h"
#include	"AdjustRecordPage.h"
#include	"AdjustLedPage.h"
#include	"ErrorRecordPage.h"
#include	"QualityPage.h"
#include	"QualityRecordPage.h"
#include	"ChannelPage.h"
#include	"Maintenance_Data.h"
#include	"Universal_Fun.h"
#include	"Quality_Data.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static SysSetPageBuffer * S_SysSetPageBuffer = NULL;
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
MyRes createSystemSetActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, SystemSetActivityName, activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	updateSystemWorkStatus(SystemDeviceInfoModifying | SystemOperatorModifying | SystemNetInfoModifying | SystemLookRecord | SystemQualityong
		| SystemMaintenancing | SystemSetting, OFF);
	SelectPage(98);
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
	S_SysSetPageBuffer->lcdinput[0] = pbuf[4];
	S_SysSetPageBuffer->lcdinput[0] = (S_SysSetPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
	//基本信息
	if(S_SysSetPageBuffer->lcdinput[0] == 0x1900)
	{
		updateSystemWorkStatus(SystemDeviceInfoModifying, ON);
		startActivity(createDeviceInfoActivity, NULL, NULL);
	}
	//操作人管理
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x1901)
	{
		updateSystemWorkStatus(SystemOperatorModifying, ON);
		startActivity(createUserManagerActivity, NULL, NULL);
	}
	//网络设置
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x1902)
	{
		updateSystemWorkStatus(SystemNetInfoModifying, ON);
		startActivity(createNetPreActivity, NULL, NULL);
	}
	//数据管理
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x1903)
	{
		/*数据*/
		S_SysSetPageBuffer->lcdinput[1] = pbuf[7];
		S_SysSetPageBuffer->lcdinput[1] = (S_SysSetPageBuffer->lcdinput[1]<<8) + pbuf[8];

		updateSystemWorkStatus(SystemLookRecord, ON);
		
		if(S_SysSetPageBuffer->lcdinput[1] == 0x0001)
			startActivity(createRecordActivity, NULL, NULL);
		else if(S_SysSetPageBuffer->lcdinput[1] == 0x0002)
			startActivity(createErrorRecordActivity, NULL, NULL);
		else if(S_SysSetPageBuffer->lcdinput[1] == 0x0003)
			startActivity(createAdjustRecordActivity, NULL, NULL);
		else if(S_SysSetPageBuffer->lcdinput[1] == 0x0004)
			startActivity(createQualityRecordActivity, NULL, NULL);
		else if(S_SysSetPageBuffer->lcdinput[1] == 0x0005)
			startActivity(createMaintenanceRecordActivity, NULL, NULL);
		else
			updateSystemWorkStatus(SystemLookRecord, OFF);
	}
	//关于按键第一次按下
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x1909)
	{
		S_SysSetPageBuffer->pressCnt = 0;
	}
	//关于按键持续按下
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x190A)
	{
		S_SysSetPageBuffer->pressCnt++;
	}
	//关于按键松开
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x190B)
	{
		//如果是长按就输入密码进入隐藏功能
		if(S_SysSetPageBuffer->pressCnt > 10)
			SendKeyCode(4);
		//短按则进入关于界面
		else
			startActivity(createAboutUsActivity, NULL, NULL);
	}
	//隐藏密码的厂家功能
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x1910)
	{
		if(GetBufLen(&pbuf[7] , 2*pbuf[6]) == 6)
		{
			if(pdPASS == CheckStrIsSame(&pbuf[7], TestPassWord, 6))
				startActivity(createReTestActivity, NULL, NULL);

			else if(pdPASS == CheckStrIsSame(&pbuf[7], AdjLedPassWord, 6))
				startActivity(createAdjustLedActivity, NULL, NULL);

			else if(pdPASS == CheckStrIsSame(&pbuf[7], FactoryResetPassWord, 6))
			{
				if(My_Pass == SystemFactoryReset())
					SendKeyCode(2);
				else
					SendKeyCode(1);
			}
			else if(pdPASS == CheckStrIsSame(&pbuf[7], UnlockLCDPassWord, 6))
			{
				unLockLCDOneTime();
				SendKeyCode(2);
			}
			else if(pdPASS == CheckStrIsSame(&pbuf[7], ChangeValueShowTypePassWord, 6))
			{
				setIsShowRealValue(true);
				SendKeyCode(2);
			}
			else if(pdPASS == CheckStrIsSame(&pbuf[7], ChannelTestPassWord, 6))
				startActivity(createChannelTestActivity, NULL, NULL);

			else
				SendKeyCode(1);
		}
		else
			SendKeyCode(1);
	}
	//质控
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x1945)
	{
		//测试中，质控功能禁止
		if(IsPaiDuiTestting())
			SendKeyCode(5);
		else if(My_Pass == CreateADeviceQuality())
		{
			S_SysSetPageBuffer->operator = getDeviceQualityOperator();
			updateSystemWorkStatus(SystemQualityong, ON);
			startActivity(createSelectUserActivity, createIntent(&(S_SysSetPageBuffer->operator), 4), createQualityActivity);
		}
		else
			SendKeyCode(3);
	}
	//维护
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x1946)
	{
		//测试中，质控功能禁止
		if(IsPaiDuiTestting())
			SendKeyCode(5);
		else if(My_Pass == CreateADeviceMaintenance())
		{
			S_SysSetPageBuffer->operator = &(getGB_DeviceMaintenance()->operator);
			updateSystemWorkStatus(SystemMaintenancing, ON);
			startActivity(createSelectUserActivity, createIntent(&(S_SysSetPageBuffer->operator), 4), createMaintenanceActivity);
		}
		else
			SendKeyCode(3);
	}
	//其他设置
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x1904)
	{
		updateSystemWorkStatus(SystemSetting, ON);
		startActivity(createOtherSetActivity, NULL, NULL);
	}

	//返回
	else if(S_SysSetPageBuffer->lcdinput[0] == 0x1906)
		backToFatherActivity();
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
	updateSystemWorkStatus(SystemDeviceInfoModifying | SystemOperatorModifying | SystemNetInfoModifying | SystemLookRecord | SystemQualityong
		| SystemMaintenancing | SystemSetting, OFF);
	SelectPage(98);
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
	if(NULL == S_SysSetPageBuffer)
	{
		S_SysSetPageBuffer = MyMalloc(sizeof(SysSetPageBuffer));
		
		if(S_SysSetPageBuffer)
		{
			memset(S_SysSetPageBuffer, 0, sizeof(SysSetPageBuffer));
	
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
	MyFree(S_SysSetPageBuffer);
	S_SysSetPageBuffer = NULL;
}



