/****************************************file start****************************************************/
#ifndef _HTTPBUF_E_H
#define	_HTTPBUF_E_H

#include	"System_Data.h"
#include	"SystemSet_Data.h"
#include	"RemoteSoft_Data.h"
#include	"Device.h"
#include	"Define.h"
#include	"TestData.h"

#pragma pack(1)
typedef struct HttpBuf_Tag
{
	char deviceId[DeviceIdLen];
	SystemData systemData;
	Device device;
	Page page;
	DeviceRecordHeader deviceRecordHeader;
	TestData * testDataPoint;
	RemoteSoftInfo remoteSoftInfo;
	char sendBuf[4096];						//�������ݻ�����
	unsigned int sendDataLen;					//�������ݳ���
	char recvBuf[2048];						//�������ݻ�����
	unsigned int recvDataLen;					//�������ݳ���
	char tempBuf2[100];
	bool isPost;
	char * tempP;
	char * tempP2;
	unsigned short i;
	unsigned short j;
	unsigned int tempInt;
}HttpBuf;
#pragma pack()

#define	HttpBufStructSize	sizeof(HttpBuf)

#endif

/****************************************end of file************************************************/

