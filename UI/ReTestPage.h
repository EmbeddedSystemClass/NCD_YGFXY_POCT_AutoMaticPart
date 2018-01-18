#ifndef __RETSET_P_H__
#define __RETSET_P_H__

#include	"Define.h"
#include	"UI_Data.h"
#include	"QRCode.h"
#include	"TestData.h"
#include	"Timer.h"
#include	"DateTime.h"
#include	"PaiduiUnitData.h"
#include	"Motor_Fun.h"

typedef struct ReTestPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[50];
	char longBuffer[1024];
	unsigned char testStep;								//0 -- �忨, 1 -- �ϻ���
	unsigned int testNum;								//���Դ�����9����ȫ������һ�μ�1
	unsigned char testIndex;							//��ǰ���Կ�����
	PaiduiUnitData paiduiUnitData[PaiDuiWeiNum];
	PaiduiUnitData * currentPaiduiUnitData;
	unsigned char i;
	MotorAction motorAction;
	ScanCodeResult cardScanResult;							//ɨ����
}ReTestPageBuffer;


MyRes createReTestActivity(Activity * thizActivity, Intent * pram);

#endif

