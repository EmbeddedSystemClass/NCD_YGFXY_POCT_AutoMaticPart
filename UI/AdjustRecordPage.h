#ifndef __ADJUSTRECORD_P_H__
#define __ADJUSTRECORD_P_H__

#include	"DeviceAdjust.h"
#include	"DeviceAdjustDao.h"
#include	"UI_Data.h"


typedef struct AdjustRecordPageBuffer_tag {
	PageRequest pageRequest;							//请求参数
	DeviceRecordHeader deviceRecordHeader;				//测试数据头信息
	Page page;											//读取的数据信息
	DeviceAdjust deviceAdjust[DeviceAdjustRecordPageShowNum];		//读取到的测试数据
	
	unsigned int i;
	char buf[100];										//临时缓冲区
	unsigned int tempvalue1;
	DeviceAdjust *tempDeviceAdjust;
	unsigned short lcdinput[100];
}AdjustRecordPageBuffer;

MyRes createAdjustRecordActivity(Activity * thizActivity, Intent * pram);

#endif

