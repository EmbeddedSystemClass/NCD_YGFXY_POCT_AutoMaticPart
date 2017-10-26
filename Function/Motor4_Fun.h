/****************************************file start****************************************************/
#ifndef	MOTOR4_F_H
#define MOTOR4_F_H

#include	"Motor_Data.h"

#if(Motor4Type == Motor4UsartMotor)
	MyRes motor4Reset(void);
	unsigned short readMotorLocation(void);
#elif(Motor4Type == Motor4IOMotor)
	void motor4MoveStep(bool isFront, unsigned short stepNum, bool isWait);
#endif

void motor4MoveTo(unsigned short location, bool isWait);
#endif

/****************************************end of file************************************************/
