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

/*电机1相关定义*/
#define	Motor1_MaxLocation					18								//电机1最大位置号
#define	Motor1_HalfLocation					PaiDuiWeiNum					//电机1中间位置号

/*电机2相关定义*/
#define	Motor2_PutCardOutLocation			100								//将卡排除设备
#define	Motor2_EndTestLocation				27600							//停止测试位置
#define	Motor2_StartTestLocation			18000							//开始测试位置
#define	Motor2_CatchCardLocation			Motor2_EndTestLocation			//抓卡准备测试
#define	Motor2_PutDownCardLocation2			33500							//从测试的一边将卡在排队位放好
#define	Motor2_MidLocation					38000							//中心位置
#define	Motor2_PutDownCardLocation			44500							//将卡在排队位放好
#define	Motor2_WaitCardLocation				53000							//等待插卡

/*电机4相关定义*/
#if(Motor4Type == Motor4UsartMotor)
	#define	Motor4_OpenLocation				20								//电机4打开
	#define	Motor4_CardLocation				60								//电机4夹卡
	#define	Motor4_CloseLocation			60								//电机4合并
#elif(Motor4Type == Motor4IOMotor)
	#define	Motor4_OpenLocation				0								//电机4打开
	#define	Motor4_CardLocation				3650							//电机4夹卡
	#define	Motor4_CloseLocation			4500							//电机4合并
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

