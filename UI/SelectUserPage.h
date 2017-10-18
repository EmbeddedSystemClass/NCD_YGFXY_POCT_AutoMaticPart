#ifndef __SUSER_P_H__
#define __SUSER_P_H__

#include	"Define.h"
#include	"MyTest_Data.h"
#include	"Timer.h"
#include	"UI_Data.h"
#include	"Operator.h"
#include	"Device.h"
#include	"Motor_Fun.h"

typedef struct UserPage_tag {
	Device device;													//�豸��Ϣ
	Operator * tempOperator;										//��ʱָ��
	Operator * targetOperator;										//ѡ������˺�Ŀ�껺��
	unsigned short lcdinput[100];									//lcd�������
	char tempBuf[20];
	unsigned short tempV1;
	unsigned char pageindex;										//ҳ������
	PaiduiUnitData * currenttestdata;
	MotorAction motorAction;
}UserPageBuffer;

MyRes createSelectUserActivity(Activity * thizActivity, Intent * pram);

#endif

