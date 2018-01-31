/****************************************file start****************************************************/
#ifndef _HTTPBUF_E_H
#define	_HTTPBUF_E_H

#include	"System_Data.h"
#include	"SystemSet_Data.h"
#include	"RemoteSoft_Data.h"
#include	"Device.h"
#include	"Define.h"
#include	"TestData.h"

#include	"tcp.h"
#include 	"api.h" 

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
	ServerSet serverSet;
	char sendBuf[HttpSendBufSize];						//发送数据缓冲区
	unsigned int sendDataLen;							//发送数据长度
	char recvBuf[HttpRecvBufSize];						//发送数据缓冲区
	unsigned int recvDataLen;							//发送数据长度
	unsigned int fileLength;							//http协议头中表明的下载文件的长度
	char tempBuf[500];
	bool isPost;
	bool isNcdDataToUpload;
	bool isNewVersionToBeDown;
	char * tempP;
	char * tempP2;
	unsigned short i;
	unsigned short j;
	unsigned int tempInt;
	unsigned short tempShort;
	MyRes status;
	
	struct ip_addr server_ipaddr;				//服务器ip
	struct netconn *clientconn;				//当前客户端
	struct netbuf *tcpRecvBuf;						//接收缓冲区
	struct pbuf *q;								//接收数据的链表
}HttpBuf;
#pragma pack()

#define	HttpBufStructSize	sizeof(HttpBuf)

#endif

/****************************************end of file************************************************/

