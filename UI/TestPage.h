#ifndef __TEST_P_H__
#define __TEST_P_H__

#include	"MyTest_Data.h"
#include	"SystemSet_Data.h"
#include	"Test_Fun.h"
#include	"UI_Data.h"

typedef struct TestPageBuffer_tag {
	PaiduiUnitData * currenttestdata;
	Basic_ICO myico[3];
	LineInfo line;
	unsigned short lcdinput[100];
	unsigned char testisover;
	char buf[100];
	TestData testDataForPrintf;							//����һ�ݲ������ݣ�����ӡ��
	unsigned char isPrintfData;							//�Ƿ����ڴ�ӡ
	SystemSetData systemSetData;
}TestPageBuffer;


MyRes createTestActivity(Activity * thizActivity, Intent * pram);

#endif

