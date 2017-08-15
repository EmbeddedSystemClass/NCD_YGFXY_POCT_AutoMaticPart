#ifndef __ADJUSTRECORD_P_H__
#define __ADJUSTRECORD_P_H__

#include	"DeviceAdjust.h"
#include	"DeviceAdjustDao.h"
#include	"UI_Data.h"


typedef struct AdjustRecordPageBuffer_tag {

	unsigned char selectindex;							//ѡ�е�����
	unsigned int pageindex;							//ҳ������
	unsigned int maxpagenum;
	
	DeviceAdjustReadPackge deviceAdjustReadPackge;		//��ȡ����
	
	char buf[100];										//��ʱ������
	char tempBuf[100];										//��ʱ������
	unsigned int tempvalue1;
	unsigned int tempvalue2;
	DeviceAdjust *tempDeviceAdjust;
	unsigned short lcdinput[100];
}AdjustRecordPageBuffer;

MyRes createAdjustRecordActivity(Activity * thizActivity, Intent * pram);

#endif

