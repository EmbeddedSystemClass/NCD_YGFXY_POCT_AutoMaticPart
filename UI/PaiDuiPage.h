#ifndef __PAIDUI_P_H__
#define __PAIDUI_P_H__

#include	"Define.h"
#include	"MyTest_Data.h"
#include	"UI_Data.h"

typedef struct PaiDuiPageBuffer_tag {
	TestData tempd;
	PaiduiUnitData *tempd2;
	Basic_ICO myico;
	Timer timer0;								//检测插卡计时器
	Timer timer1;								//检测排队模块失联计时器
	unsigned short tarindex;
	float tempvalue;
	unsigned int tempvalue1;					//超时时间
	unsigned int tempvalue2;					//倒计时剩余时间
	char buf[100];
	char buf2[100];
	unsigned short lcdinput[100];
	CreateTestErrorType error;
	PaiduiUnitData * currentTestDataBuffer;							//当前测试数据地址
	unsigned char step;												//创建步骤
	unsigned int count;
	bool pageisbusy;
}PaiDuiPageBuffer;


MyRes createPaiDuiActivity(Activity * thizActivity, Intent * pram);


#endif

