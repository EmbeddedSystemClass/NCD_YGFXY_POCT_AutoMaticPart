/****************************************file start****************************************************/
#ifndef _DEVICEMAINTENANCEDAO_D_H
#define	_DEVICEMAINTENANCEDAO_D_H

#include	"Define.h"
#include	"DeviceMaintenance.h"

MyRes writeDeviceMaintenanceToFile(DeviceMaintenance * deviceMaintenance);
MyRes readDeviceMaintenanceFromFileByPageRequest(PageRequest * pageRequest, DeviceRecordHeader * deviceRecordHeader, Page * page);
MyRes plusDeviceMaintenanceHeaderUpLoadIndexToFile(unsigned char userIndexPlusNum, unsigned char ncdIndexPlusNum);
MyRes deleteDeviceMaintenanceFile(void);

#endif

/****************************************end of file************************************************/
