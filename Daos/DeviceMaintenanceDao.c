/***************************************************************************************************
*FileName: DeviceAdjustDao
*Description: ��д�豸У׼��¼
*Author: xsx_kair
*Data: 2017��6��2�� 15:53:25
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"DeviceMaintenanceDao.h"
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

MyRes writeDeviceMaintenanceToFile(DeviceMaintenance * deviceMaintenance)
{
	if(deviceMaintenance)
	{
		return writeRecordDataToFile(DeviceMainenanceFileName, deviceMaintenance, DeviceMaintenanceStructSize);
	}
	
	return My_Fail;
}
MyRes readDeviceMaintenanceFromFileByPageRequest(PageRequest * pageRequest, DeviceRecordHeader * deviceRecordHeader, Page * page)
{
	return readRecordDataFromFileByPageRequest(DeviceMainenanceFileName, pageRequest, deviceRecordHeader, page, DeviceMaintenanceStructSize);
}

MyRes plusDeviceMaintenanceHeaderUpLoadIndexToFile(unsigned char userIndexPlusNum, unsigned char ncdIndexPlusNum)
{
	return plusRecordDataHeaderUpLoadIndexToFile(DeviceMainenanceFileName, userIndexPlusNum, ncdIndexPlusNum);
}

MyRes deleteDeviceMaintenanceFile(void)
{
	return deleteRecordDataFile(DeviceMainenanceFileName);
}

/****************************************end of file************************************************/
