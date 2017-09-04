#ifndef __ERRORRECORD_P_H__
#define __ERRORRECORD_P_H__

#include	"DeviceError.h"
#include	"DeviceErrorDao.h"
#include	"UI_Data.h"


typedef struct ErrorRecordPageBuffer_tag {
	PageRequest pageRequest;							//请求参数
	DeviceRecordHeader deviceRecordHeader;				//测试数据头信息
	Page page;											//读取的数据信息
	DeviceError deviceError[DeviceErrorRecordPageShowNum];		//读取到的测试数据
	
	unsigned int i;
	
	char buf[100];										//临时缓冲区
	unsigned int tempvalue1;
	DeviceError * tempDeviceError;
	unsigned short lcdinput[100];
}ErrorRecordPageBuffer;

MyRes createErrorRecordActivity(Activity * thizActivity, Intent * pram);

#endif

