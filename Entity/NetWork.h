/****************************************file start****************************************************/
#ifndef _NETWORK_E_H
#define	_NETWORK_E_H

#include	"Define.h"

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
	IP ip;												//������ip
	unsigned char LineMAC[6];							//������MAC
	LinkStatus lineStatus;							//����������״̬��0 -- �����磬 1 -- ������
}WireNetInfo; 
#pragma pack()

#pragma pack(1)
typedef struct
{
	IP staticIP;														//��̬IP
	IPMode ipMode;														//ip��ȡ��ʽ
}WireNetSet; 
#pragma pack()

#pragma pack(1)
typedef struct
{
	IP serverIP;														//������IP
	unsigned short serverPort;											//�������˿ں�
}ServerSet; 
#pragma pack()

#define	ServerSetStructSize		sizeof(ServerSet)

#endif

/****************************************end of file************************************************/
