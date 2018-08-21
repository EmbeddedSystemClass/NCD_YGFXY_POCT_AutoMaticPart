/****************************************file start****************************************************/
#ifndef	PRINTF_F_H
#define	PRINTF_F_H

#include	"Define.h"
#include	"TestData.h"

typedef struct PrintfBuf_Tag
{
	TestData testData;
	DateTime dateTime;
    PrintInfo printInfo;
	char buf[100];
}PrintfBuf;

void PrintfData(TestData * testd);

#endif

/****************************************end of file************************************************/
