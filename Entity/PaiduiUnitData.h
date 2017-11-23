/****************************************file start****************************************************/
#ifndef _PAIDUIUNITDATA_E_H
#define	_PAIDUIUNITDATA_E_H

#include	"TestData.h"
#include	"DateTime.h"
#include	"Define.h"
#include	"Timer.h"

#pragma pack(1)
typedef struct PaiduiUnitData_tag {
	TestData testData;
	Timer timeDown_timer;																	//倒计时计时器
	Timer timeUp_timer;																		//超时计时器
	unsigned char index;																	//测试数据的索引
	unsigned char cardLocation;																//放卡的位置号
	unsigned char testLocation;																//对应的测试位置号
	unsigned short ledValue;																//测试led亮度
	MyPaiDuiStatues statues;																//测试阶段
}PaiduiUnitData;
#pragma pack()

#define	PaiduiUnitDataStructSize	sizeof(PaiduiUnitData)


#endif

/****************************************end of file************************************************/
