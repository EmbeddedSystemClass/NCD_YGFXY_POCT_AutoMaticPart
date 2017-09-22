#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"

/*设置中各个功能的密码*/
#define		AdminPassWord				"201300\0"								//管理员密码，用于修改设备id
#define		AdjustPassWord				"201301\0"								//校准密码
#define		TestPassWord				"201302\0"								//老化测试密码
#define		CheckQRPassWord				"201303\0"								//测试二维码密码
#define		AdjLedPassWord				"201304\0"								//校准led密码
#define		FactoryResetPassWord		"201305\0"								//恢复出厂设置密码
#define		ChangeValueShowTypePassWord	"201306\0"								//切换结果显示模式，是否显示真实值
#define		UnlockLCDPassWord			"201307\0"								//解锁屏幕一次

/*V1.0.03*/
#define	GB_SoftVersion	(unsigned short)2000
#define	GB_SoftVersion_Build	"Build17090401\0"

/*服务器信息*/
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

/*SD卡文件名*/
#define	TestDataFileName			"0:/TD.ncd\0"
#define	DeviceFileName				"0:/Device.ncd\0"
#define	DeviceAdjustFileName		"0:/Dadj.ncd\0"
#define	DeviceErrorFileName			"0:/Derr.ncd\0"
#define	DeviceMainenanceFileName	"0:/Dmai.ncd\0"
#define	DeviceQualityFileName		"0:/Dqua.ncd\0"


#define	_Use_AdjustLed_Fun	0
#define	_Use_Lwip_Fun	0
/**********************************************************************************************************/
/******************************************操作结果变量*************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	My_Pass = pdPASS,			//操作成功
	My_Fail = pdFAIL			//操作失败
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
/******************************************fatfs结构体定义*************************************************/
/**********************************************************************************************************/
/*此结构体用于在进行sd操作是申请内存*/
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

#define	PaiDuiWeiNum	8							//排队位置数目
typedef enum
{
	statusNull = 0,									//没开始,默认状态
	status_start = 5,								//启动排队
	status_timedown = 10,							//倒计时状态
	status_timeup = 15,								//超时状态
	status_waitTest = 14,							//倒计时时间小于30s，卡闪烁提示，界面不切换，锁定，不允许添加卡
	status_testting = 16,							//进入测试状态，倒计时时间小于10s，切换到倒计时界面等待
	status_end = 17,
}MyPaiDuiStatues;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************二维码相关定义**************************************************/
/**********************************************************************************************************/


typedef enum
{
	CardCodeScanning = 0,											//正在扫描
	CardCodeScanOK = 1,												//扫描二维码成功
	CardCodeScanFail = 2,											//扫描失败
	CardCodeTimeOut = 3,											//过期
	CardCodeCardOut = 4,											//卡被拔出
	CardCodeScanTimeOut = 5,										//扫描超时
	CardCodeCRCError = 6,											//crc错误
	CardUnsupported = 7,											//当前程序不支持
	CardNone = 0xff,
}ScanCodeResult;


/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************样品ID相关定义**************************************************/
/**********************************************************************************************************/
#define	MaxSampleIDLen	20											//最笾С盅穒d长度为20
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************界面相关定义****************************************************/
/***************************************************************************�*******************************/

typedef struct LineInfo_Tag
{
	unsigned short MaxData;				//实时曲线最大的数值
	unsigned short Y_Scale;				//曲线y轴最低的刻度
	unsigned short MUL_Y;				//Y轴放大倍数
}LineInfo;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************屏幕相关定义****************************************************/
/**********************************************************************************************************/
#define	LCD_Head_1		0xAA											//协议头1
#define	LCD_Head_2		0xBB											//协议头2

#define W_REGSITER 		0x80          									//写入数据,无应答          
#define R_REGSITER 		0x81          									//读取数据，有应答
#define W_ADDRESS  		0x82											//写变量寄存器，无应答
#define R_ADDRESS  		0x83											//读变量寄存器数据，有应答
#define W_LINE     		0x84          									//写曲线缓冲区，无应答

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
/******************************************自检功能相关定义****************************************************/
/**********************************************************************************************************/



typedef enum
{
	SelfCheck_None = 0,													//自检未开始
	SelfChecking = 1,
	SelfCheck_OK = 2,													//自检正常
	SelfCheck_Error = 3,												//自检有不可忽略的错误
}SelfCheckStatus;
/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	DESC = 0,												//正序
	ASC = 1													//逆序
}OrderByEnum;

//读取请求信息
#pragma pack(1)
typedef struct PageRequest_tag {
	unsigned int pageIndex;											//起始读取索引，0为第一个
	unsigned char pageSize;													//每页的数目
	OrderByEnum orderType;													//排序方式
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

#define	DeviceRecordHeaderStructSize		sizeof(DeviceRecordHeader)								//最多保存的用户数目
#define	DeviceRecordHeaderStructCrcSize		DeviceRecordHeaderStructSize - 2						//最多保存的用户数目

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
/**************************校准参数***********************************************************/
/*********************************************************************************************/
typedef enum
{
	NoResult = 0,										//还未测试，结果未null
	NoSample = 1,										//未加样
	PeakError = 2,										//检测卡没有C峰
	MemError = 3,										//内存错误
	TestInterrupt = 8,									//测试中断
	ResultIsOK = 99,									//测试成功
}ResultState;

#pragma pack(1)
typedef struct Point_tag {
	unsigned short y;
	unsigned short x;
}Point;
#pragma pack()

#endif

