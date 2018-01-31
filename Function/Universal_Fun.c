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
#include	"Led_Driver.h"
#include	"SystemSet_Dao.h"
#include	"System_Data.h"
#include	"TestDataDao.h"
#include	"DeviceDao.h"
#include	"DeviceMaintenanceDao.h"
#include	"DeviceAdjustDao.h"
#include	"DeviceErrorDao.h"
#include	"DeviceQualityDao.h"
#include	"RemoteSoft_Data.h"
#include	"DateTime.h"
#include	"MyMem.h"
#include	"MyTools.h"
#include	"CRC16.h"

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
	DateTime * dateTime = getSystemTime(NULL);
	
	RTC_GetTimeData(dateTime);
}


MyRes SystemFactoryReset(void)
{
	SystemResetBuf * systemResetBuf = NULL;
	
	systemResetBuf = MyMalloc(SystemResetBufStructSize);
	if(systemResetBuf)
	{
		systemResetBuf->systemSetData = (SystemSetData *)systemResetBuf->buffer;

		setDefaultSystemSetData(systemResetBuf->systemSetData);
		systemResetBuf->systemSetData->testLedLightIntensity = getSystemTestLedLightIntensity();
		
		if(My_Pass != SaveSystemSetData(systemResetBuf->systemSetData))
		{
			MyFree(systemResetBuf);
			return My_Fail;
		}
	}
	else
		return My_Fail;
	
	systemResetBuf->device = (Device *)systemResetBuf->buffer;
	memset(systemResetBuf->device, 0, DeviceStructSize);
	getSystemDeviceId(systemResetBuf->device->deviceId);
	systemResetBuf->device->crc = CalModbusCRC16Fun(systemResetBuf->device, DeviceStructCrcSize, NULL);
	if(My_Fail == SaveDeviceToFile(systemResetBuf->device))
	{
		MyFree(systemResetBuf);
		return My_Fail;
	}
	
	MyFree(systemResetBuf);

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

void checkCanRestartForUpgrade(void)
{
	unsigned short systemWorkStatus = 0;
	
	systemWorkStatus = readSystemWorkStatus();
	
	if((systemWorkStatus & (SystemTestting | SystemQualityong)) == 0 && getIsSuccessDownloadFirmware() == true )
		while(1);
}
