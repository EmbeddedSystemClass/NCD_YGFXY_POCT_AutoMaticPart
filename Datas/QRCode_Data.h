/****************************************file start****************************************************/
#ifndef QRCODE_D_H
#define QRCODE_D_H

#include	"Define.h"
#include	"QRCode.h"

QRCode * getGB_QRCode(void);
void resetGB_QRCode(void);
bool isReadingQRCode(void);
void setReadQRCodeStatus(bool status);
	
#endif

/****************************************end of file************************************************/
