#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"

/*V1.0.03*/
#define	GB_SoftVersion	(unsigned short)1006
#define	GB_SoftVersion_Build	"Build18012401\0"

//¶şÎ¬Âë¶¨Òå
#define QRVersion0Define		0								//¶şÎ¬ÂëÔ­Ê¼°æ±¾¶¨Òå
#define QRVersion2Define		'2'								//¶şÎ¬Âë°æ±¾2¶¨Òå
#define QRVersion3Define		'3'								//¶şÎ¬Âë°æ±¾3¶¨Òå
#define QRVersionUnDefine		0xff							//²»Ö§³Ö¶şÎ¬Âë°æ±¾
#define	GB_QRVersion			QRVersion3Define				//µ±Ç°Ö§³ÖµÄ¶şÎ¬Âë°æ±¾£¬ÍùÏÂ¼æÈİ

/*·şÎñÆ÷ĞÅÏ¢*/
#define	NCD_ServerIp_1		116
#define	NCD_ServerIp_2		62
#define	NCD_ServerIp_3		108
#define	NCD_ServerIp_4		201
#define NCD_ServerPort		8080


#define	Motor4IOMotor					0x90							//IO¿Ú¿ØÖÆµÄ×¦×Ó
#define	Motor4UsartMotor				0x91							//´®¿Ú¿ØÖÆµÄ×¦×Ó
#define	Motor4Type						Motor4UsartMotor

#define	DEVICE_EN						100
#define	DEVICE_CN						101
#define	DeviceLanguage					DEVICE_CN

#define	Device_Final					0x27
#define	Device_Demo						0x28
#define	Device_FastDemo					0x29
#define	DeviceUseType					Device_Final

#define	PaiDuiWeiNum					9							//ÅÅ¶ÓÎ»ÖÃÊıÄ¿
#define	MaxQualityCount					PaiDuiWeiNum

#define	MaxUpLoadTestDataNum			1							//²âÊÔÊı¾İÒ»´ÎÉÏ´«ÊıÄ¿£¬must less than PageContentItemNum
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

#define SystemTestting                 				((uint16_t)0x0001)
#define SystemDeviceInfoModifying                 	((uint16_t)0x0002)
#define SystemOperatorModifying                 	((uint16_t)0x0004)
#define SystemNetInfoModifying                 		((uint16_t)0x0008)
#define SystemLookRecord                 			((uint16_t)0x0010)
#define SystemQualityong                			((uint16_t)0x0020)
#define SystemMaintenancing                 		((uint16_t)0x0040)
#define SystemSetting                 				((uint16_t)0x0080)


#define	FreeRTOSZeroDelay	0				//0ms
#define	FreeRTOSTenDelay	10				//10ms
/**********************************************************************************************************/
/******************************************fatfs½á¹¹Ìå¶¨Òå*************************************************/
/**********************************************************************************************************/
/*´Ë½á¹¹ÌåÓÃÓÚÔÚ½øĞĞsd²Ù×÷ÊÇÉêÇëÄÚ´æ*/
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
	statusNone = 19,								//Ã»¿ªÊ¼,Ä¬ÈÏ×´Ì¬
	statusMotorMoveWaitCard = 1,					//µç»úÕıÔÚÔËĞĞµ½µÈ´ı²å¿¨¿Ú
	statusWaitCardPutIn = 2,						//µÈ´ı²å¿¨
	statusWaitCardPutOut = 4,						//µÈ´ı²å¿¨
	statusWaitScanQR = 3,							//µÈ´ıÉ¨Ãè¶şÎ¬Âë½á¹û
	statusMotorPutCardDown = 6,						//¶ÁÈ¡¶şÎ¬Âë³É¹¦ºó£¬µÈ´ıµç»úÔË¶¯½«¿¨·ÅÔÚÅÅ¶ÓÎ»
	statusMotorOrigin = 7,							//µÈ´ıµç»ú»ØÔ­µã
	status_start = 5,								//Æô¶¯ÅÅ¶Ó
	status_timedown = 10,							//µ¹¼ÆÊ±×´Ì¬
	status_timeup = 15,								//³¬Ê±×´Ì¬
	status_testting = 16,							//¿ªÊ¼²âÊÔ
	statusPrepareTest = 18,							//½øÈë²âÊÔ×´Ì¬£¬µ¹¼ÆÊ±Ê±¼äĞ¡ÓÚ10s£¬ÇĞ»»µ½µ¹¼ÆÊ±½çÃæµÈ´ı
	statusTestMotor = 20,							//µÈ´ıµç»ú×ßµ½²âÊÔÎ»ÖÃ
	statusPutCardOut = 21,							//½«¿¨ÅÅ³ıÉè±¸
	statusWaitCardOut = 22,							//µÈ´ı¿¨ÅÅ³ıÉè±¸
	statusWaitPutCardInTestPlace = 23,				//µÈ´ı¿¨ÔÚ²âÊÔ¶Ë·ÅºÃ
	statusInputSampleID = 24,						//ÕıÔÚÊäÈëÑùÆ·ID
	status_end = 17,
}MyPaiDuiStatues;

#define	StartTestTime								10					//½ûÖ¹´´½¨²âÊÔµÄ×îĞ¡Ê±¼ä
#define	DisableCreateNewTestTime					30					//½ûÖ¹´´½¨²âÊÔµÄ×îĞ¡Ê±¼ä
#define	DisableReturnTime							40					//½ûÖ¹·µ»Ø£¬¼´½«²âÊÔÊ±¼ä
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************¶şÎ¬ÂëÏà¹Ø¶¨Òå**************************************************/
/**********************************************************************************************************/


typedef enum
{
	CardScanNone = 0,
	
	CardCodeScanOK = 1,												//É¨Ãè¶şÎ¬Âë³É¹¦
	CardCodeScanFail = 2,											//É¨ÃèÊ§°Ü
	CardCodeTimeOut = 3,											//¹ıÆÚ
	CardCodeCardOut = 4,											//¿¨±»°Î³ö
	CardCodeScanTimeOut = 5,										//É¨Ãè³¬Ê±
	CardCodeCRCError = 6,											//crc´íÎó
	CardUnsupported = 7,											//µ±Ç°³ÌĞò²»Ö§³Ö
	CardCodeScanning = 8,											//ÕıÔÚÉ¨Ãè
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
	unsigned short MaxData;				//ÊµÊ±ÇúÏß×î´óµÄÊıÖµ
	unsigned short Y_Scale;				//ÇúÏßyÖá×îµÍµÄ¿Ì¶È
	unsigned short MUL_Y;				//YÖá·Å´ó±¶Êı
}LineInfo;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************ÆÁÄ»Ïà¹Ø¶¨Òå****************************************************/
/**********************************************************************************************************/
#define	LCD_Head_1		0xAA											//Ğ­ÒéÍ·1
#define	LCD_Head_2		0xBB											//Ğ­ÒéÍ·2

#define W_REGSITER 		0x80          									//Ğ´ÈëÊı¾İ,ÎŞÓ¦´ğ          
#define R_REGSITER 		0x81          									//¶ÁÈ¡Êı¾İ£¬ÓĞÓ¦´ğ
#define W_ADDRESS  		0x82											//Ğ´±äÁ¿¼Ä´æÆ÷£¬ÎŞÓ¦´ğ
#define R_ADDRESS  		0x83											//¶Á±äÁ¿¼Ä´æÆ÷Êı¾İ£¬ÓĞÓ¦´ğ
#define W_LINE     		0x84          									//Ğ´ÇúÏß»º³åÇø£¬ÎŞÓ¦´ğ

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
	SelfCheck_OK = 2,													//×Ô¼ìÕı³£
	SelfCheck_Error = 3,												//×Ô¼ìÓĞ²»¿ÉºöÂÔµÄ´íÎó
}SelfCheckStatus;
/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	DESC = 0,												//ÕıĞò
	ASC = 1													//ÄæĞò
}OrderByEnum;

//¶ÁÈ¡ÇëÇóĞÅÏ¢
#pragma pack(1)
typedef struct PageRequest_tag {
	unsigned int pageIndex;											//ÆğÊ¼¶ÁÈ¡Ë÷Òı£¬0ÎªµÚÒ»¸ö
	unsigned char pageSize;													//Ã¿Ò³µÄÊıÄ¿
	OrderByEnum orderType;													//ÅÅĞò·½Ê½
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

#define	DeviceRecordHeaderStructSize		sizeof(DeviceRecordHeader)								//×î¶à±£´æµÄÓÃ»§ÊıÄ¿
#define	DeviceRecordHeaderStructCrcSize		DeviceRecordHeaderStructSize - 2						//×î¶à±£´æµÄÓÃ»§ÊıÄ¿
#define	PageContentItemNum					10														//Ò»´Î×î¶à¶ÁÈ¡µÄÊıÄ¿
#pragma pack(1)
typedef struct
{
	unsigned int totalPageSize;																		//×ÜÒ³Êı
	unsigned int totalItemSize;																		//×ÜÊı
	unsigned int readItemSize;																		//¶Áµ½µÄÊıÄ¿
	bool isForNCD;																					//true¶ÁÈ¡Å¦¿µ¶ÈÊı¾İ£¬false¶ÁÈ¡ÓÃ»§Êı¾İ
	void * content[PageContentItemNum];																//Êı¾İ
}Page;
#pragma pack()

#define	PageStructSize		sizeof(Page)

/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	NoResult = 0,										//»¹Î´²âÊÔ£¬½á¹ûÎ´null
	NoSample = 1,										//Î´¼ÓÑù
	PeakError = 2,										//¼ì²â¿¨Ã»ÓĞC·å
	MemError = 3,										//ÄÚ´æ´íÎó
	TestInterrupt = 8,									//²âÊÔÖĞ¶Ï
	qrError = 9,										//¶şÎ¬Âë´íÎó
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

