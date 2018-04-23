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
#define	Motor2_EndTestLocation				28200							//停止测试位置27150  29610
#define	Motor2_StartTestLocation			19050							//开始测试位置18000  20460
#define	Motor2_CatchCardLocation			Motor2_EndTestLocation			//抓卡准备测试
#define	Motor2_PutDownCardLocation2			30800							//从测试的一边将卡在排队位放好
#define	Motor2_MidLocation					37000							//中心位置38000
#define	Motor2_PutDownCardLocation			43500							//将卡在排队位放好
#define	Motor2_WaitCardLocation				53000							//等待插卡

/*电机4相关定义*/
#if(Motor4Type == Motor4UsartMotor)
	#define	Motor4_OpenLocation				100								//电机4打开
	#define	Motor4_CardLocation				400								//电机4夹卡
	#define	Motor4_CloseLocation			Motor4_CardLocation				//电机4合并
#elif(Motor4Type == Motor4IOMotor)
	#define	Motor4_OpenLocation				0								//电机4打开
	#define	Motor4_CardLocation				3650							//电机4夹卡
	#define	Motor4_CloseLocation			4500							//电机4合并
#endif

Motor * getMotor(Motorx_Def motor);

#endif

/****************************************end of file************************************************/

