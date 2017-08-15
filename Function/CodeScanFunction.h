#ifndef __RCODE_F_H__
#define __RCODE_F_H__

#include	"QRCode.h"
#include	"Define.h"
#include	"Timer.h"

#define	MAX_QR_CODE_LENGHT	300					//��ά����󳤶�
#define	MAX_SCAN_QR_TIME	10					//ɨ���ά��ʱ��

typedef struct ReadQRCodeBuffer_tag {
	char originalcode[MAX_QR_CODE_LENGHT+1];						//ԭʼ�Ķ�ά��
	unsigned short originalCodeLen;									//ԭʼ���ݳ���
	char decryptcode[MAX_QR_CODE_LENGHT+1];						//���ܺ�Ķ�ά��
	char tempbuf[64];											//��ʱ����
	char * pbuf1;												//��ʱָ��1
	char * pbuf2;												//��ʱָ��2
	QRCode * cardQR;											//�����ά��
	Timer timer;												//��ʱ��
	unsigned short motorLocation;								//���λ��
	unsigned char motorDir;										//�������
	unsigned char tempV1;
}ReadQRCodeBuffer;

void ScanCodeFun(void);

#endif

