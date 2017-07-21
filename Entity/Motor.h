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
	bool isSleep;									//�Ƿ�����
	unsigned short moveStepNum;						//�˶�����
	unsigned short motorLocation;					//�����ǰλ��
	unsigned short motorTargetLocation;				//���Ŀ��λ��
	unsigned short motorMaxLocation;				//������λ��
	bool parm1;										//�������1
	bool parm2; 									//�������2
}Motor;
#pragma pack()

#endif

/****************************************end of file************************************************/
