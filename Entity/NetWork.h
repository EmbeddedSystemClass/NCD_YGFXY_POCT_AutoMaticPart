/****************************************file start****************************************************/
#ifndef _NETWORK_E_H
#define	_NETWORK_E_H

#include	"Define.h"

typedef enum
{ 
	Link_Down = 0,
	Link_Up = 1
}LinkStatus;



/*ip获取方式*/
typedef enum
{
	Dynamic_IP = 1,														//自动获取动态ip
	Static_IP = 2														//用户设置ip
}IPMode;

#pragma pack(1)
typedef struct ip_Tag
{
	unsigned char ip_1;
	unsigned char ip_2;
	unsigned char ip_3;
	unsigned char ip_4;
}IP;
#pragma pack()

#pragma pack(1)
typedef struct
{
	IP ip;												//有线网ip
	unsigned char LineMAC[6];							//有线网MAC
	LinkStatus lineStatus;							//有线网链接状态，0 -- 无网络， 1 -- 有网络
}WireNetInfo; 
#pragma pack()

#pragma pack(1)
typedef struct
{
	IP staticIP;														//静态IP
	IPMode ipMode;														//ip获取方式
}WireNetSet; 
#pragma pack()

#pragma pack(1)
typedef struct
{
	IP serverIP;														//服务器IP
	unsigned short serverPort;											//服务器端口号
}ServerSet; 
#pragma pack()

#define	ServerSetStructSize		sizeof(ServerSet)

#endif

/****************************************end of file************************************************/
