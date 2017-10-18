#ifndef __ADJUSTLED_P_H__
#define __ADJUSTLED_P_H__

#include	"UI_Data.h"
#include	"PaiduiUnitData.h"
#include	"SystemSet_Data.h"
#include	"Motor_Fun.h"

typedef struct AdjustLedPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[100];
	bool isTestting;									//�Ƿ����ڲ�����
	bool isReady;
	ResultState cardpretestresult;
	unsigned short targetValue;							//Ŀ��ֵ
	unsigned short targetRange;							//���ֵ
	unsigned short maxPoint[2];							//0--���ֵ��1--���ֵλ��
	unsigned short i;
	unsigned char testCnt;								//���Ҵ���
	SystemSetData systemSetData;						//ϵͳ�������������ڱ���ʧ�ܻع�
	PaiduiUnitData paiduiUnitData;
	unsigned short ledLevel;
	MotorAction motorAction;
}AdjustLedPageBuffer;


MyRes createAdjustLedActivity(Activity * thizActivity, Intent * pram);

#endif
