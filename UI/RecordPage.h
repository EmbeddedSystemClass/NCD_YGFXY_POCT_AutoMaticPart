#ifndef __RECORD_P_H__
#define __RECORD_P_H__

#include	"MyTest_Data.h"
#include	"TestDataDao.h"
#include	"UI_Data.h"

typedef struct Record_tag {

	PageRequest pageRequest;							//请求参数
	DeviceRecordHeader deviceRecordHeader;				//测试数据头信息
	Page page;											//读取的数据信息
	TestData testData[TestDataRecordPageShowNum];		//读取到的测试数据

	unsigned int i;
	char buf[300];										//临时缓冲区
	unsigned int tempvalue1;
	TestData *tempdata;
	unsigned short lcdinput[20];
}RecordPageBuffer;

MyRes createRecordActivity(Activity * thizActivity, Intent * pram);

#endif

