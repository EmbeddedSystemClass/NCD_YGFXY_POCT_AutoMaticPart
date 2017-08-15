#ifndef __SUSER_P_H__
#define __SUSER_P_H__

#include	"Define.h"
#include	"MyTest_Data.h"
#include	"Timer.h"
#include	"UI_Data.h"
#include	"Operator.h"
#include	"Device.h"

typedef struct UserPage_tag {
	unsigned char selectindex;										//ѡ�е�����
	unsigned char pageindex;										//ҳ������
	Device device;													//�豸��Ϣ
	Operator * tempUser;											//��ʱָ��
	Operator * tempUser2;											//��ǰѡ�еĲ����ˣ�null��ʾδѡ��
	Operator * targetOperator;										//ѡ������˺�Ŀ�껺��
	unsigned short lcdinput[100];									//lcd�������
}UserPageBuffer;

MyRes createSelectUserActivity(Activity * thizActivity, Intent * pram);

#endif

