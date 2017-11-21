#ifndef __NETSET_P_H__
#define __NETSET_P_H__

#include	"Define.h"
#include	"SystemSet_Data.h"
#include	"UI_Data.h"

typedef struct NetSetPageBuffer_tag {
	WireNetSet wireNetSet;
	SystemSetData systemSetData;
	char buf[100];
	unsigned short lcdinput[50];
}NetSetPageBuffer;


MyRes createNetSetActivity(Activity * thizActivity, Intent * pram);

#endif

