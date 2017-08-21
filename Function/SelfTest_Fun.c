/***************************************************************************************************
*FileName：SelfTest_Fun
*Description：自检功能
*Author：xsx
*Data：2016年1月27日10:29:14
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"SelfTest_Fun.h"

#include	"SystemSet_Data.h"
#include	"DeviceDao.h"
#include	"LedCheck_Driver.h"
#include	"DA_Driver.h"
#include	"SystemSet_Dao.h"
#include	"Motor1_Driver.h"
#include	"Motor2_Driver.h"
#include	"Motor4_Driver.h"
#include	"Motor1_Fun.h"
#include	"Motor2_Fun.h"
#include	"Motor4_Fun.h"

#include	"MyMem.h"
#include	"CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	"stdio.h"
#include	"stdlib.h"
#include	<string.h>
#include	<math.h>
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
static ERROR_SelfTest SelfTestStatus = SelfTestting;							//保存每个自检状态
static FATFS S_Fatfs;				/* File system object */
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static void setSelfTestStatus(ERROR_SelfTest status);
static MyRes loadSystemData(void);
static MyRes testLed(void);
static MyRes testMotol(void);
static void deviceAdjustSelf(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void setSelfTestStatus(ERROR_SelfTest status)
{
	SelfTestStatus = status;
}

ERROR_SelfTest getSelfTestStatus(void)
{
	return SelfTestStatus;
}

/***************************************************************************************************
*FunctionName：SelfTest_Function
*Description：自检功能实现
*Input：None
*Output：None
*Author：xsx
*Data：2016年1月27日10:38:06
***************************************************************************************************/
void SelfTest_Function(void)
{
	setSelfTestStatus(SelfTestting);
	
	//SD卡初始化
	if(FR_OK != f_mount(&S_Fatfs, "0:", 1))
	{
		setSelfTestStatus(SD_ERROR);
		return;
	}
	
	//加载系统数据，并发生加载结果,如果加载失败，则终止自检程序
	if(My_Pass != loadSystemData())
	{
		setSelfTestStatus(SystemData_ERROR);
		return;
	}
	
	//检测led
	if(My_Pass != testLed())
	{
		setSelfTestStatus(Light_Error);
		return;
	}

	//测试传动模块
	if(My_Pass != testMotol())
	{
		setSelfTestStatus(Motol_ERROR);
		return;
	}
	
	//设备校准
//	deviceAdjustSelf();
	
	//自检完成，发送结果
	setSelfTestStatus(SelfTest_OK);
}

/***************************************************************************************************
*FunctionName: loadSystemData
*Description: 加载系统数据
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月23日09:06:16
***************************************************************************************************/
static MyRes loadSystemData(void)
{
	SystemSetData * systemSetData = NULL;
	MyRes status = My_Fail;
	
	systemSetData = MyMalloc(SystemSetDataStructSize);

	if(systemSetData)
	{
		memset(systemSetData, 0, SystemSetDataStructSize);
		//读取SD卡中的配置文件
		ReadSystemSetData(systemSetData);
		
		//如果crc错误表示读取失败，或者设备第一次开机，不存在配置文件，则恢复出厂设置
		if(systemSetData->crc != CalModbusCRC16Fun(systemSetData, SystemSetDataStructCrcSize, NULL))
			setDefaultSystemSetData(systemSetData);								//恢复出厂设置
		else
			upDateSystemSetData(systemSetData);									//将读取的配置更新到内存中

		//无论是否成功读取到配置文件，都保存SD卡一次，用以测试SD卡是否正常
		if(My_Pass == SaveSystemSetData(systemSetData))
		{
			status = My_Pass;
		}
	}
	
	MyFree(systemSetData);
	
	return status;
}

/***************************************************************************************************
*FunctionName: testLed
*Description: 测试led是否发光
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月23日15:39:25
***************************************************************************************************/
static MyRes testLed(void)
{
/*	SetLedVol(0);
	vTaskDelay(100 / portTICK_RATE_MS);
	if(ON == readLedCheckStatus())
		return My_Fail;
	
	SetLedVol(300);
	vTaskDelay(100 / portTICK_RATE_MS);
	if(OFF == readLedCheckStatus())
	{
		SetLedVol(0);
		return My_Fail;
	}

	SetLedVol(0);*/
	return My_Pass;
}

/***************************************************************************************************
*FunctionName: testMotol
*Description: 测试传动模块
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月23日16:21:14
***************************************************************************************************/
static MyRes testMotol(void)
{
	//测试电机1
	motor1MoveToNum(1, 20000);
	
	if(getMotorxLocation(Motor_1) != 1)
		return My_Fail;
	
	//检测电机2
	motor2MoveTo(0, 15000);
	if(!Motor2Sensor1Triggered)
		return My_Fail;
	
	motor2MoveTo(60000, 20000);
	if(!Motor2Sensor3Triggered)
		return My_Fail;
	
	motor2MoveTo(Motor2_MidLocation, 20000);
	
	//检测电机4
	motor4MoveTo(0, 5000);
	if(!Motor4Sensor1Triggered)
		return My_Fail;
	
	motor4MoveTo(Motor4_OpenLocation, 5000);
	vTaskDelay(3000 / portTICK_RATE_MS);

	return My_Pass;
}

static void deviceAdjustSelf(void)
{

}
