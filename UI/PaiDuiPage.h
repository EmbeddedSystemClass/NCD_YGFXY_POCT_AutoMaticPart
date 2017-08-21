#ifndef __PAIDUI_P_H__
#define __PAIDUI_P_H__

#include	"Define.h"
#include	"MyTest_Data.h"
#include	"UI_Data.h"

typedef struct PaiDuiPageBuffer_tag {
	TestData tempd;
	PaiduiUnitData *tempd2;
	Basic_ICO myico;
	Timer timer0;								//���忨��ʱ��
	Timer timer1;								//����Ŷ�ģ��ʧ����ʱ��
	unsigned short tarindex;
	float tempvalue;
	unsigned int tempvalue1;					//��ʱʱ��
	unsigned int tempvalue2;					//����ʱʣ��ʱ��
	char buf[100];
	char buf2[100];
	unsigned short lcdinput[100];
	CreateTestErrorType error;
	PaiduiUnitData * currentTestDataBuffer;							//��ǰ�������ݵ�ַ
	unsigned char step;												//��������
	unsigned int count;
	bool pageisbusy;
}PaiDuiPageBuffer;


MyRes createPaiDuiActivity(Activity * thizActivity, Intent * pram);


#endif

