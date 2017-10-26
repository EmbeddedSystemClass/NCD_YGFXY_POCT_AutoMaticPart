#ifndef __QUALITYDETAIL_P_H__
#define __QUALITYDETAIL_P_H__

#include	"UI_Data.h"
#include	"DeviceQuality.h"

typedef struct QualityDetailPageBuffer_tag {
	unsigned short lcdinput[100];
	char tempBuf[100];
	DeviceQuality deviceQuality;
	double tempValue1;
	unsigned int tempIntV1;
	double tempValue2;
	unsigned char tempValue3;
	unsigned int i;
}QualityDetailPageBuffer;

#define	QualityDetailPageBufferStructSize	sizeof(QualityDetailPageBuffer)

MyRes createQualityDetailActivity(Activity * thizActivity, Intent * pram);

#endif

