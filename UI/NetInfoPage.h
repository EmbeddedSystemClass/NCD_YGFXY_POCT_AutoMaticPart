#ifndef __NETINFO_P_H__
#define __NETINFO_P_H__

#include	"Define.h"
#include	"UI_Data.h"
#include	"NetWork.h"
#include	"SystemSet_Data.h"
#include	"System_Data.h"
#include	"Timer.h"

typedef struct NetInfoPageBuffer_tag {
	unsigned short lcdinput[20];
	SystemData systemData;
	Timer timer;
	char tempbuffer1[100];
	Basic_ICO lineico;
	Basic_ICO wifiico;
	bool isGetWifiControl;								//�Ƿ��ѻ�ȡwifi����Ȩ
	IP WifiIP;										//wifi��ip
	char WifiSSID[30];									//������wifi������
	char WifiMAC[13];									//wifiģ���mac
	unsigned char WifiIndicator;						//������wifi���ź�
}NetInfoPageBuffer;


MyRes createNetInfoActivity(Activity * thizActivity, Intent * pram);

#endif

