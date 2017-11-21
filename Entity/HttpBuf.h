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
	char sendBuf[HttpSendBufSize];						//�������ݻ�����
	unsigned int sendDataLen;							//�������ݳ���
	char recvBuf[HttpRecvBufSize];						//�������ݻ�����
	unsigned int recvDataLen;							//�������ݳ���
	unsigned int fileLength;							//httpЭ��ͷ�б����������ļ��ĳ���
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

