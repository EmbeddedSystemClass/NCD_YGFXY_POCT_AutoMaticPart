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
	double average;														//ƽ��ֵ
	double stdev;														//��׼��
	double tempCV;
	double CV_T;															//����ϵ��1
	double CV_C;															//����ϵ��1
	double CV_0;															//����ϵ��1
	double finalBili;
	unsigned short tempvalue3;
	bool isE;
	unsigned short tempSeries[MaxPointLen+FilterNum];
	double lastdata;														//��¼�ϴ��˲�����
	unsigned short motorLocation;
	unsigned short maxdata;
	PaiduiUnitData * paiduiUnitData;												//��������ָ��
	ResultState resultstatues;											//���Խ��״̬
	Motor * motor;
}TempCalData;
#pragma pack()

MyRes InitTestFunData(void);
ResultState TestFunction(PaiduiUnitData * parm);

MyRes TakeTestPointData(void * data);
MyRes TakeTestResultData(void * data);

#endif

