#ifndef __QUALITYRECORD_P_H__
#define __QUALITYRECORD_P_H__

#include	"DeviceQuality.h"
#include	"UI_Data.h"


typedef struct QualityRecordPageBuffer_tag {

	unsigned char selectindex;							//ѡ�е�����
	unsigned int pageindex;							//ҳ������
	unsigned int maxpagenum;
	
	DeviceQualityReadPackge deviceQualityReadPackge;		//��ȡ����
	
	char buf[100];										//��ʱ������
	char tempBuf[100];										//��ʱ������
	unsigned int tempvalue1;
	unsigned int tempvalue2;
	DeviceQuality *tempDeviceQuality;
	unsigned short lcdinput[100];
}QualityRecordPageBuffer;

MyRes createQualityRecordActivity(Activity * thizActivity, Intent * pram);

#endif

