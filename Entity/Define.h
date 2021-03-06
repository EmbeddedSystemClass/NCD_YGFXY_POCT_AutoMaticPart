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

	#define	GB_SoftVersion	(unsigned short)1017
	#define	GB_SoftVersionStr	"V1.0.17\0"
	#define	GB_SoftVersion_Build	"Build18082701\0"
	
#elif (DeviceBuildId == Device_NCD13021806109)

	#define	GB_SoftVersion	(unsigned short)1017
	#define	GB_SoftVersionStr	"V1.0.17\0"
	#define	GB_SoftVersion_Build	"Build18082701\0"

#elif (DeviceBuildId == Device_NCD13021801104)
	
	#define	GB_SoftVersion	(unsigned short)1016
	#define	GB_SoftVersionStr	"V1.0.16\0"
	#define	GB_SoftVersion_Build	"Build18082701\0"
	
#elif (DeviceBuildId == Device_NCD13021801105)
	
	#define	GB_SoftVersion	(unsigned short)1016
	#define	GB_SoftVersionStr	"V1.0.16\0"
	#define	GB_SoftVersion_Build	"Build18082701\0"
	
#elif (DeviceBuildId == Device_NCD13021801106)
	
	#define	GB_SoftVersion	(unsigned short)1017
	#define	GB_SoftVersionStr	"V1.0.17\0"
	#define	GB_SoftVersion_Build	"Build18082701\0"
	
#endif

//二维码定义
#define QRVersion0Define		0								//二维码原始版本定义
#define QRVersion2Define		'2'								//二维码版本2定义
#define QRVersion3Define		'3'								//二维码版本3定义
#define QRVersionUnDefine		0xff							//不支持二维码版本
#define	GB_QRVersion			QRVersion3Define				//当前支持的二维码版本，往下兼容

/*服务器信息*/
#define	NCD_ServerIp_1		116
#define	NCD_ServerIp_2		62
#define	NCD_ServerIp_3		108
#define	NCD_ServerIp_4		201
#define NCD_ServerPort		8080


#define	Motor4IOMotor					0x90							//IO口控制的爪子
#define	Motor4UsartMotor				0x91							//串口控制的爪子
#define	Motor4Type						Motor4UsartMotor

#define	DEVICE_EN						100
#define	DEVICE_CN						101
#define	DeviceLanguage					DEVICE_CN

#define	UserNormalProgram				0x60							//通用程序,上传至ncd自己的软件
#define	UserXGProgram					0x61							//孝感中心医院程序，孝感LIS
#define	UserProgramType					UserNormalProgram				//程序用户选择定义

#define	Device_Final					0x27                            //正式程序
#define	Device_Demo						0x28                            //演示程序
#define	Device_FastDemo					0x29                            //更快的演示程序
#define Device_TestCenter               0x2A                            //检验中心程序
#define	DeviceUseType					Device_Final	

/**********************************************************************************************************/
/**********************************************************************************************************/
/**********************************************************************************************************/


#define	PaiDuiWeiNum					9							//排队位置数目
#define	MaxQualityCount					PaiDuiWeiNum

#define	MaxUpLoadTestDataNum			1							//测试数据一次上传数目，must less than PageContentItemNum
#define MAX_TEST_CNT                    1                           //重测次数
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
typedef enum
{
	statusNull = 0,									//没开始,默认状态
	statusNone = 19,								//没开始,默认状态
	statusMotorMoveWaitCard = 1,					//电机正在运行到等待插卡口
	statusWaitCardPutIn = 2,						//等待插卡
	statusWaitCardPutOut = 4,						//等待插卡
	statusWaitScanQR = 3,							//等待扫描二维码结果
	statusMotorPutCardDown = 6,						//读取二维码成功后，等待电机运动将卡放在排队位
	statusMotorOrigin = 7,							//等待电机回原点
	status_start = 5,								//启动排队
	status_timedown = 10,							//倒计时状态
	status_timeup = 15,								//超时状态
	status_testting = 16,							//开始测试
	statusPrepareTest = 18,							//进入测试状态，倒计时时间小于10s，切换到倒计时界面等待
	statusTestMotor = 20,							//等待电机走到测试位置
	statusPutCardOut = 21,							//将卡排除设备
	statusWaitCardOut = 22,							//等待卡排除设备
	statusWaitPutCardInTestPlace = 23,				//等待卡在测试端放好
	statusInputSampleID = 24,						//正在输入样品ID
	status_end = 17,
}MyPaiDuiStatues;

#define	StartTestTime								10					//禁止创建测试的最小时间
#define	DisableCreateNewTestTime					30					//禁止创建测试的最小时间
#define	DisableReturnTime							40					//禁止返回，即将测试时间
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************二维码相关定义**************************************************/
/**********************************************************************************************************/


typedef enum
{
	CardScanNone = 0,
	
	CardCodeScanOK = 1,												//扫描二维码成功
	CardCodeScanFail = 2,											//扫描失败
	CardCodeTimeOut = 3,											//过期
	CardCodeCardOut = 4,											//卡被拔出
	CardCodeScanTimeOut = 5,										//扫描超时
	CardCodeCRCError = 6,											//crc错误
	CardUnsupported = 7,											//当前程序不支持
	CardCodeScanning = 8,											//正在扫描
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

#define	BasicIcoStructSize		sizeof(Basic_ICO)
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
#define	PageContentItemNum					10														//一次最多读取的数目
#pragma pack(1)
typedef struct
{
	unsigned int totalPageSize;																		//总页数
	unsigned int totalItemSize;																		//总数
	unsigned int readItemSize;																		//读到的数目
	bool isForNCD;																					//true读取纽康度数据，false读取用户数据
	void * content[PageContentItemNum];																//数据
}Page;
#pragma pack()

#define	PageStructSize		sizeof(Page)

/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	NoResult = 0,										//还未测试，结果未null
	NoSample = 1,										//未加样
	PeakError = 2,										//检测卡没有C峰
	MemError = 3,										//内存错误
    T_CV_0_05 = 4,                                      //找不到T，但是默认位置的cv>0.05
    C_CV_ERR_1 = 5,
    C_CV_ERR_2 = 6,
    C_L_T_L_1 = 7,                                        //c位置小于T位置
    C_L_T_L_2 = 9,                                        //c位置 - T位置 (50-100)
    C_CV_T_CV = 10,                                     //c_cv + t_cv < 0.13
    CANLIU = 11,                                        //残留
    B_V_ERR = 12,                                       //基线值错误
	TestInterrupt = 8,									//测试中断
	qrError = 13,										//二维码错误
    RE_TEST = 14,                                       //重测
	ResultIsOK = 99,									//测试成功
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

