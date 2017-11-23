#ifndef __ADJUSTLED_P_H__
#define __ADJUSTLED_P_H__

#include	"UI_Data.h"
#include	"PaiduiUnitData.h"
#include	"SystemSet_Data.h"
#include	"Motor_Fun.h"

#define	MinLedValue		600								//0.5V
#define	MiddleLedValue	1200							//1V
#define	MaxLedValue		1800							//1.5V
#define	AdjustLedStep	100
#define	MaxAdjustCnt	7

typedef struct AdjustLedPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[100];
	ResultState cardTestResult;								//���Խ��
	unsigned short targetValue;							//Ŀ��ֵ
	unsigned short targetRange;							//���ֵ
	unsigned short maxPoint[2];							//0--���ֵ��1--���ֵλ��
	unsigned short i;
	unsigned char testIndex;								//���Ҵ���
	SystemSetData systemSetData;						//ϵͳ�������������ڱ���ʧ�ܻع�
	PaiduiUnitData paiduiUnitData;
	unsigned short currentLedValue;						//��ǰϵͳ�е�ledֵ
	MotorAction motorAction;
}AdjustLedPageBuffer;


MyRes createAdjustLedActivity(Activity * thizActivity, Intent * pram);

#endif
