#ifndef __TESTF_H__
#define __TESTF_H__

#include	"Define.h"
#include 	"FreeRTOS.h"
#include	"MyTest_Data.h"

#define	DataMaxWinSize			20
#define	TestMotorStep	(Motor2_StartTestLocation - Motor2_EndTestLocation)

#pragma pack(1)
typedef struct TempCalData_tag{
	double tempvalue1;
	double tempvalue2;
	double average;														//平均值
	double stdev;														//标准差
	double CV_1;															//变异系数1
	double CV_2;															//变异系数1
	double CV_3;															//变异系数1
	double CV_4;															//变异系数1
	double CV_5;															//变异系数2
	unsigned short tempvalue3;
	unsigned short temptestline[MaxPointLen];
	double lastdata;														//记录上次滤波数据
	unsigned short motorLocation;
	unsigned short maxdata;
	PaiduiUnitData * paiduiUnitData;												//测试数据指针
	ResultState resultstatues;											//测试结果状态
}TempCalData;
#pragma pack()

MyRes InitTestFunData(void);
ResultState TestFunction(PaiduiUnitData * parm);

MyRes TakeTestPointData(void * data);
MyRes TakeTestResultData(void * data);

#endif

