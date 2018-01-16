/****************************************file start****************************************************/
#ifndef _QRCODE_E_H
#define	_QRCODE_E_H

#include	"ItemConstData.h"
#include	"DateTime.h"

#define	PiHaoLen	15
#define	PiNumLen	10

#pragma pack(1)
typedef struct QRCode_Tag
{
	unsigned short	ItemLocation;								//T��λ��
	unsigned char 	ChannelNum;									//ͨ����(0-7)
	float			ItemFenDuan[2];									//�ֶη�߱�
	float			ItemBiaoQu[3][4];								//��׼����
	unsigned char 	qu1Ise;										//����1�Ƿ���ָ��
	unsigned char 	qu2Ise;										//����2�Ƿ���ָ��
	unsigned char 	qu3Ise;										//����3�Ƿ���ָ��
	unsigned short 	CardWaitTime;								//��⿨��Ӧʱ��
	unsigned short 	CLineLocation;								//c��λ��
	char			PiHao[PiHaoLen];							//���κ�
	char			piNum[PiNumLen];							//���ڱ��
	DateTime		CardBaoZhiQi;								//������
	ItemConstData 	itemConstData;								//����Ŀ�Ĺ̶�����
	unsigned char qrVersion;									//qr version
	unsigned char calMode;										//calculate result mode(1:T/C, 2:T/T+C)
	unsigned short 	CRC16;										//crc
}QRCode;
#pragma pack()

#define	QRCodeStructSize	sizeof(QRCode)

#endif

/****************************************end of file************************************************/
