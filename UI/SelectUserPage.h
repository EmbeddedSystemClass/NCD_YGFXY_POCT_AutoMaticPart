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
	unsigned char pageIndex;										//页面索引
	signed char selectIndex;										//当前页面选择索引
	unsigned char pageValidNum;										//当前页面有效数据个数
	unsigned short allValidNum;										//所有读取到的有效数据个数
	Operator operatorList[MaxOperatorSize];							//操作人列表数据
	Operator * tempOperator;										//临时指针
	Operator * targetOperator;										//选择操作人后目标缓存
	unsigned short lcdinput[100];									//lcd输入解析
	char tempBuf[20];
	unsigned short tempV1;
	PaiduiUnitData * currenttestdata;
	MotorAction motorAction;
}UserPageBuffer;

MyRes createSelectUserActivity(Activity * thizActivity, Intent * pram);

#endif

