/****************************************file start****************************************************/
#ifndef _PAIDUIUNITDATA_E_H
#define	_PAIDUIUNITDATA_E_H

#include	"TestData.h"
#include	"DateTime.h"
#include	"Define.h"

#pragma pack(1)
typedef struct PaiduiUnitData_tag {
	TestData testData;
	Timer timer;																	//���浹��ʱ��ʱ��
	Timer timer2;																	//��ʱ����ʱ��ʱ��
	Timer timer3;																	//��ʱ����ʱ��ʱ��
	unsigned char errorcount;														//δ��������
	MyPaiDuiStatues statues;														//���Խ׶�
	unsigned char jieduan;															//���Խ׶�
	unsigned char testlocation;
	unsigned short ledLight;														//����ʱled�����ȣ�Ϊϵͳ�����б����У׼ledֵ
}PaiduiUnitData;
#pragma pack()

#endif

/****************************************end of file************************************************/
