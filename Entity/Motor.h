/****************************************file start****************************************************/
#ifndef _MOTOR_E_H
#define	_MOTOR_E_H

#include	"Define.h"

#pragma pack(1)
typedef struct Motor_Tag
{
	unsigned char highTime;							//�ߵ�ƽʱ�䣬*100us
	unsigned char lowTime;							//�͵�ƽʱ�䣬*100us
	unsigned char periodCnt;						//��ǰ�����ڣ���ʱ������жϴ���
	bool isFront;									//�Ƿ�ǰ��
	unsigned int moveStepNum;						//�˶�����
	unsigned int motorLocation;					//�����ǰλ��
	unsigned int motorTargetLocation;				//���Ŀ��λ��
	unsigned int motorMaxLocation;				//������λ��
	unsigned int parm1;							//�������1
	bool parm2; 									//�������2
	unsigned int parm3; 									//�������3
}Motor;
#pragma pack()

#endif

/****************************************end of file************************************************/
