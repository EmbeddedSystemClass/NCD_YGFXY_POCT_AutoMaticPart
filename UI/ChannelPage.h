#ifndef __CHANNEL_P_H__
#define __CHANNEL_P_H__

#include	"UI_Data.h"
#include	"PaiduiUnitData.h"
#include	"Motor_Fun.h"
#include	"Define.h"

#define	MaxChannelTestCnt		9

typedef struct ChannelPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[100];
	ScanCodeResult scancode;
	unsigned short targetChannel;							//Ŀ��ͨ��
	unsigned short targetCnt;								//���Դ���
	unsigned short i;
	unsigned short testIndex;								//���Ҵ���
	PaiduiUnitData paiduiUnitData;
	MotorAction motorAction;
	Point T_Point[MaxChannelTestCnt];
	Point C_Point[MaxChannelTestCnt];
	float t_c[MaxChannelTestCnt];
	float testValue[MaxChannelTestCnt];
	DateTime dateTime;
}ChannelPageBuffer;


MyRes createChannelTestActivity(Activity * thizActivity, Intent * pram);

#endif
