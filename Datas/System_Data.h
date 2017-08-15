/****************************************file start****************************************************/
#ifndef	SYSTEM_D_H
#define	SYSTEM_D_H

#include	"DateTime.h"

#pragma pack(1)
typedef struct
{
	DateTime systemDateTime;
}SystemData;
#pragma pack()


const SystemData * getSystemRunTimeData(void);
	
#endif

/****************************************end of file************************************************/

