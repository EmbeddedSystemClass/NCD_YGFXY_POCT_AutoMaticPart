#ifndef __TESTF_H__
#define __TESTF_H__

#include	"Define.h"
#include 	"FreeRTOS.h"
#include	"MyTest_Data.h"
#include	"Motor_Data.h"

#pragma pack(1)
typedef struct TempCalData_tag{
	double tempvalue1;
	double tempvalue2;
	double average;														//平均值
	double stdev;														//标准差
	double tempCV;
	double CV_T;															//变异系数1
	double CV_C;															//变异系数1
	double CV_0;															//变异系数1
	double finalBili;
	unsigned short tempvalue3;
	bool isE;
	unsigned short tempSeries[MaxPointLen+FilterNum];
	double lastdata;														//记录上次滤波数据
	unsigned short motorLocation;
	unsigned short maxdata;
	PaiduiUnitData * paiduiUnitData;												//测试数据指针
	ResultState resultstatues;											//测试结果状态
	Motor * motor;
}TempCalData;
#pragma pack()

MyRes InitTestFunData(void);
ResultState TestFunction(PaiduiUnitData * parm);

MyRes TakeTestPointData(void * data);
MyRes TakeTestResultData(void * data);

#endif

