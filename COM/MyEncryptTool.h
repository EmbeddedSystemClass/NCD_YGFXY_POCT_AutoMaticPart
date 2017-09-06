#ifndef __ENCRYPT_T_H__
#define __ENCRYPT_T_H__

#include	"Define.h"


typedef struct PointBuffer_Tag
{
	unsigned short i;
	unsigned short j;
	unsigned short tempIndex;
	unsigned short tempV1;
	unsigned short tempV2;
}PointBuffer;

#define	PointBufferStructSize	sizeof(PointBuffer)

MyRes MyDencrypt(char *source, char *target, unsigned short len);

#endif

