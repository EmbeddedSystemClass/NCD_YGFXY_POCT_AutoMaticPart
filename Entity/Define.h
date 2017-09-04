#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"

/*ÉèÖÃÖÐ¸÷¸ö¹¦ÄÜµÄÃÜÂë*/
#define		AdminPassWord				"201300\0"								//¹ÜÀíÔ±ÃÜÂë£¬ÓÃÓÚÐÞ¸ÄÉè±¸id
#define		AdjustPassWord				"201301\0"								//Ð£×¼ÃÜÂë
#define		TestPassWord				"201302\0"								//ÀÏ»¯²âÊÔÃÜÂë
#define		CheckQRPassWord				"201303\0"								//²âÊÔ¶þÎ¬ÂëÃÜÂë
#define		AdjLedPassWord				"201304\0"								//Ð£×¼ledÃÜÂë
#define		FactoryResetPassWord		"201305\0"								//»Ö¸´³ö³§ÉèÖÃÃÜÂë
#define		ChangeValueShowTypePassWord	"201306\0"								//ÇÐ»»½á¹ûÏÔÊ¾Ä£Ê½£¬ÊÇ·ñÏÔÊ¾ÕæÊµÖµ
#define		UnlockLCDPassWord			"201307\0"								//½âËøÆÁÄ»Ò»´Î

/*V1.0.03*/
#define	GB_SoftVersion	(unsigned short)2000
#define	GB_SoftVersion_Build	"Build17090401\0"

/*·þÎñÆ÷ÐÅÏ¢*/
#define	NCD_ServerIp_1		116
#define	NCD_ServerIp_2		62
#define	NCD_ServerIp_3		108
#define	NCD_ServerIp_4		201
#define NCD_ServerPort		8080

#define	HttpResponeOK				"HTTP/1.1 200 OK\0"

//poct
#define	ReadTimeUrl					"DeviceReadTime\0"
#define	QueryDeviceByDeviceIdUrl	"DeviceQueryDeviceByDeviceId\0"
#define	UpLoadDeviceAdjustUrl		"UpLoadDeviceAdjust\0"
#define	UpLoadDeviceErrorUrl		"UpLoadDeviceError\0"
#define	UpLoadYGFXYDataUrl			"upLoadYGFXY_Data\0"
#define	UpLoadDeviceMaintenanceUrl	"UpLoadDeviceMaintenance\0"
#define	UpLoadDeviceQualityUrl		"UpLoadDeviceQuality\0"

//ncd server
#define	NcdUpLoadYGFXYDataUrl		"UpLoadYGFXY\0"
#define	QueryRemoteSoftVersionUrl	"QuerySoftInfo\0"
#define	DownRemoteSoftFileUrl		"DownloadSoftFile\0"

/*SD¿¨ÎÄ¼þÃû*/
#define	TestDataFileName			"0:/TD.ncd\0"
#define	DeviceFileName				"0:/Device.ncd\0"
#define	DeviceAdjustFileName		"0:/Dadj.ncd\0"
#define	DeviceErrorFileName			"0:/Derr.ncd\0"
#define	DeviceMainenanceFileName	"0:/Dmai.ncd\0"
#define	DeviceQualityFileName		"0:/Dqua.ncd\0"


#define	_Use_AdjustLed_Fun	0
#define	_Use_Lwip_Fun	0
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

#define	PaiDuiWeiNum	8							//ÅÅ¶ÓÎ»ÖÃÊýÄ¿
typedef enum
{
	statusNull = 0,									//Ã»¿ªÊ¼,Ä¬ÈÏ×´Ì¬
	status_start = 5,								//Æô¶¯ÅÅ¶Ó
	status_timedown = 10,							//µ¹¼ÆÊ±×´Ì¬
	status_timeup = 15,								//³¬Ê±×´Ì¬
	status_waitTest = 14,							//µ¹¼ÆÊ±Ê±¼äÐ¡ÓÚ30s£¬¿¨ÉÁË¸ÌáÊ¾£¬½çÃæ²»ÇÐ»»£¬Ëø¶¨£¬²»ÔÊÐíÌí¼Ó¿¨
	status_testting = 16,							//½øÈë²âÊÔ×´Ì¬£¬µ¹¼ÆÊ±Ê±¼äÐ¡ÓÚ10s£¬ÇÐ»»µ½µ¹¼ÆÊ±½çÃæµÈ´ý
}MyPaiDuiStatues;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************¶þÎ¬ÂëÏà¹Ø¶¨Òå**************************************************/
/**********************************************************************************************************/


typedef enum
{
	CardCodeScanning = 0,											//ÕýÔÚÉ¨Ãè
	CardCodeScanOK = 1,												//É¨Ãè¶þÎ¬Âë³É¹¦
	CardCodeScanFail = 2,											//É¨ÃèÊ§°Ü
	CardCodeTimeOut = 3,											//¹ýÆÚ
	CardCodeCardOut = 4,											//¿¨±»°Î³ö
	CardCodeScanTimeOut = 5,										//É¨Ãè³¬Ê±
	CardCodeCRCError = 6,											//crc´íÎó
	CardUnsupported = 7,											//µ±Ç°³ÌÐò²»Ö§³Ö
	CardNone = 0xff,
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

#pragma pack(1)
typedef struct
{
	unsigned int totalPageSize;
	unsigned int totalItemSize;
	unsigned int readItemSize;
	void * content[10];
}Page;
#pragma pack()

#define	PageStructSize		sizeof(Page)

/**********************************************************************************************************/
/**********************************************************************************************************/

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/**************************Ð£×¼²ÎÊý***********************************************************/
/*********************************************************************************************/
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

#endif

