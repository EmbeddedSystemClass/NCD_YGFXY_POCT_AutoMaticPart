#ifndef _MYTEST_S_D_H__
#define _MYTEST_S_D_H__

#include	"Define.h"
#include	"PaiduiUnitData.h"
#include	"Operator.h"

typedef enum
{ 
	Error_OK = 0,																	//�����ɹ�
	Error_Mem = 1,																	//�ڴ����
	Error_PaiDuiBusy = 2,															//�ŶӲ���æ����ֹ�ٴδ�����������
	Error_StopNormalTest = 3,														//���������У���ֹ�������
	Error_PaiduiFull = 4,															//�Ŷ�λ��
	Error_PaiduiTesting = 5,														//������
	Error_PaiduiDisconnect = 6														//�Ŷ�ģ��ʧ��
}CreateTestErrorType;

CreateTestErrorType CreateANewTest(PaiduiUnitData ** TestDataBuffer);
MyRes DeleteCurrentTest(void);
PaiduiUnitData * GetTestItemByIndex(unsigned char index);
unsigned short GetMinWaitTime(void);
bool isSomePaiduiInOutTimeStatus(void);
bool isInTimeOutStatus(PaiduiUnitData * PaiduiUnitData);
void SetCurrentTestItem(PaiduiUnitData * s_PaiduiUnitData);
PaiduiUnitData * GetCurrentTestItem(void);
bool IsPaiDuiTestting(void);

#endif

