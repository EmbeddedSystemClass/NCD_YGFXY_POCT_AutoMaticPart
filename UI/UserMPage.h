#ifndef __USERM_P_H__
#define __USERM_P_H__

#include	"Define.h"
#include	"UI_Data.h"
#include	"SystemSet_Data.h"
#include	"Operator.h"
#include	"MyLock.h"
#include	"Timer.h"

typedef struct UserMPageBuffer_tag {
	unsigned char pageindex;										//页面索引
	Device device;
	Operator operatorCopy[MaxOperatorSize];							//操作人备份
	Operator * tempOperator;
	Operator * currentOperator;
	Operator newOperator;
	unsigned short lcdinput[100];
	char buf[100];
	unsigned short tempV1;
	unsigned short tempV2;
	MyLock * myDeviceLock;
	bool isLocked;
	Timer timer;
}UserMPageBuffer;

MyRes createUserManagerActivity(Activity * thizActivity, Intent * pram);

#endif

