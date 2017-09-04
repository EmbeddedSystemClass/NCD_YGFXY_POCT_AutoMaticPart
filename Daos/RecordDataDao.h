/****************************************file start****************************************************/
#ifndef _RECORDDATA_D_H
#define	_RECORDDATA_D_H

#include	"Define.h"

MyRes writeRecordDataToFile(const char * fileName, void * recordData, unsigned int recordItemSize);
MyRes readRecordDataFromFileByPageRequest(const char * fileName, PageRequest * pageRequest, DeviceRecordHeader * deviceRecordHeader, 
	Page * page, unsigned short ItemByteSize);
MyRes plusRecordDataHeaderUpLoadIndexToFile(const char * fileName, unsigned char userIndexPlusNum, unsigned char ncdIndexPlusNum);
MyRes deleteRecordDataFile(const char * fileName);

#endif

/****************************************end of file************************************************/
