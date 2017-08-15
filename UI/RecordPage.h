#ifndef __RECORD_P_H__
#define __RECORD_P_H__

#include	"MyTest_Data.h"
#include	"TestDataDao.h"
#include	"UI_Data.h"
#include	"SystemSet_Data.h"

typedef struct Record_tag {

	unsigned char selectindex;							//ѡ�е�����
	unsigned int pageindex;								//ҳ������
	unsigned int maxpagenum;
	
	PageRequest pageRequest;							//�������
	DeviceRecordHeader deviceRecordHeader;				//��������ͷ��Ϣ
	TestData testData[TestDataRecordPageShowNum];		//��ȡ���Ĳ�������
	unsigned char readTotalNum;							//��ȡ������Ч������Ŀ
	
	char buf[300];										//��ʱ������
	char tempBuf[100];										//��ʱ������
	unsigned int tempvalue1;
	unsigned int tempvalue2;
	TestData *tempdata;
	unsigned short lcdinput[100];
	SystemSetData systemSetData;
}RecordPageBuffer;

MyRes createRecordActivity(Activity * thizActivity, Intent * pram);

#endif

