/****************************************file start****************************************************/
#ifndef	MOTOR_D_H
#define	MOTOR_D_H

#include	"Motor.h"

typedef enum
{ 
	Motor_1 = 0x00,
	Motor_2 = 0x01,
	Motor_3 = 0x02,
	Motor_4 = 0x03,
	Motor_None = 0xff,
}Motorx_Def;

void setMotorxDir(Motorx_Def motor, bool value);
bool getMotorxDir(Motorx_Def motor);
void toggleMotorxDir(Motorx_Def motor);

void setMotorxSleep(Motorx_Def motor, bool value);
bool getMotorxSleep(Motorx_Def motor);
void toggleMotorxSleep(Motorx_Def motor);

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

unsigned short getMotorxMaxLocation(Motorx_Def motor);

void setMotorxParm1(Motorx_Def motor, bool value);
bool getMotorxParm1(Motorx_Def motor);
#endif

/****************************************end of file************************************************/
