#ifndef __UNIVERSAL_F_H__
#define __UNIVERSAL_F_H__

#include	"Define.h"
#include	"System_Data.h"
#include	"SystemSet_Data.h"
#include	"Operator.h"
#include	"Device.h"
#include	"StringDefine.h"

typedef struct SystemResetBuf_Tag
{
	unsigned char buffer[4096];
	SystemSetData * systemSetData;
	Device * device;
	unsigned short tempShort;
}SystemResetBuf;

#define	SystemResetBufStructSize	sizeof(SystemResetBuf)

void readAndUpdateSystemDateTimeFun(void);
MyRes SystemFactoryReset(void);
void checkCanRestartForUpgrade(void);

#endif

