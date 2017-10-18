#ifndef __NETINFO_P_H__
#define __NETINFO_P_H__

#include	"Define.h"
#include	"UI_Data.h"
#include	"NetWork.h"
#include	"SystemSet_Data.h"
#include	"System_Data.h"
#include	"Timer.h"
#include	"SIM800_Fun.h"

typedef struct NetInfoPageBuffer_tag {
	unsigned short lcdinput[20];
	SystemData systemData;
	Timer timer;
	char tempbuffer1[100];
	Basic_ICO lineico;
	Basic_ICO GSMico;
	bool isGetGSMControl;								//�Ƿ��ѻ�ȡGSM�Ŀ���Ȩ
	Sim800InitBuf sim800InitBuf;
	unsigned char GsmInd;								//�ź�������0-5
	char phoneNum[14];									//�ֻ���
	char ICCID[20];										//sim��ICCID
}NetInfoPageBuffer;


MyRes createNetInfoActivity(Activity * thizActivity, Intent * pram);

#endif

