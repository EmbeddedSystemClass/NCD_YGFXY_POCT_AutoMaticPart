#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"

/*V1.0.03*/
#define	GB_SoftVersion	(unsigned short)1003
#define	GB_SoftVersion_Build	"Build17112102\0"

/*·þÎñÆ÷ÐÅÏ¢*/
#define	NCD_ServerIp_1		116
#define	NCD_ServerIp_2		62
#define	NCD_ServerIp_3		108
#define	NCD_ServerIp_4		201
#define NCD_ServerPort		8080


#define	Motor4IOMotor					0x90							//IO¿Ú¿ØÖÆµÄ×¦×Ó
#define	Motor4UsartMotor				0x91							//´®¿Ú¿ØÖÆµÄ×¦×Ó
#define	Motor4Type						Motor4IOMotor

#define	DEVICE_EN						100
#define	DEVICE_CN						101
#define	DeviceLanguage					DEVICE_CN

#define	Device_Final					0x27
#define	Device_Demo						0x28
#define	DeviceUseType					Device_Final

#define	NCDServerFinal					0x88
#define	NCDServerDebug					0x89
#define	NCDServerType					NCDServerFinal

#define	PaiDuiWeiNum					9							//ÅÅ¶ÓÎ»ÖÃÊýÄ¿
#define	MaxQualityCount					PaiDuiWeiNum

#define	MaxUpLoadTestDataNum			1							//²âÊÔÊý¾ÝÒ»´ÎÉÏ´«ÊýÄ¿£¬must less than PageContentItemNum
/**********************************************************************************************************/
/******************************************²Ù×÷½á¹û±äÁ¿*************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	My_Pass = pdPASS,			//²Ù×÷³É¹¦
	My_Fail = pdFAIL			//²Ù×÷Ê§°Ü
}MyRes;

typedef enum
{ 
	true = 1,
	false = 0
}bool;

typedef enum
{ 
	Bool_Null = 0,
	Bool_True = 1,
	Bool_False = 2
}MyBool;

typedef enum
{ 
	ON = 1,
	OFF = 0
}MyBitAction;

/**********************************************************************************************************/
/******************************************fatfs½á¹¹Ìå¶¨Òå*************************************************/
/**********************************************************************************************************/
/*´Ë½á¹¹ÌåÓÃÓÚÔÚ½øÐÐsd²Ù×÷ÊÇÉêÇëÄÚ´æ*/
typedef struct
{
	FIL file;
	UINT br;
	UINT bw;
	FRESULT res;
	FSIZE_t size;
	unsigned int tempValue1;
	unsigned short i;
	unsigned char *tempPoint;
}FatfsFileInfo_Def; 

#define	MyFileStructSize sizeof(FatfsFileInfo_Def)
/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	statusNull = 0,									//Ã»¿ªÊ¼,Ä¬ÈÏ×´Ì¬
	statusNone = 19,									//Ã»¿ªÊ¼,Ä¬ÈÏ×´Ì¬
	statusMotorMoveQR = 1,							//µç»úÕýÔÚÔËÐÐµ½µÈ´ý²å¿¨¿Ú
	statusWaitCardPutIn = 2,						//µÈ´ý²å¿¨
	statusWaitCardPutOut = 4,						//µÈ´ý²å¿¨
	statusWaitScanQR = 3,							//µÈ´ýÉ¨Ãè¶þÎ¬Âë½á¹û
	statusMotorPutCardDown = 6,						//¶ÁÈ¡¶þÎ¬Âë³É¹¦ºó£¬µÈ´ýµç»úÔË¶¯½«¿¨·ÅÔÚÅÅ¶ÓÎ»
	statusMotorOrigin = 7,							//µÈ´ýµç»ú»ØÔ­µã
	status_start = 5,								//Æô¶¯ÅÅ¶Ó
	status_timedown = 10,							//µ¹¼ÆÊ±×´Ì¬
	status_timeup = 15,								//³¬Ê±×´Ì¬
	status_testting = 16,							//¿ªÊ¼²âÊÔ
	statusPrepareTest = 18,							//½øÈë²âÊÔ×´Ì¬£¬µ¹¼ÆÊ±Ê±¼äÐ¡ÓÚ10s£¬ÇÐ»»µ½µ¹¼ÆÊ±½çÃæµÈ´ý
	statusTestMotor = 20,							//µÈ´ýµç»ú×ßµ½²âÊÔÎ»ÖÃ
	statusPutCardOut = 21,							//½«¿¨ÅÅ³ýÉè±¸
	statusWaitCardOut = 22,							//µÈ´ý¿¨ÅÅ³ýÉè±¸
	status_end = 17,
}MyPaiDuiStatues;

#define	StartTestTime								20					//½ûÖ¹´´½¨²âÊÔµÄ×îÐ¡Ê±¼ä
#define	DisableCreateNewTestTime					30					//½ûÖ¹´´½¨²âÊÔµÄ×îÐ¡Ê±¼ä
#define	DisableReturnTime							40					//½ûÖ¹·µ»Ø£¬¼´½«²âÊÔÊ±¼ä
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************¶þÎ¬ÂëÏà¹Ø¶¨Òå**************************************************/
/**********************************************************************************************************/


typedef enum
{
	CardScanNone = 0,
	
	CardCodeScanOK = 1,												//É¨Ãè¶þÎ¬Âë³É¹¦
	CardCodeScanFail = 2,											//É¨ÃèÊ§°Ü
	CardCodeTimeOut = 3,											//¹ýÆÚ
	CardCodeCardOut = 4,											//¿¨±»°Î³ö
	CardCodeScanTimeOut = 5,										//É¨Ãè³¬Ê±
	CardCodeCRCError = 6,											//crc´íÎó
	CardUnsupported = 7,											//µ±Ç°³ÌÐò²»Ö§³Ö
	CardCodeScanning = 8,											//ÕýÔÚÉ¨Ãè
}ScanCodeResult;


/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************ÑùÆ·IDÏà¹Ø¶¨Òå**************************************************/
/**********************************************************************************************************/
#define	MaxSampleIDLen	20											//×îóÖ§³ÖÑùÆ·id³¤¶ÈÎª20
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************½çÃæÏà¹Ø¶¨Òå****************************************************/
/***************************************************************************´*******************************/

typedef struct LineInfo_Tag
{
	unsigned short MaxData;				//ÊµÊ±ÇúÏß×î´óµÄÊýÖµ
	unsigned short Y_Scale;				//ÇúÏßyÖá×îµÍµÄ¿Ì¶È
	unsigned short MUL_Y;				//YÖá·Å´ó±¶Êý
}LineInfo;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************ÆÁÄ»Ïà¹Ø¶¨Òå****************************************************/
/**********************************************************************************************************/
#define	LCD_Head_1		0xAA											//Ð­ÒéÍ·1
#define	LCD_Head_2		0xBB											//Ð­ÒéÍ·2

#define W_REGSITER 		0x80          									//Ð´ÈëÊý¾Ý,ÎÞÓ¦´ð          
#define R_REGSITER 		0x81          									//¶ÁÈ¡Êý¾Ý£¬ÓÐÓ¦´ð
#define W_ADDRESS  		0x82											//Ð´±äÁ¿¼Ä´æÆ÷£¬ÎÞÓ¦´ð
#define R_ADDRESS  		0x83											//¶Á±äÁ¿¼Ä´æÆ÷Êý¾Ý£¬ÓÐÓ¦´ð
#define W_LINE     		0x84          									//Ð´ÇúÏß»º³åÇø£¬ÎÞÓ¦´ð

#pragma pack(1)
typedef struct BasicICO_Tag
{
	unsigned short X;
	unsigned short Y;
	unsigned short ICO_ID;
}Basic_ICO;
#pragma pack()

#define	BasicIcoStructSize		sizeof(Basic_ICO)
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************×Ô¼ì¹¦ÄÜÏà¹Ø¶¨Òå****************************************************/
/**********************************************************************************************************/



typedef enum
{
	SelfCheck_None = 0,													//×Ô¼ìÎ´¿ªÊ¼
	SelfChecking = 1,
	SelfCheck_OK = 2,													//×Ô¼ìÕý³£
	SelfCheck_Error = 3,												//×Ô¼ìÓÐ²»¿ÉºöÂÔµÄ´íÎó
}SelfCheckStatus;
/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	DESC = 0,												//ÕýÐò
	ASC = 1													//ÄæÐò
}OrderByEnum;

//¶ÁÈ¡ÇëÇóÐÅÏ¢
#pragma pack(1)
typedef struct PageRequest_tag {
	unsigned int pageIndex;											//ÆðÊ¼¶ÁÈ¡Ë÷Òý£¬0ÎªµÚÒ»¸ö
	unsigned char pageSize;													//Ã¿Ò³µÄÊýÄ¿
	OrderByEnum orderType;													//ÅÅÐò·½Ê½
}PageRequest;
#pragma pack()

#define	PageRequestStructSize		sizeof(PageRequest)
#define	PageRequestStructCrcSize	PageRequestStructSize - 2

#pragma pack(1)
typedef struct
{
	unsigned int itemSize;
	unsigned int userUpLoadIndex;
	unsigned int ncdUpLoadIndex;
	unsigned char parm[20];								//not used
	unsigned short crc;
}DeviceRecordHeader;
#pragma pack()

#define	DeviceRecordHeaderStructSize		sizeof(DeviceRecordHeader)								//×î¶à±£´æµÄÓÃ»§ÊýÄ¿
#define	DeviceRecordHeaderStructCrcSize		DeviceRecordHeaderStructSize - 2						//×î¶à±£´æµÄÓÃ»§ÊýÄ¿
#define	PageContentItemNum					10														//Ò»´Î×î¶à¶ÁÈ¡µÄÊýÄ¿
#pragma pack(1)
typedef struct
{
	unsigned int totalPageSize;																		//×ÜÒ³Êý
	unsigned int totalItemSize;																		//×ÜÊý
	unsigned int readItemSize;																		//¶Áµ½µÄÊýÄ¿
	bool isForNCD;																					//true¶ÁÈ¡Å¦¿µ¶ÈÊý¾Ý£¬false¶ÁÈ¡ÓÃ»§Êý¾Ý
	void * content[PageContentItemNum];																//Êý¾Ý
}Page;
#pragma pack()

#define	PageStructSize		sizeof(Page)

/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	NoResult = 0,										//»¹Î´²âÊÔ£¬½á¹ûÎ´null
	NoSample = 1,										//Î´¼ÓÑù
	PeakError = 2,										//¼ì²â¿¨Ã»ÓÐC·å
	MemError = 3,										//ÄÚ´æ´íÎó
	TestInterrupt = 8,									//²âÊÔÖÐ¶Ï
	ResultIsOK = 99,									//²âÊÔ³É¹¦
}ResultState;

#pragma pack(1)
typedef struct Point_tag {
	unsigned short y;
	unsigned short x;
}Point;
#pragma pack()

/**********************************************************************************************************/
/**********************************************************************************************************/
#pragma pack(1)
typedef struct TempDataBuffer_tag {
	unsigned char i;
	unsigned short j;
	unsigned int tempInt;
	double tempDouble;
	unsigned short shortData[1024];
	unsigned char * tempCharP;
	unsigned short * tempShortP;
}TempDataBuffer;
#pragma pack()
#define	TempDataBufferStructSize	sizeof(TempDataBuffer)

#endif

