#ifndef _MYTEST_S_D_H__
#define _MYTEST_S_D_H__

#include	"Define.h"
#include	"PaiduiUnitData.h"
#include	"Operator.h"

typedef enum
{ 
	Error_None = 0,
	Error_OK = 0x11,																	//�����ɹ�
	Error_Mem = 0x12,																	//�ڴ����
	Error_PaiDuiBusy = 0x13,															//�ŶӲ���æ����ֹ�ٴδ�����������
	Error_StopNormalTest = 0x14,														//���������У���ֹ�������
	Error_PaiduiFull = 0x15,															//�Ŷ�λ��
	Error_PaiduiTesting = 0x16,														//������
	Error_PaiduiDisconnect = 0x17														//�Ŷ�ģ��ʧ��
	
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

