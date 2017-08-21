/****************************************file start****************************************************/
#ifndef _MYLOCK_H
#define	_MYLOCK_H

#include	"Define.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#pragma pack(1)
typedef struct MyLock_Tag
{
	void * ownerId;							//ӵ����id��0 -- û��ӵ���߱���û������ 1-255��Ч
}MyLock;
#pragma pack()

MyRes LockObject(MyLock * myLock, void * ownerId, unsigned char cnt);
MyRes UnLockObject(MyLock * myLock, void * ownerId);

#endif

/****************************************end of file***************************************************/
