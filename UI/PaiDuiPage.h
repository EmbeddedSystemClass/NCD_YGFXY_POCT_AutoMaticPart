#ifndef __PAIDUI_P_H__
#define __PAIDUI_P_H__

#include	"Define.h"
#include	"MyTest_Data.h"
#include	"UI_Data.h"

typedef struct PaiDuiPageBuffer_tag {
	unsigned int tempvalue1;					//��ʱʱ��
	unsigned int tempvalue2;					//����ʱʣ��ʱ��
	char buf[100];
	unsigned short lcdinput[20];
	CreateTestErrorType error;
	PaiduiUnitData * currentTestDataBuffer;							//��ǰ�������ݵ�ַ
	PaiduiUnitData * tempPaiduiUnitData;
	unsigned int count;
	unsigned short motor1Location;
	bool isMotorStartted;
	unsigned short index;
	unsigned char lastMotorLocation;
}PaiDuiPageBuffer;


MyRes createPaiDuiActivity(Activity * thizActivity, Intent * pram);


#endif

