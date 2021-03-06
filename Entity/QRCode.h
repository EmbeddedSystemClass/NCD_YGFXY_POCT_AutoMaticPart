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
	unsigned short	ItemLocation;								//T线位置
	unsigned char 	ChannelNum;									//通道号(0-7)
	float			ItemFenDuan[2];									//分段峰高比
	float			ItemBiaoQu[3][4];								//标准曲线
	unsigned char 	qu1Ise;										//曲线1是否是指数
	unsigned char 	qu2Ise;										//曲线2是否是指数
	unsigned char 	qu3Ise;										//曲线3是否是指数
	unsigned short 	CardWaitTime;								//检测卡反应时间
	unsigned short 	CLineLocation;								//c线位置
	char			PiHao[PiHaoLen];							//批次号
	char			piNum[PiNumLen];							//批内编号
	DateTime		CardBaoZhiQi;								//保质期
	ItemConstData 	itemConstData;								//此项目的固定数据
	unsigned char qrVersion;									//qr version
	unsigned char calMode;										//calculate result mode(1:T/C, 2:T/T+C)
	unsigned short 	CRC16;										//crc
}QRCode;
#pragma pack()

#define	QRCodeStructSize	sizeof(QRCode)

#endif

/****************************************end of file************************************************/
