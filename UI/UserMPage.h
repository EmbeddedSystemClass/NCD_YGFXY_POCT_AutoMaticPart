#ifndef __USERM_P_H__
#define __USERM_P_H__

#include	"Define.h"
#include	"UI_Data.h"
#include	"SystemSet_Data.h"
#include	"Operator.h"

typedef struct UserMPageBuffer_tag {
	unsigned char pageIndex;										//ҳ������
	signed char selectIndex;										//��ǰҳ��ѡ������
	unsigned char pageValidNum;										//��ǰҳ����Ч���ݸ���
	unsigned short allValidNum;										//���ж�ȡ������Ч���ݸ���
	Operator operatorList[MaxOperatorSize];							//�������б�����
	Operator operatorDetail;										//����������
	Operator * tempOperator;
	unsigned short lcdinput[30];
	char buf[100];
	unsigned int tempV1;
	unsigned int tempV2;
	unsigned char i;
	unsigned char j;
}UserMPageBuffer;

MyRes createUserManagerActivity(Activity * thizActivity, Intent * pram);

#endif

