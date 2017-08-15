#ifndef __SDI_P_H__
#define __SDI_P_H__

#include	"UI_Data.h"
#include	"Operator.h"
#include	"Device.h"
#include	"MyLock.h"
#include	"Timer.h"

typedef struct SetDeviceInfoPageBuffer_tag {
	Device device;
	Operator operator;														//��ʱ����
	char deviceunit[DeviceAddrLen];									//�豸ʹ�õ�λ
	bool ismodify;
	unsigned short lcdinput[100];
	char tempBuf[100];
	MyLock * myDeviceLock;
	bool isLocked;
	Timer timer;
}SetDeviceInfoPageBuffer;


MyRes createSetDeviceInfoActivity(Activity * thizActivity, Intent * pram);

#endif

