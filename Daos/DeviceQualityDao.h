/****************************************file start****************************************************/
#ifndef _DEVICEQUALITYDAO_D_H
#define	_DEVICEQUALITYDAO_D_H

#include	"Define.h"
#include	"DeviceQuality.h"

MyRes writeDeviceQualityToFile(DeviceQuality * DeviceQuality);
MyRes readDeviceQualityFromFileByPageRequest(PageRequest * pageRequest, DeviceRecordHeader * deviceRecordHeader, Page * page);
MyRes plusDeviceQualityHeaderUpLoadIndexToFile(unsigned char userIndexPlusNum, unsigned char ncdIndexPlusNum);
MyRes deleteDeviceQualityFile(void);

#endif

/****************************************end of file************************************************/
