/****************************************file start****************************************************/
#ifndef	MOTOR2_F_H
#define MOTOR2_F_H

#include	"Motor_Data.h"

void motor2MoveTo(unsigned char highTime, unsigned char lowTime, unsigned short location, bool isWait);
void motor2MoveStep(bool isFront, unsigned short stepNum, bool isWait);
void motor2StopMove(void);
#endif

/****************************************end of file************************************************/
