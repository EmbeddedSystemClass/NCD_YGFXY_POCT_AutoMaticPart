/****************************************file start****************************************************/
#ifndef _HTTPBUF_E_H
#define	_HTTPBUF_E_H

#include	"System_Data.h"
#include	"SystemSet_Data.h"
#include	"RemoteSoft_Data.h"
#include	"Device.h"
#include	"Define.h"
#include	"TestData.h"

#define		HttpSendBufSize		4096
#define		HttpRecvBufSize		2048

#pragma pack(1)
typedef struct HttpBuf_Tag
{
	Device * device;
	Page page;
	DeviceRecordHeader deviceRecordHeader;
	TestData * testDataPoint;
	RemoteSoftInfo * remoteSoftInfo;
	char sendBuf[HttpSendBufSize];						//发送数据缓冲区
	unsigned int sendDataLen;							//发送数据长度
	char recvBuf[HttpRecvBufSize];						//发送数据缓冲区
	unsigned int recvDataLen;							//发送数据长度
	unsigned int fileLength;							//http协议头中表明的下载文件的长度
	char tempBuf[500];
	bool isPost;
	char * tempP;
	char * tempP2;
	unsigned short i;
	unsigned short j;
	unsigned int tempInt;
	unsigned short tempShort;
}HttpBuf;
#pragma pack()

#define	HttpBufStructSize	sizeof(HttpBuf)

#endif

/****************************************end of file************************************************/

