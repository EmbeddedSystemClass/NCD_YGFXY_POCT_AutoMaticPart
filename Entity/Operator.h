/****************************************file start****************************************************/
#ifndef _OPERATOR_E_H
#define	_OPERATOR_E_H

#define	OperatorNameLen	20

#pragma pack(1)
typedef struct
{
	unsigned short id;														//id���Ϊ0�����Ǳ�����ӣ���Ҫ��������ˣ������Ϊ0����������������˲�����
	char name[OperatorNameLen];
	char age[10];
	char sex[10];
	char phone[20];
	char job[30];
	char department[20];
	unsigned short crc;
}Operator;
#pragma pack()

#define	MaxPageNum	4
#define	MaxPageShowOperatorSize	5													//һҳ��ʾ�û���Ŀ
#define	MaxOperatorSize	MaxPageNum*MaxPageShowOperatorSize							//��ౣ����û���Ŀ

#define	MaxOperatorPageSize		(MaxOperatorSize / MaxPageShowOperatorSize)			//������ҳ���ҳ��
#define OneOperatorStructSize	sizeof(Operator)									//һ�������˽ṹ���С
#define	OneOperatorStructCrcSize	OneOperatorStructSize - 2						//һ�������˽ṹ���С,������crc
#define	AllOperatorStructSize	(OneOperatorStructSize * MaxOperatorSize)			//���в����˴�С

#endif

/****************************************end of file************************************************/
