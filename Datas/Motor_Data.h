/****************************************file start****************************************************/
#ifndef	MOTOR_D_H
#define	MOTOR_D_H

#include	"Motor.h"

typedef enum
{ 
	Motor_1 = 0x00,
	Motor_2 = 0x01,
	Motor_4 = 0x02,
	Motor_None = 0xff,
}Motorx_Def;

#define	MotorLocationZero					0
#define	MotorLocationNone					MotorLocationZero

/*���1��ض���*/
#define	Motor1_MaxLocation					18								//���1���λ�ú�
#define	Motor1_HalfLocation					PaiDuiWeiNum					//���1�м�λ�ú�

/*���2��ض���*/
#define	Motor2_PutCardOutLocation			100								//�����ų��豸
#define	Motor2_EndTestLocation				28200							//ֹͣ����λ��27150  29610
#define	Motor2_StartTestLocation			19050							//��ʼ����λ��18000  20460
#define	Motor2_CatchCardLocation			Motor2_EndTestLocation			//ץ��׼������
#define	Motor2_PutDownCardLocation2			30800							//�Ӳ��Ե�һ�߽������Ŷ�λ�ź�
#define	Motor2_MidLocation					37000							//����λ��38000
#define	Motor2_PutDownCardLocation			43500							//�������Ŷ�λ�ź�
#define	Motor2_WaitCardLocation				53000							//�ȴ��忨

/*���4��ض���*/
#if(Motor4Type == Motor4UsartMotor)
	#define	Motor4_OpenLocation				100								//���4��
	#define	Motor4_CardLocation				400								//���4�п�
	#define	Motor4_CloseLocation			Motor4_CardLocation				//���4�ϲ�
#elif(Motor4Type == Motor4IOMotor)
	#define	Motor4_OpenLocation				0								//���4��
	#define	Motor4_CardLocation				3650							//���4�п�
	#define	Motor4_CloseLocation			4500							//���4�ϲ�
#endif

Motor * getMotor(Motorx_Def motor);

#endif

/****************************************end of file************************************************/

