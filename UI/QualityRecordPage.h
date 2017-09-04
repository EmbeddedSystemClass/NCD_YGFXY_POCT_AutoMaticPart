#ifndef __QUALITYRECORD_P_H__
#define __QUALITYRECORD_P_H__

#include	"DeviceQuality.h"
#include	"UI_Data.h"


typedef struct QualityRecordPageBuffer_tag {
	PageRequest pageRequest;							//�������
	DeviceRecordHeader deviceRecordHeader;				//��������ͷ��Ϣ
	Page page;											//��ȡ��������Ϣ
	DeviceQuality deviceQuality[DeviceQualityRecordPageShowNum];		//��ȡ���Ĳ�������
	
	unsigned int i;
	
	char buf[100];										//��ʱ������
	unsigned int tempvalue1;
	DeviceQuality * tempDeviceQuality;
	unsigned short lcdinput[100];
}QualityRecordPageBuffer;

MyRes createQualityRecordActivity(Activity * thizActivity, Intent * pram);

#endif

