#ifndef __ERRORRECORD_P_H__
#define __ERRORRECORD_P_H__

#include	"DeviceError.h"
#include	"DeviceErrorDao.h"
#include	"UI_Data.h"


typedef struct ErrorRecordPageBuffer_tag {

	unsigned char selectindex;							//ѡ�е�����
	unsigned int pageindex;							//ҳ������
	unsigned int maxpagenum;
	
	DeviceErrorReadPackge deviceErrorReadPackge;		//��ȡ����
	
	char buf[100];										//��ʱ������
	char tempBuf[100];										//��ʱ������
	unsigned int tempvalue1;
	unsigned int tempvalue2;
	DeviceError *tempDeviceError;
	unsigned short lcdinput[100];
}ErrorRecordPageBuffer;

MyRes createErrorRecordActivity(Activity * thizActivity, Intent * pram);

#endif

