/****************************************file start****************************************************/
#ifndef _DEVICEQUALITY_E_H
#define	_DEVICEQUALITY_E_H

#include	"DateTime.h"
#include	"Operator.h"
#include	"Define.h"
#include	"QRCode.h"
#include	"TestData.h"

#define		DeviceQualityRecordPageShowNum	8

#pragma pack(1)
typedef struct
{
	DateTime dateTime;												//质控时间
	Operator operator;												//质控人
	char itemName[ItemNameLen];										//质控项目名称
	double standardValue;											//质控标准值
	double avgValue;												//测试平均值
	double avgPicha;												//平均偏差
	double testValue[MaxQualityCount];								//质控测试值
	double testPicha[MaxQualityCount];								//质控测试偏差
	MyBool testResult[MaxQualityCount];								//质控结果标志
	bool isOk;														//质控结果
	char desc[50];													//质控说明
	unsigned short crc;
}DeviceQuality;
#pragma pack()

#define	DeviceQualityStructSize		sizeof(DeviceQuality)							//最多保存的用户数目
#define	DeviceQualityStructCrcSize	DeviceQualityStructSize - 2						//最多保存的用户数目

#endif

/****************************************end of file************************************************/
