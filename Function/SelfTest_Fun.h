#ifndef SELFCHECK_FUN_H
#define SELFCHECK_FUN_H

#include	"DeviceError.h"
#include	"SystemSet_Data.h"
#include	"DeviceAdjust.h"
#include	"Timer.h"
#include	"Define.h"
#include	"Motor.h"
#include	"Motor_Fun.h"

typedef enum
{ 
	SystemData_OK = 0,								//系统数据加载成功
	SystemData_ERROR = 1,							//系统数据加载失败
	
	Light_OK = 2,									//led正常
	Light_Error = 3,								//led异常
	
	AD_OK = 4,										//采集模块正常
	AD_ERROR = 5,									//采集模块错误
	
	Motol_OK = 6,									//传动模块正常
	Motol_ERROR = 7,								//传动模块错误
	
	Erweima_OK = 8,									//二维码模块正常
	Erweima_ERROR = 9,								//二维码模块错误
	
	SD_OK = 10,
	SD_ERROR = 11,
	
	SelfTestting = 99,								//自检中
	SelfTest_OK = 100,								//自检成功
	SelfTest_Error = 101,								//自检成功
	SelfTest_None = 0xff
}ERROR_SelfTest;

#pragma pack(1)
typedef struct SelfTestBuf_Tag
{
	SystemSetData systemSetData;
	DeviceAdjust deviceAdjust;
	DeviceError deviceError;
	Device device;
	unsigned short juli;
	Motor * motor;
	MotorAction motorAction;
	Timer timer;
	double a;
	double b;
	unsigned char i;
	unsigned char j;
	unsigned int tempInt;
	MyBitAction locationStatus[Motor1_HalfLocation];
	MyRes status;
}SelfTestBuf;
#pragma pack()

void SelfTest_Function(void);
ERROR_SelfTest getSelfTestStatus(void);

#endif

