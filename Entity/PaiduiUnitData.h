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
	unsigned char testlocation;
}PaiduiUnitData;
#pragma pack()

#define	PaiduiUnitDataStructSize	sizeof(PaiduiUnitData)


#endif

/****************************************end of file************************************************/
