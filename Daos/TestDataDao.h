/****************************************file start****************************************************/
#ifndef _TESTDATA_D_H
#define	_TESTDATA_D_H

#include	"Define.h"
#include	"SystemSet_Data.h"
#include	"TestData.h"



MyRes writeTestDataToFile(TestData * testData);
MyRes readTestDataFromFileByPageRequest(PageRequest * pageRequest, DeviceRecordHeader * deviceRecordHeader, Page * page);
MyRes plusTestDataHeaderUpLoadIndexToFile(unsigned char userIndexPlusNum, unsigned char ncdIndexPlusNum);
MyRes deleteTestDataFile(void);

#endif

/****************************************end of file************************************************/
