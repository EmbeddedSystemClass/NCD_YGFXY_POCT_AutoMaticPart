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
	Timer timeDown_timer;																	//����ʱ��ʱ��
	Timer timeUp_timer;																		//��ʱ��ʱ��
	unsigned char testlocation;
}PaiduiUnitData;
#pragma pack()

#define	PaiduiUnitDataStructSize	sizeof(PaiduiUnitData)


#endif

/****************************************end of file************************************************/
