#ifndef __CHECKQR_P_H__
#define __CHECKQR_P_H__

#include	"UI_Data.h"
#include	"QRCode.h"

typedef struct CheckQRPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[100];
	QRCode qrCode;						//��ά�����ݻ�����
	ScanCodeResult scancode;
	Timer timer;						//�����ʾ�忨
	bool isScanning;				//�Ƿ�����ɨ����
	double inputTC;
	double basicResult;				//������
}CheckQRPageBuffer;


MyRes createCheckQRActivity(Activity * thizActivity, Intent * pram);

#endif

