/****************************************file start****************************************************/
#ifndef _RETESTDATA_E_H
#define	_RETESTDATA_E_H

#include	"PaiduiUnitData.h"
#include	"DateTime.h"
#include	"Define.h"

typedef struct ReTestData_tag{
	unsigned int retestcount;								//�ܴ���
	unsigned short retestsurpluscount;						//ʣ����Դ���
	unsigned short retestedcount;							//�Ѿ����Դ���
	unsigned char reteststatus;								//�ϻ�����״̬��0ֹͣ����0����
	unsigned char retestStep;								//���Բ��裬1�ȴ��忨��2����ά�룬3����
	Timer retesttimer;										//�ϻ����Լ�ʱ��
	Timer oneretesttimer;									//һ���ϻ����Լ�ʱ��
	PaiduiUnitData paiduiUnitData;
	char result[30];										//��ǰ���ԵĽ���
	unsigned short playcount;								//�ϻ����ż���������¼���Ŵ���
	Timer playtimer;										//�ϻ���Ƶ��ʱ��	
	Timer oneplaytimer;										//�ϻ����ż�ʱ������¼ÿ�β��ŵ�ʱ��
	DateTime startplayTime;								//��ǰ��Ƶ��ʼʱ��
	DateTime endplayTime;									//��ǰ��Ƶ����ʱ��
	double advalue1;										//DAֵ100ʱ��ADֵ
	double advalue2;										//DAֵ200ʱ��ADֵ
	double advalue3;										//DAֵ300ʱ��ADֵ
	MyLEDCheck_TypeDef ledstatus;							//����ģ��״̬
	unsigned char playstatus;								//����״̬
}ReTestData;

#endif

/****************************************end of file************************************************/
