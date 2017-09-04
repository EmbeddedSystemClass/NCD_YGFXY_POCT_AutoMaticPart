/****************************************file start****************************************************/
#ifndef _DEVICEERRORDAO_D_H
#define	_DEVICEERRORDAO_D_H

#include	"Define.h"
#include	"DeviceError.h"

MyRes writeDeviceErrorToFile(DeviceError * deviceError);
MyRes readDeviceErrorFromFileByPageRequest(PageRequest * pageRequest, DeviceRecordHeader * deviceRecordHeader, Page * page);
MyRes plusDeviceErrorHeaderUpLoadIndexToFile(unsigned char userIndexPlusNum, unsigned char ncdIndexPlusNum);
MyRes deleteDeviceErrorFile(void);

#endif

/****************************************end of file************************************************/
