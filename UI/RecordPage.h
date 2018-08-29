#ifndef __RECORD_P_H__
#define __RECORD_P_H__

#include	"MyTest_Data.h"
#include	"TestDataDao.h"
#include	"UI_Data.h"

typedef struct Record_tag {

	PageRequest pageRequest;							//�������
	DeviceRecordHeader deviceRecordHeader;				//��������ͷ��Ϣ
	Page page;											//��ȡ��������Ϣ
	TestData testData[TestDataRecordPageShowNum];		//��ȡ���Ĳ�������

	unsigned int i;
	char buf[300];										//��ʱ������
	unsigned int tempvalue1;
	TestData *tempdata;
	unsigned short lcdinput[20];
}RecordPageBuffer;

MyRes createRecordActivity(Activity * thizActivity, Intent * pram);

#endif

