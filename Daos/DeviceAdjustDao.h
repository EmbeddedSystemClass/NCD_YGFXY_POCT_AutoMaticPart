/****************************************file start****************************************************/
#ifndef _DEVICEADJUSTDAO_D_H
#define	_DEVICEADJUSTDAO_D_H

#include	"Define.h"
#include	"DeviceAdjust.h"

MyRes writeDeviceAdjustToFile(DeviceAdjust * deviceAdjust);
MyRes readDeviceAdjustFromFileByPageRequest(PageRequest * pageRequest, DeviceRecordHeader * deviceRecordHeader, Page * page);
MyRes plusDeviceAdjustHeaderUpLoadIndexToFile(unsigned char userIndexPlusNum, unsigned char ncdIndexPlusNum);
MyRes deleteDeviceAdjustFile(void);

#endif

/****************************************end of file************************************************/
