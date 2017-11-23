#ifndef __USERM_P_H__
#define __USERM_P_H__

#include	"Define.h"
#include	"UI_Data.h"
#include	"SystemSet_Data.h"
#include	"Operator.h"

typedef struct UserMPageBuffer_tag {
	unsigned char pageIndex;										//页面索引
	signed char selectIndex;										//当前页面选择索引
	unsigned char pageValidNum;										//当前页面有效数据个数
	unsigned short allValidNum;										//所有读取到的有效数据个数
	Operator operatorList[MaxOperatorSize];							//操作人列表数据
	Operator operatorDetail;										//操作人详情
	Operator * tempOperator;
	unsigned short lcdinput[30];
	char buf[100];
	unsigned int tempV1;
	unsigned int tempV2;
	unsigned char i;
	unsigned char j;
}UserMPageBuffer;

MyRes createUserManagerActivity(Activity * thizActivity, Intent * pram);

#endif

