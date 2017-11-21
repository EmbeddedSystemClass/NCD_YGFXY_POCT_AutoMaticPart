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
	unsigned char pageIndex;										//ҳ������
	signed char selectIndex;										//��ǰҳ��ѡ������
	unsigned char pageValidNum;										//��ǰҳ����Ч���ݸ���
	unsigned short allValidNum;										//���ж�ȡ������Ч���ݸ���
	Operator operatorList[MaxOperatorSize];							//�������б�����
	Operator * tempOperator;										//��ʱָ��
	Operator * targetOperator;										//ѡ������˺�Ŀ�껺��
	unsigned short lcdinput[100];									//lcd�������
	char tempBuf[20];
	unsigned short tempV1;
	PaiduiUnitData * currenttestdata;
	MotorAction motorAction;
}UserPageBuffer;

MyRes createSelectUserActivity(Activity * thizActivity, Intent * pram);

#endif

