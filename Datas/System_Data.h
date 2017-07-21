/****************************************file start****************************************************/
#ifndef	SYSTEM_D_H
#define	SYSTEM_D_H

#pragma pack(1)
typedef struct
{
	unsigned char temp;
}SystemData;
#pragma pack()


const SystemData * getSystemRunTimeData(void);
	
#endif

/****************************************end of file************************************************/

