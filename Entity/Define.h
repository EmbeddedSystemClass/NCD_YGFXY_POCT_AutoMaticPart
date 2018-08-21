#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"

/**********************************************************************************************************/
/**********************************************************************************************************/
/**********************************************************************************************************/
//build by device id
#define	DeviceAll						0x30
#define	Device_NCD13021806109			0x31
#define	Device_NCD13021801104			0x32
#define	Device_NCD13021801105			0x33
#define	Device_NCD13021801106			0x34
#define	DeviceBuildId					DeviceAll	

#if (DeviceBuildId == DeviceAll)

	#define	GB_SoftVersion	(unsigned short)1016
	#define	GB_SoftVersionStr	"V1.0.16\0"
	#define	GB_SoftVersion_Build	"Build18081501\0"
	
#elif (DeviceBuildId == Device_NCD13021806109)

	#define	GB_SoftVersion	(unsigned short)1016
	#define	GB_SoftVersionStr	"V1.0.16\0"
	#define	GB_SoftVersion_Build	"Build18070501\0"

#elif (DeviceBuildId == Device_NCD13021801104)
	
	#define	GB_SoftVersion	(unsigned short)1015
	#define	GB_SoftVersionStr	"V1.0.15\0"
	#define	GB_SoftVersion_Build	"Build18070501\0"
	
#elif (DeviceBuildId == Device_NCD13021801105)
	
	#define	GB_SoftVersion	(unsigned short)1015
	#define	GB_SoftVersionStr	"V1.0.15\0"
	#define	GB_SoftVersion_Build	"Build18070501\0"
	
#elif (DeviceBuildId == Device_NCD13021801106)
	
	#define	GB_SoftVersion	(unsigned short)1016
	#define	GB_SoftVersionStr	"V1.0.16\0"
	#define	GB_SoftVersion_Build	"Build18070501\0"
	
#endif

//∂˛Œ¨¬Î∂®“Â
#define QRVersion0Define		0								//∂˛Œ¨¬Î‘≠ º∞Ê±æ∂®“Â
#define QRVersion2Define		'2'								//∂˛Œ¨¬Î∞Ê±æ2∂®“Â
#define QRVersion3Define		'3'								//∂˛Œ¨¬Î∞Ê±æ3∂®“Â
#define QRVersionUnDefine		0xff							//≤ª÷ß≥÷∂˛Œ¨¬Î∞Ê±æ
#define	GB_QRVersion			QRVersion3Define				//µ±«∞÷ß≥÷µƒ∂˛Œ¨¬Î∞Ê±æ£¨Õ˘œ¬ºÊ»›

/*∑˛ŒÒ∆˜–≈œ¢*/
#define	NCD_ServerIp_1		116
#define	NCD_ServerIp_2		62
#define	NCD_ServerIp_3		108
#define	NCD_ServerIp_4		201
#define NCD_ServerPort		8080


#define	Motor4IOMotor					0x90							//IOø⁄øÿ÷∆µƒ◊¶◊”
#define	Motor4UsartMotor				0x91							//¥Æø⁄øÿ÷∆µƒ◊¶◊”
#define	Motor4Type						Motor4UsartMotor

#define	DEVICE_EN						100
#define	DEVICE_CN						101
#define	DeviceLanguage					DEVICE_CN

#define	UserNormalProgram				0x60							//Õ®”√≥Ã–Ú,…œ¥´÷¡ncd◊‘º∫µƒ»Ìº˛
#define	UserXGProgram					0x61							//–¢∏–÷––ƒ“Ω‘∫≥Ã–Ú£¨–¢∏–LIS
#define	UserProgramType					UserNormalProgram				//≥Ã–Ú”√ªß—°‘Ò∂®“Â

#define	Device_Final					0x27                            //’˝ Ω≥Ã–Ú
#define	Device_Demo						0x28                            //—› æ≥Ã–Ú
#define	Device_FastDemo					0x29                            //∏¸øÏµƒ—› æ≥Ã–Ú
#define Device_TestCenter               0x2A                            //ºÏ—È÷––ƒ≥Ã–Ú
#define	DeviceUseType					Device_Final	

/**********************************************************************************************************/
/**********************************************************************************************************/
/**********************************************************************************************************/


#define	PaiDuiWeiNum					9							//≈≈∂”Œª÷√ ˝ƒø
#define	MaxQualityCount					PaiDuiWeiNum

#define	MaxUpLoadTestDataNum			1							//≤‚ ‘ ˝æ›“ª¥Œ…œ¥´ ˝ƒø£¨must less than PageContentItemNum
/**********************************************************************************************************/
/******************************************≤Ÿ◊˜Ω·π˚±‰¡ø*************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	My_Pass = pdPASS,			//≤Ÿ◊˜≥…π¶
	My_Fail = pdFAIL			//≤Ÿ◊˜ ß∞‹
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
#define	FreeRTOSDelay_50	50				//10ms
/**********************************************************************************************************/
/******************************************fatfsΩ·ππÃÂ∂®“Â*************************************************/
/**********************************************************************************************************/
/*¥ÀΩ·ππÃÂ”√”⁄‘⁄Ω¯––sd≤Ÿ◊˜ «…Í«Îƒ⁄¥Ê*/
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
	statusNull = 0,									//√ªø™ º,ƒ¨»œ◊¥Ã¨
	statusNone = 19,								//√ªø™ º,ƒ¨»œ◊¥Ã¨
	statusMotorMoveWaitCard = 1,					//µÁª˙’˝‘⁄‘À––µΩµ»¥˝≤Âø®ø⁄
	statusWaitCardPutIn = 2,						//µ»¥˝≤Âø®
	statusWaitCardPutOut = 4,						//µ»¥˝≤Âø®
	statusWaitScanQR = 3,							//µ»¥˝…®√Ë∂˛Œ¨¬ÎΩ·π˚
	statusMotorPutCardDown = 6,						//∂¡»°∂˛Œ¨¬Î≥…π¶∫Û£¨µ»¥˝µÁª˙‘À∂ØΩ´ø®∑≈‘⁄≈≈∂”Œª
	statusMotorOrigin = 7,							//µ»¥˝µÁª˙ªÿ‘≠µ„
	status_start = 5,								//∆Ù∂Ø≈≈∂”
	status_timedown = 10,							//µπº∆ ±◊¥Ã¨
	status_timeup = 15,								//≥¨ ±◊¥Ã¨
	status_testting = 16,							//ø™ º≤‚ ‘
	statusPrepareTest = 18,							//Ω¯»Î≤‚ ‘◊¥Ã¨£¨µπº∆ ± ±º‰–°”⁄10s£¨«–ªªµΩµπº∆ ±ΩÁ√Êµ»¥˝
	statusTestMotor = 20,							//µ»¥˝µÁª˙◊ﬂµΩ≤‚ ‘Œª÷√
	statusPutCardOut = 21,							//Ω´ø®≈≈≥˝…Ë±∏
	statusWaitCardOut = 22,							//µ»¥˝ø®≈≈≥˝…Ë±∏
	statusWaitPutCardInTestPlace = 23,				//µ»¥˝ø®‘⁄≤‚ ‘∂À∑≈∫√
	statusInputSampleID = 24,						//’˝‘⁄ ‰»Î—˘∆∑ID
	status_end = 17,
}MyPaiDuiStatues;

#define	StartTestTime								10					//Ω˚÷π¥¥Ω®≤‚ ‘µƒ◊Ó–° ±º‰
#define	DisableCreateNewTestTime					30					//Ω˚÷π¥¥Ω®≤‚ ‘µƒ◊Ó–° ±º‰
#define	DisableReturnTime							40					//Ω˚÷π∑µªÿ£¨º¥Ω´≤‚ ‘ ±º‰
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************∂˛Œ¨¬Îœ‡πÿ∂®“Â**************************************************/
/**********************************************************************************************************/


typedef enum
{
	CardScanNone = 0,
	
	CardCodeScanOK = 1,												//…®√Ë∂˛Œ¨¬Î≥…π¶
	CardCodeScanFail = 2,											//…®√Ë ß∞‹
	CardCodeTimeOut = 3,											//π˝∆⁄
	CardCodeCardOut = 4,											//ø®±ª∞Œ≥ˆ
	CardCodeScanTimeOut = 5,										//…®√Ë≥¨ ±
	CardCodeCRCError = 6,											//crc¥ÌŒÛ
	CardUnsupported = 7,											//µ±«∞≥Ã–Ú≤ª÷ß≥÷
	CardCodeScanning = 8,											//’˝‘⁄…®√Ë
}ScanCodeResult;


/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************—˘∆∑IDœ‡πÿ∂®“Â**************************************************/
/**********************************************************************************************************/
#define	MaxSampleIDLen	20											//◊ÓÛ÷ß≥÷—˘∆∑id≥§∂»Œ™20
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************ΩÁ√Êœ‡πÿ∂®“Â****************************************************/
/***************************************************************************¥*******************************/

typedef struct LineInfo_Tag
{
	unsigned short MaxData;				// µ ±«˙œﬂ◊Ó¥Ûµƒ ˝÷µ
	unsigned short Y_Scale;				//«˙œﬂy÷·◊ÓµÕµƒøÃ∂»
	unsigned short MUL_Y;				//Y÷·∑≈¥Û±∂ ˝
}LineInfo;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************∆¡ƒªœ‡πÿ∂®“Â****************************************************/
/**********************************************************************************************************/
#define	LCD_Head_1		0xAA											//–≠“ÈÕ∑1
#define	LCD_Head_2		0xBB											//–≠“ÈÕ∑2

#define W_REGSITER 		0x80          									//–¥»Î ˝æ›,Œﬁ”¶¥          
#define R_REGSITER 		0x81          									//∂¡»° ˝æ›£¨”–”¶¥
#define W_ADDRESS  		0x82											//–¥±‰¡øºƒ¥Ê∆˜£¨Œﬁ”¶¥
#define R_ADDRESS  		0x83											//∂¡±‰¡øºƒ¥Ê∆˜ ˝æ›£¨”–”¶¥
#define W_LINE     		0x84          									//–¥«˙œﬂª∫≥Â«¯£¨Œﬁ”¶¥

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
/******************************************◊‘ºÏπ¶ƒ‹œ‡πÿ∂®“Â****************************************************/
/**********************************************************************************************************/



typedef enum
{
	SelfCheck_None = 0,													//◊‘ºÏŒ¥ø™ º
	SelfChecking = 1,
	SelfCheck_OK = 2,													//◊‘ºÏ’˝≥£
	SelfCheck_Error = 3,												//◊‘ºÏ”–≤ªø…∫ˆ¬‘µƒ¥ÌŒÛ
}SelfCheckStatus;
/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	DESC = 0,												//’˝–Ú
	ASC = 1													//ƒÊ–Ú
}OrderByEnum;

//∂¡»°«Î«Û–≈œ¢
#pragma pack(1)
typedef struct PageRequest_tag {
	unsigned int pageIndex;											//∆ º∂¡»°À˜“˝£¨0Œ™µ⁄“ª∏ˆ
	unsigned char pageSize;													//√ø“≥µƒ ˝ƒø
	OrderByEnum orderType;													//≈≈–Ú∑Ω Ω
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

#define	DeviceRecordHeaderStructSize		sizeof(DeviceRecordHeader)								//◊Ó∂‡±£¥Êµƒ”√ªß ˝ƒø
#define	DeviceRecordHeaderStructCrcSize		DeviceRecordHeaderStructSize - 2						//◊Ó∂‡±£¥Êµƒ”√ªß ˝ƒø
#define	PageContentItemNum					10														//“ª¥Œ◊Ó∂‡∂¡»°µƒ ˝ƒø
#pragma pack(1)
typedef struct
{
	unsigned int totalPageSize;																		//◊‹“≥ ˝
	unsigned int totalItemSize;																		//◊‹ ˝
	unsigned int readItemSize;																		//∂¡µΩµƒ ˝ƒø
	bool isForNCD;																					//true∂¡»°≈¶øµ∂» ˝æ›£¨false∂¡»°”√ªß ˝æ›
	void * content[PageContentItemNum];																// ˝æ›
}Page;
#pragma pack()

#define	PageStructSize		sizeof(Page)

/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	NoResult = 0,										//ªπŒ¥≤‚ ‘£¨Ω·π˚Œ¥null
	NoSample = 1,										//Œ¥º”—˘
	PeakError = 2,										//ºÏ≤‚ø®√ª”–C∑Â
	MemError = 3,										//ƒ⁄¥Ê¥ÌŒÛ
    T_CV_0_05 = 4,                                      //’“≤ªµΩT£¨µ´ «ƒ¨»œŒª÷√µƒcv>0.05
    C_CV_ERR_1 = 5,
    C_CV_ERR_2 = 6,
    C_L_T_L_1 = 7,                                        //cŒª÷√–°”⁄TŒª÷√
    C_L_T_L_2 = 9,                                        //cŒª÷√ - TŒª÷√ (50-100)
    C_CV_T_CV = 10,                                     //c_cv + t_cv < 0.13
    CANLIU = 11,                                        //≤–¡Ù
    B_V_ERR = 12,                                       //ª˘œﬂ÷µ¥ÌŒÛ
	TestInterrupt = 8,									//≤‚ ‘÷–∂œ
	qrError = 13,										//∂˛Œ¨¬Î¥ÌŒÛ
	ResultIsOK = 99,									//≤‚ ‘≥…π¶
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


#pragma pack(1)
typedef struct PrintInfo_Tag
{
	char header[30];
	unsigned short crc;
}PrintInfo;
#pragma pack()

#endif

