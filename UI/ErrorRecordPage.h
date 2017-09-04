#ifndef __ERRORRECORD_P_H__
#define __ERRORRECORD_P_H__

#include	"DeviceError.h"
#include	"DeviceErrorDao.h"
#include	"UI_Data.h"


typedef struct ErrorRecordPageBuffer_tag {
	PageRequest pageRequest;							//�������
	DeviceRecordHeader deviceRecordHeader;				//��������ͷ��Ϣ
	Page page;											//��ȡ��������Ϣ
	DeviceError deviceError[DeviceErrorRecordPageShowNum];		//��ȡ���Ĳ�������
	
	unsigned int i;
	
	char buf[100];										//��ʱ������
	unsigned int tempvalue1;
	DeviceError * tempDeviceError;
	unsigned short lcdinput[100];
}ErrorRecordPageBuffer;

MyRes createErrorRecordActivity(Activity * thizActivity, Intent * pram);

#endif

