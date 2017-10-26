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
#define	Motor2_EndTestLocation				27600							//ֹͣ����λ��
#define	Motor2_StartTestLocation			18000							//��ʼ����λ��
#define	Motor2_CatchCardLocation			Motor2_EndTestLocation			//ץ��׼������
#define	Motor2_PutDownCardLocation2			33500							//�Ӳ��Ե�һ�߽������Ŷ�λ�ź�
#define	Motor2_MidLocation					38000							//����λ��
#define	Motor2_PutDownCardLocation			44500							//�������Ŷ�λ�ź�
#define	Motor2_WaitCardLocation				53000							//�ȴ��忨

/*���4��ض���*/
#if(Motor4Type == Motor4UsartMotor)
	#define	Motor4_OpenLocation				20								//���4��
	#define	Motor4_CardLocation				60								//���4�п�
	#define	Motor4_CloseLocation			60								//���4�ϲ�
#elif(Motor4Type == Motor4IOMotor)
	#define	Motor4_OpenLocation				0								//���4��
	#define	Motor4_CardLocation				3650							//���4�п�
	#define	Motor4_CloseLocation			4500							//���4�ϲ�
#endif


Motor * getMotor(Motorx_Def motor);

void setMotorxDir(Motorx_Def motor, bool value);
bool getMotorxDir(Motorx_Def motor);
void toggleMotorxDir(Motorx_Def motor);

void setMotorxMoveStepNum(Motorx_Def motor, unsigned short value);
unsigned short getMotorxMoveStepNum(Motorx_Def motor);
void minusMotorxMoveStepNum(Motorx_Def motor);

void setMotorxPeriodCnt(Motorx_Def motor, unsigned char value);
unsigned char getMotorxPeriodCnt(Motorx_Def motor);
void plusMotorxPeriodCnt(Motorx_Def motor);
	
void setMotorxLocation(Motorx_Def motor, unsigned short value);
unsigned short getMotorxLocation(Motorx_Def motor);
void plusMotorxLocation(Motorx_Def motor, unsigned short value);

void setMotorxTargetLocation(Motorx_Def motor, unsigned short value);
unsigned short getMotorxTargetLocation(Motorx_Def motor);
	
unsigned short getMotorxHighTime(Motorx_Def motor);
unsigned short getMotorxLowTime(Motorx_Def motor);


#endif

/****************************************end of file************************************************/

