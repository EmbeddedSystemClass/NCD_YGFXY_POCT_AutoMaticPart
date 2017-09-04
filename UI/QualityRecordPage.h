#ifndef __QUALITYRECORD_P_H__
#define __QUALITYRECORD_P_H__

#include	"DeviceQuality.h"
#include	"UI_Data.h"


typedef struct QualityRecordPageBuffer_tag {
	PageRequest pageRequest;							//请求参数
	DeviceRecordHeader deviceRecordHeader;				//测试数据头信息
	Page page;											//读取的数据信息
	DeviceQuality deviceQuality[DeviceQualityRecordPageShowNum];		//读取到的测试数据
	
	unsigned int i;
	
	char buf[100];										//临时缓冲区
	unsigned int tempvalue1;
	DeviceQuality * tempDeviceQuality;
	unsigned short lcdinput[100];
}QualityRecordPageBuffer;

MyRes createQualityRecordActivity(Activity * thizActivity, Intent * pram);

#endif

