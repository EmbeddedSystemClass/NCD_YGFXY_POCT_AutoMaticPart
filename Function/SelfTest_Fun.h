#ifndef SELFCHECK_FUN_H
#define SELFCHECK_FUN_H

#include	"Define.h"

typedef enum
{ 
	SystemData_OK = 0,								//ϵͳ���ݼ��سɹ�
	SystemData_ERROR = 1,							//ϵͳ���ݼ���ʧ��
	
	Light_OK = 2,									//led����
	Light_Error = 3,								//led�쳣
	
	AD_OK = 4,										//�ɼ�ģ������
	AD_ERROR = 5,									//�ɼ�ģ�����
	
	Motol_OK = 6,									//����ģ������
	Motol_ERROR = 7,								//����ģ�����
	
	Erweima_OK = 8,									//��ά��ģ������
	Erweima_ERROR = 9,								//��ά��ģ�����
	
	SD_OK = 10,
	SD_ERROR = 11,
	
	SelfTestting = 99,								//�Լ���
	SelfTest_OK = 100								//�Լ�ɹ�
}ERROR_SelfTest;

void SelfTest_Function(void);
ERROR_SelfTest getSelfTestStatus(void);

#endif

