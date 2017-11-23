/***************************************************************************************************
*FileName��LCD_Driver
*Description��Һ������һЩ���ܺ���
*Author��xsx
*Data��2016��4��29��10:13:01
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"LCD_Driver.h"
#include	"QueueUnits.h"
#include	"System_Data.h"

#include	"CRC16.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"stdio.h"
#include	"string.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
//static unsigned char *txdat = NULL;
//static unsigned char tempbuf[200];
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static void WriteLCDRegister(unsigned char reg, void *data, unsigned char len);
static void ReadLCDRegister(unsigned char reg, unsigned char len);
static void WriteLCDData(unsigned short addr, void *data, unsigned char len);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName: WriteRegister
*Description: д�Ĵ���
*Input: reg -- �Ĵ�����ַ
*		data -- д������
*		datalen -- д�����ݵĳ���
*Output: ��
*Author: xsx
*Date: 2016��8��5��15:18:01
***************************************************************************************************/
static void WriteLCDRegister(unsigned char reg, void *data, unsigned char len)
{			
	unsigned char *q = NULL;
	unsigned char *p = (unsigned char *)data;
	unsigned char i=0;
	unsigned char *txdat = NULL;
	
	txdat = MyMalloc(len + 10);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, len + 10);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = len+4;
	
	*q++ = W_REGSITER;
	
	*q++ = reg;

	for(i=0; i<len; i++)
		*q++ = *p++;
	
	CalModbusCRC16Fun(txdat+3, len + 2, q);
	
	SendDataToQueue(GetUsart6TXQueue(), GetUsart6TxMutex(), txdat, txdat[2]+3, 1, 100 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart6TXInterrupt);
	
	MyFree(txdat);
}

/***************************************************************************************************
*FunctionName: ReadLCDRegister
*Description: ��ȡ��Ļ�Ĵ���ֵ
*Input: reg -- �Ĵ�����ַ
*		len -- ��ȡ����
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��12��11:01:48
***************************************************************************************************/
static void ReadLCDRegister(unsigned char reg, unsigned char len)
{			
	unsigned char *q = NULL;
	unsigned char *txdat = NULL;
	
	txdat = MyMalloc(16);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, 16);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = 1 + 4;
	
	*q++ = R_REGSITER;
	
	*q++ = reg;

	*q++ = len;
	
	CalModbusCRC16Fun(txdat+3, 1 + 2, q);
	
	SendDataToQueue(GetUsart6TXQueue(), GetUsart6TxMutex(), txdat, txdat[2]+3, 1, 100 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart6TXInterrupt);
	
	MyFree(txdat);
}

static void WriteLCDData(unsigned short addr, void *data, unsigned char len)
{
	unsigned char *q = NULL;
	unsigned char *p = (unsigned char *)data;
	unsigned char i=0;
	unsigned char *txdat = NULL;
	
	txdat = MyMalloc(len + 10);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, len + 10);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = len+5;

	*q++ = W_ADDRESS;
	
	*q++ = addr>>8;
	*q++ = addr;

	for(i=0; i<len; i++)
		*q++ = *p++;
	
	CalModbusCRC16Fun(txdat+3, len + 3, q);
	
	SendDataToQueue(GetUsart6TXQueue(), GetUsart6TxMutex(), txdat, txdat[2]+3, 1, 100 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart6TXInterrupt);

	MyFree(txdat);
}

/***************************************************************************************************
*FunctionName��SelectPage
*Description��ѡ����ʾ��ͼƬid
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��30��17:02:39
***************************************************************************************************/
void SelectPage(unsigned short index)
{
	unsigned char tempbuf[10];
	tempbuf[0] = index >> 8;
	tempbuf[1] = index;
	WriteLCDRegister(0x03, tempbuf, 2);
}

/***************************************************************************************************
*FunctionName: ReadCurrentPageId
*Description: ��ȡ��ǰҳ��id
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��12��11:02:58
***************************************************************************************************/
void ReadCurrentPageId(void)
{
	ReadLCDRegister(0x03, 2);
}

/***************************************************************************************************
*FunctionName: ReadLcdSoftVersion
*Description: ��ȡ��Ļ������汾
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��19��14:40:12
***************************************************************************************************/
void ReadLcdSoftVersion(void)
{
	ReadLCDRegister(0x00, 1);
}

/***************************************************************************************************
*FunctionName��SendKeyCode
*Description�����ع���
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��1��16:46:15
***************************************************************************************************/
void SendKeyCode(unsigned char keycode)
{
	WriteLCDRegister(0x4f, &keycode, 1);
}

void ClearLine(unsigned char Command)
{
	WriteLCDRegister(0xEB, &Command, 1);
}

/************************************************************************
** ������:SetLEDLight
** ��  ��:num -- ��Ļ���ȣ�1 -- 1%�� 100--100%
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
void SetLEDLight(unsigned char num)
{
	WriteLCDRegister(0x01, &num, 1);
}
/***************************************************************************************************
*FunctionName��DisText
*Description����ʾ�ı�
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��30��17:02:53
***************************************************************************************************/
void DisText(unsigned short addr, void *data, unsigned short len)
{
	WriteLCDData(addr, data, len);
}

void ClearText(unsigned short addr)
{	
	unsigned char tempbuf[20];
	memset(tempbuf, 0x0, 10);
	DisText(addr, tempbuf, 1);
}

void WriteRadioData(unsigned short addr, void *data, unsigned short len)
{
	WriteLCDData(addr, data, len);
}

void BasicUI(unsigned short addr , unsigned short cmd, unsigned short datapacknum, void *data , unsigned short len)
{
	unsigned char *p = (unsigned char *)data;
	unsigned char i=0;
	unsigned char *txdat = NULL;
	txdat = MyMalloc(128);
	if(txdat == NULL)
		return;

	memset(txdat, 0, 128);
	//����ͼ��ָ��
	txdat[0] = cmd>>8;		txdat[1] = cmd;
	//����ͼ�����ݰ�����
	txdat[2] = datapacknum>>8;		txdat[3] = datapacknum;
	
	//����keil�ǵ��ֽ���ǰ��������������������Ҫ���ֽ���ǰ�����������ｫ�ߵͻ���
	for(i=0; i<len/2; i++)
	{
		txdat[4+i*2] = p[i*2+1];
		txdat[5+i*2] = p[i*2];
	}
	
	WriteLCDData(addr, txdat, len+4);
	
	MyFree(txdat);
}

/***************************************************************************************************
*FunctionName��BasicPic
*Description������ͼ��
*Input��addr -- �ؼ���ַ   �� datanum -- ���ݰ�����  �� soureid -- ͼƬԴҳ��id��  
*Output��None
*Author��xsx
*Data��
***************************************************************************************************/
void BasicPic(unsigned short addr,unsigned short datanum, unsigned short soureid ,unsigned short xs,unsigned short ys ,unsigned short xe,unsigned short ye ,
	unsigned short txs,unsigned short tys)
{	
	unsigned char *txdat = NULL;
	txdat = MyMalloc(128);
	if(txdat == NULL)
		return;
	memset(txdat, 0, 128);
	
	//cmd 0x0006
	txdat[0] = 0;		txdat[1] = 6;
		
	//���ݰ�����1
	txdat[2] = datanum>>8;		txdat[3] = datanum;
		
	//ԭҳ��id
	txdat[4] = soureid>>8;	txdat[5] = soureid;
	//ԭҳ��x
	txdat[6] = xs>>8;	txdat[7] = xs;
	//ԭҳ��x
	txdat[8] = ys>>8;	txdat[9] = ys;
	//ԭҳ��x
	txdat[10] = xe>>8;	txdat[11] = xe;
	//ԭҳ��x
	txdat[12] = ye>>8;	txdat[13] = ye;
	//ԭҳ��x
	txdat[14] = txs>>8;	txdat[15] = txs;
	//ԭҳ��x
	txdat[16] = tys>>8;	txdat[17] = tys;

	WriteLCDData(addr, txdat, 18);
	
	MyFree(txdat);
}



/************************************************************************
** ������:GetBufLen
** ��  ��:���������ص��ַ�����0xff��β,��0xff����0x00��������㳤��
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
unsigned short GetBufLen(unsigned char *p ,unsigned short len)
{
	unsigned short i ;
	for(i=0; i<len; i++)
			{
				if(p[i] == 0xff || p[i] == 0)
				{
					p[i] = 0;
					return i;
				}
			}
		return 0;
}

void getLcdInputData(void * myBuffer, void * lcdData)
{
	unsigned short len = 1000;
	unsigned char * p = (unsigned char *)myBuffer;
	unsigned char * q = (unsigned char *)lcdData;
	
	while(len--)
	{
		if(*q == 0xff)
		{
			*p++ = 0;
			break;
		}
		else
			*p++ = *q++;
	}
}

/************************************************************************
** ������:
** ��  ��:
** ��  ��:pram----����
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
void DisPlayLine(unsigned char channel , void * data , unsigned char datalen)
{
	unsigned short * p = (unsigned short *)data;
				
	unsigned char *q = NULL;
	unsigned char i = 0;
	unsigned short tempdat = 0;
	unsigned char *txdat = NULL;
	
	txdat = MyMalloc(datalen*2 + 50);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, datalen*2 + 50);
	q = txdat;

	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = (2+datalen*2+2);
	*q++ = W_LINE;
	*q++ = (0x01 << channel);
		
	for(i=0; i<datalen; i++)
	{
		tempdat = *p++;
		*q++ = (unsigned char)(tempdat>>8);
		*q++ = (unsigned char)tempdat;
	}
	
	CalModbusCRC16Fun(txdat+3, datalen*2 + 2, q);
	
	SendDataToQueue(GetUsart6TXQueue(), GetUsart6TxMutex(), txdat, txdat[2]+3, 1, 50 * portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart6TXInterrupt);

	MyFree(txdat);
}

void SetChartSize(unsigned short add , unsigned short num)
{
	unsigned char tempbuf[10];
	tempbuf[0] = num>>8;
	tempbuf[1] = num;
	
	WriteLCDData(add+0x08, tempbuf, 2);
}

void DspNum(unsigned short addr , unsigned int num, unsigned char len)
{
	unsigned char i=0;
	unsigned char tempbuf[128];
	
	for(i=0; i<len; i++)
	{
		tempbuf[i] = (num >> ((len-i-1)*8));
	}

	WriteLCDData(addr, tempbuf, len);
}

void WriteVarIcoNum(unsigned short addr, unsigned short num)
{
	unsigned char tempbuf[6];
	tempbuf[0] = (unsigned char)(num >> 8);
	tempbuf[1] = (unsigned char)num ;

	WriteLCDData(addr, tempbuf, 2);
}
	
void unLockLCDOneTime(void)
{
	WriteLCDData(0x6000, "NEWcando2013\0", 12);
}

void writeDataToLcd(unsigned short addr, void * data, unsigned char len)
{
	WriteLCDData(addr, data, len);
}
