#ifndef _MYTEST_S_D_H__
#define _MYTEST_S_D_H__

#include	"Define.h"
#include	"PaiduiUnitData.h"
#include	"Operator.h"

typedef enum
{ 
	Error_None = 0,
	Error_OK = 0x11,																	//创建成功
	Error_Mem = 0x12,																	//内存错误
	Error_PaiDuiBusy = 0x13,															//排队测试忙，禁止再次创建批量测试
	Error_StopNormalTest = 0x14,														//批量测试中，禁止常规测试
	Error_PaiduiFull = 0x15,															//排队位满
	Error_PaiduiTesting = 0x16,														//测试中
	Error_PaiduiDisconnect = 0x17														//排队模块失联
	
}CreateTestErrorType;

CreateTestErrorType CreateANewTest(PaiduiUnitData ** TestDataBuffer);
MyRes DeleteCurrentTest(void);
PaiduiUnitData * GetTestItemByIndex(unsigned char index);
unsigned short GetMinWaitTime(void);
bool isSomePaiduiInOutTimeStatus(void);
bool isInTimeOutStatus(PaiduiUnitData * PaiduiUnitData);
void SetCurrentTestItem(PaiduiUnitData * s_PaiduiUnitData);
PaiduiUnitData * GetCurrentTestItem(void);
bool IsPaiDuiTestting(void);

#endif

