/****************************************file start****************************************************/
#ifndef _REMOTESOFTINFO_E_H
#define	_REMOTESOFTINFO_E_H

#include	"RemoteSoftInfo.h"
#include	"Define.h"

#pragma pack(1)
typedef struct
{
	unsigned short RemoteFirmwareVersion;
	char md5[32];
	char desc[200];
	bool isSuccessDownloadFirmware;
}RemoteSoftInfo;
#pragma pack()

#define	RemoteSoftInfoStructSize		sizeof(RemoteSoftInfo)							//��ౣ����û���Ŀ
#define	RemoteSoftInfoStructCrcSize		RemoteSoftInfoStructSize - 2						//��ౣ����û���Ŀ

#endif

/****************************************end of file************************************************/
