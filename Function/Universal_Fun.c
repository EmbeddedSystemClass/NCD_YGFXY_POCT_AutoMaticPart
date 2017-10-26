/***************************************************************************************************
*FileName:SDFunction
*Description:一些SD卡的操作函数
*Author:xsx
*Data:2016年4月30日16:06:36
***************************************************************************************************/


/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"Universal_Fun.h"

#include	"RX8025_Driver.h"
#include	"Ceju_Driver.h"

#include	"SystemSet_Dao.h"
#include	"TestDataDao.h"
#include	"DeviceDao.h"
#include	"DeviceMaintenanceDao.h"
#include	"DeviceAdjustDao.h"
#include	"DeviceErrorDao.h"
#include	"DeviceQualityDao.h"
#include	"System_Data.h"
#include	"DateTime.h"
#include	"MyMem.h"
#include	"MyTools.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/





/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void readAndUpdateSystemDateTimeFun(void)
{
	DateTime * dateTime = NULL;
	
	dateTime = MyMalloc(DateTimeStructSize);
	if(dateTime)
	{
		if(My_Pass == RTC_GetTimeData(dateTime))
			setSystemTime(dateTime);
	}
	
	MyFree(dateTime);
}


MyRes SystemFactoryReset(void)
{
	SystemSetData * systemSetData = NULL;
	SystemSetData * GBSystemSetData = NULL;
	
	systemSetData = MyMalloc(SystemSetDataStructSize*2);
	if(systemSetData)
	{
		GBSystemSetData = systemSetData;
		GBSystemSetData++;
		
		setDefaultSystemSetData(systemSetData);
		
		readGbSystemSetData(GBSystemSetData);

		memcpy(systemSetData->deviceId, GBSystemSetData->deviceId, DeviceIdLen);

		systemSetData->testLedLightIntensity = GBSystemSetData->testLedLightIntensity;
		
		if(My_Pass != SaveSystemSetData(systemSetData))
		{
			MyFree(systemSetData);
			return My_Fail;
		}
		else
			MyFree(systemSetData);
	}
	else
		return My_Fail;
	
	if(My_Fail == deleteDeviceFile())
		return My_Fail;

	if(My_Fail == deleteTestDataFile())
		return My_Fail;
	
	if(My_Fail == deleteDeviceQualityFile())
		return My_Fail;
	
	if(My_Fail == deleteDeviceErrorFile())
		return My_Fail;
	
	if(My_Fail == deleteDeviceAdjustFile())
		return My_Fail;
	
	if(My_Fail == deleteDeviceMaintenanceFile())
		return My_Fail;
	
	return My_Pass;
}

