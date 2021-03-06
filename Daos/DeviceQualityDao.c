/***************************************************************************************************
*FileName: DeviceAdjustDao
*Description: 读写设备校准记录
*Author: xsx_kair
*Data: 2017年6月2日 15:53:25
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"DeviceQualityDao.h"
#include	"RecordDataDao.h"
#include	"StringDefine.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/


/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
MyRes writeDeviceQualityToFile(DeviceQuality * deviceQuality)
{
	if(deviceQuality)
	{
		return writeRecordDataToFile(DeviceQualityFileName, deviceQuality, DeviceQualityStructSize);
	}
	
	return My_Fail;
}

MyRes readDeviceQualityFromFileByPageRequest(PageRequest * pageRequest, DeviceRecordHeader * deviceRecordHeader, Page * page)
{
	return readRecordDataFromFileByPageRequest(DeviceQualityFileName, pageRequest, deviceRecordHeader, page, DeviceQualityStructSize);
}

MyRes plusDeviceQualityHeaderUpLoadIndexToFile(unsigned char userIndexPlusNum, unsigned char ncdIndexPlusNum)
{
	return plusRecordDataHeaderUpLoadIndexToFile(DeviceQualityFileName, userIndexPlusNum, ncdIndexPlusNum);
}

MyRes deleteDeviceQualityFile(void)
{
	return deleteRecordDataFile(DeviceQualityFileName);
}

/****************************************end of file************************************************/
