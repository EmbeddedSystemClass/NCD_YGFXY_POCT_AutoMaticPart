#ifndef LCD_D_H_H
#define LCD_D_H_H

#include	"Usart6_Driver.h"
#include	"DateTime.h"

#define	MaxSendDataLen		xTxQueue6_Len-20							//������Ļ��������󳤶�

void SelectPage(unsigned short index);
void ReadCurrentPageId(void);
void ReadLcdSoftVersion(void);
void DisText(unsigned short addr, void *data, unsigned short len);
void ClearText(unsigned short addr);
void WriteRadioData(unsigned short addr, void *data, unsigned short len);
void BasicPic(unsigned short addr,unsigned short datanum, unsigned short soureid ,unsigned short xs,unsigned short ys ,unsigned short xe,unsigned short ye ,
	unsigned short txs,unsigned short tys);
void SendKeyCode(unsigned char keycode);
unsigned short GetBufLen(unsigned char *p ,unsigned short len);
void DspTimeAndTempData(void);
void DisPlayLine(unsigned char channel , void * data , unsigned char datalen);
void SetChartSize(unsigned short add , unsigned short num);
void DspNum(unsigned short addr , unsigned int num, unsigned char len);
void ClearLine(unsigned char Command);
void BasicUI(unsigned short addr , unsigned short cmd, unsigned short datapacknum, void *data , unsigned short len);
void SetLEDLight(unsigned char num);
void WriteVarIcoNum(unsigned short addr, unsigned short num);
void unLockLCDOneTime(void);
void writeDataToLcd(unsigned short addr, void * data, unsigned char len);
void getLcdInputData(void * myBuffer, void * lcdData);

#endif


