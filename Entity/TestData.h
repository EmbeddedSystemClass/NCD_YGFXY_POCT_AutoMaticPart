/****************************************file start****************************************************/
#ifndef _TESTDATA_E_H
#define	_TESTDATA_E_H

#include	"Operator.h"
#include	"QRCode.h"
#include	"DateTime.h"
#include	"Define.h"
#include	"Temperature.h"

#define	TestMotorStepUp					6
#define	TestDataRecordPageShowNum		8							//一页8个数据
#define	TestStep 		(Motor2_EndTestLocation - Motor2_StartTestLocation)/TestMotorStepUp
#define	AvregeNum		5								//平均值滤波个数
#define	MaxPointLen		300

#pragma pack(1)
typedef struct TestSeries_tag {
	unsigned short TestPoint[MaxPointLen];
	Point C_Point;
	Point T_Point;
	Point B_Point;
	float t_c;
	float result;
} TestSeries;
#pragma pack()

#pragma pack(1)
typedef struct TestData_tag {
	Operator 		operator;							//操作人
	char 			sampleid[MaxSampleIDLen];			//样本编号
	QRCode 			qrCode;								//二维码信息
	TestSeries 		testSeries;
	DateTime 		testDateTime;						//测试时间
	unsigned short 	time;								//超时时间
	ResultState 	testResultDesc;						//测试结果状态
	unsigned short 	crc;
}TestData;
#pragma pack()


#define	TestSeriesStructSize	sizeof(TestSeries)

#define	TestDataStructSize		sizeof(TestData)
#define	TestDataStructCrcSize	TestDataStructSize - 2

#endif

/****************************************end of file************************************************/
