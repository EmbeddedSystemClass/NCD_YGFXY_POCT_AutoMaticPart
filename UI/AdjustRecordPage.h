#ifndef __ADJUSTRECORD_P_H__
#define __ADJUSTRECORD_P_H__

#include	"DeviceAdjust.h"
#include	"DeviceAdjustDao.h"
#include	"UI_Data.h"


typedef struct AdjustRecordPageBuffer_tag {
	PageRequest pageRequest;							//�������
	DeviceRecordHeader deviceRecordHeader;				//��������ͷ��Ϣ
	Page page;											//��ȡ��������Ϣ
	DeviceAdjust deviceAdjust[DeviceAdjustRecordPageShowNum];		//��ȡ���Ĳ�������
	
	unsigned int i;
	char buf[100];										//��ʱ������
	unsigned int tempvalue1;
	DeviceAdjust *tempDeviceAdjust;
	unsigned short lcdinput[100];
}AdjustRecordPageBuffer;

MyRes createAdjustRecordActivity(Activity * thizActivity, Intent * pram);

#endif

