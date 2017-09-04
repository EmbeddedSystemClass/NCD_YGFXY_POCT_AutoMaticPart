/****************************************file start****************************************************/
#ifndef	SYSTEM_D_H
#define	SYSTEM_D_H

#include	"DateTime.h"
#include	"NetWork.h"

#pragma pack(1)
typedef struct
{
	DateTime systemDateTime;
	WireNetInfo wireNetInfo;
}SystemData;
#pragma pack()


const SystemData * getSystemRunTimeData(void);
void setSystemTime(DateTime * dateTime);
DateTime * getSystemTime(DateTime * dateTime);
void setSystemWireIP(unsigned int ip);
void setSystemWireMac(unsigned char * mac);
void setSystemWireLinkStatus(LinkStatus linkStatus);

#endif

/****************************************end of file************************************************/

