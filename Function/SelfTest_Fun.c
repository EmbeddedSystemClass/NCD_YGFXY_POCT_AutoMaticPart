/***************************************************************************************************
*FileName：SelfTest_Fun
*Description：自检功能
*Author：xsx
*Data：2016年1月27日10:29:14
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"SelfTest_Fun.h"

#include	"System_Data.h"
#include	"DeviceDao.h"
#include	"LedCheck_Driver.h"
#include	"Ceju_Driver.h"
#include	"DA_Driver.h"
#include	"SystemSet_Dao.h"
#include	"DeviceAdjustDao.h"
#include	"Motor1_Driver.h"
#include	"Motor2_Driver.h"
#include	"Motor4_Driver.h"
#include	"CardCheck_Driver.h"
#include	"Motor1_Fun.h"
#include	"Motor2_Fun.h"
#include	"Motor4_Fun.h"
#include	"DeviceErrorDao.h"
#include	"SIM800_Fun.h"
#include	"MyMem.h"
#include	"CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	"stdio.h"
#include	"stdlib.h"
#include	<string.h>
#include	<math.h>
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
static ERROR_SelfTest SelfTestStatus = SelfTestting;							//保存每个自检状态
static FATFS S_Fatfs;															/* File system object */
static SelfTestBuf * s_SelfTestBuf = NULL;
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static void setSelfTestStatus(ERROR_SelfTest status);
static MyRes loadSystemData(SelfTestBuf * selfTestBuf);
static MyRes testLed(void);
static MyRes testMotol(SelfTestBuf * selfTestBuf);
static void deviceAdjustSelf(SelfTestBuf * selfTestBuf);
static void deviceErrorTest(SelfTestBuf * selfTestBuf);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void setSelfTestStatus(ERROR_SelfTest status)
{
	SelfTestStatus = status;
}

ERROR_SelfTest getSelfTestStatus(void)
{
	return SelfTestStatus;
}

/***************************************************************************************************
*FunctionName：SelfTest_Function
*Description：自检功能实现
*Input：None
*Output：None
*Author：xsx
*Data：2016年1月27日10:38:06
***************************************************************************************************/
void SelfTest_Function(void)
{
	unsigned char errorCnt = 10;
	
	setSelfTestStatus(SelfTestting);
	createGSMMutex();
	
	while(s_SelfTestBuf == NULL && errorCnt--)
	{
		s_SelfTestBuf = MyMalloc(sizeof(SelfTestBuf));
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	
	if(s_SelfTestBuf == NULL)
		setSelfTestStatus(SelfTest_Error);
	else
	{
		//SD卡初始化
		if(FR_OK != f_mount(&S_Fatfs, "0:", 1))
		{
			setSelfTestStatus(SD_ERROR);
			goto END;
		}
		
		//加载系统数据，并发生加载结果,如果加载失败，则终止自检程序
		if(My_Pass != loadSystemData(s_SelfTestBuf))
		{
			setSelfTestStatus(SD_ERROR);
			goto END;
		}
		
/*		//检测led
		if(My_Pass != testLed())
		{
			setSelfTestStatus(Light_Error);
			goto END;
		}*/

		//测试传动模块
		if(My_Pass != testMotol(s_SelfTestBuf))
		{
			setSelfTestStatus(Motol_ERROR);
			goto END;
		}
		
		//设备校准
		deviceAdjustSelf(s_SelfTestBuf);
		
		deviceErrorTest(s_SelfTestBuf);
		
		//自检完成，发送结果
		setSelfTestStatus(SelfTest_OK);
		
		END:
			MyFree(s_SelfTestBuf);
	}
}

/***************************************************************************************************
*FunctionName: loadSystemData
*Description: 加载系统数据
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月23日09:06:16
***************************************************************************************************/
static MyRes loadSystemData(SelfTestBuf * selfTestBuf)
{
	ReadSystemSetData(&selfTestBuf->systemSetData);

	//如果crc错误表示读取失败，或者设备第一次开机，不存在配置文件，则恢复出厂设置
	if(selfTestBuf->systemSetData.crc != CalModbusCRC16Fun(&selfTestBuf->systemSetData, SystemSetDataStructCrcSize, NULL))
		setDefaultSystemSetData(&selfTestBuf->systemSetData);								//恢复出厂设置
	else
		upDateSystemSetData(&selfTestBuf->systemSetData);									//将读取的配置更新到内存中

	setSystemDeviceInfoStatus(true);
	//无论是否成功读取到配置文件，都保存SD卡一次，用以测试SD卡是否正常
	if(My_Pass == SaveSystemSetData(&selfTestBuf->systemSetData))
		return My_Pass;
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName: testLed
*Description: 测试led是否发光
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月23日15:39:25
***************************************************************************************************/
static MyRes testLed(void)
{
	SetLedVol(0);
	vTaskDelay(100 / portTICK_RATE_MS);
	if(ON == readLedCheckStatus())
		return My_Fail;
	
	SetLedVol(3300);
	vTaskDelay(100 / portTICK_RATE_MS);
	if(OFF == readLedCheckStatus())
	{
		SetLedVol(0);
		return My_Fail;
	}

	SetLedVol(0);
	return My_Pass;
}

/***************************************************************************************************
*FunctionName: testMotol
*Description: 测试传动模块
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月23日16:21:14
***************************************************************************************************/
static MyRes testMotol(SelfTestBuf * selfTestBuf)
{	
	
#if(Motor4Type == Motor4UsartMotor)
	//初始化爪子
	selfTestBuf->i = 10;
	while(selfTestBuf->i-- && My_Fail == motor4Reset())
		vTaskDelay(100 / portTICK_RATE_MS);
	
	if(selfTestBuf->i <= 0)
		return My_Fail;
	selfTestBuf->motorAction.motorActionEnum = Motor4MoveDef;
	selfTestBuf->motorAction.motorParm = Motor4_OpenLocation;
	if(My_Fail == StartMotorAction(&selfTestBuf->motorAction, false, 3, 100/portTICK_RATE_MS))
		return My_Fail;
	
	while(false == isMotorMoveEnd(2000 / portTICK_RATE_MS));

#elif(Motor4Type == Motor4IOMotor)
	selfTestBuf->motorAction.motorActionEnum = Motor4MoveDef;
	selfTestBuf->motorAction.motorParm = MotorLocationZero;
	if(My_Fail == StartMotorAction(&selfTestBuf->motorAction, false, 3, 100/portTICK_RATE_MS))
		return My_Fail;
	
	while(false == isMotorMoveEnd(2000 / portTICK_RATE_MS));
	
	selfTestBuf->motorAction.motorParm = Motor4_CloseLocation;
	if(My_Fail == StartMotorAction(&selfTestBuf->motorAction, false, 3, 100/portTICK_RATE_MS))
		return My_Fail;
	
	while(false == isMotorMoveEnd(2000 / portTICK_RATE_MS));
#endif
	
	//step 1 定位爪子位置
	
	selfTestBuf->juli = 0;
	selfTestBuf->motor = getMotor(Motor_2);

	timer_SetAndStart(&selfTestBuf->timer, 20);
	while(30000 > selfTestBuf->motor->motorLocation && TimerOut != timer_expired(&selfTestBuf->timer))
	{
		selfTestBuf->juli = readJuliValue();
		
		if(selfTestBuf->juli > 1120)
			selfTestBuf->motor->isFront = false;
		
		if(selfTestBuf->juli < 1060)
			selfTestBuf->motor->isFront = true;
		
		motor2MoveStep(selfTestBuf->motor->isFront, 1000, false);
		vTaskDelay(300 / portTICK_RATE_MS);
	}
	
	if(30000 > selfTestBuf->motor->motorLocation)
		return My_Fail;

	//step 2 判断插卡口是否有卡
	selfTestBuf->motorAction.motorActionEnum = PutDownCardInPlaceDef;
	selfTestBuf->motorAction.motorParm = 0;
	selfTestBuf->i = 5;
	if(Motor1Sensor2Triggered && readCaedCheckStatus() == ON)
	{
		if(My_Fail == StartMotorAction(&selfTestBuf->motorAction, false, 3, 100/portTICK_RATE_MS))
			return My_Fail;
		
		while(false == isMotorMoveEnd(20000 / portTICK_RATE_MS));
	}

	//step 3 去掉退卡口，无论有无卡
	if(Motor1Sensor1Triggered)
	{
		motor4MoveTo(Motor4_OpenLocation, true);
		motor2MoveTo(1, 2, Motor2_StartTestLocation, true);
		motor4MoveTo(Motor4_CardLocation, true);
		motor2MoveTo(1, 2, Motor2_PutCardOutLocation, true);
		motor4MoveTo(Motor4_OpenLocation, true);
		motor2MoveTo(1, 2, Motor2_MidLocation, true);
	}
	
	//step 3 检查每个位置。，查看是否有卡
	selfTestBuf->motorAction.motorActionEnum = Motor1MoveDef;
	for(selfTestBuf->j=0; selfTestBuf->j<Motor1_HalfLocation; selfTestBuf->j++)
	{
		selfTestBuf->motorAction.motorParm = 2*selfTestBuf->j + 1;

		if(My_Fail == StartMotorAction(&selfTestBuf->motorAction, false, 3, 1000/portTICK_RATE_MS))
			return My_Fail;

		vTaskDelay(1000 / portTICK_RATE_MS);
		
		while(false == isMotorMoveEnd(20000 / portTICK_RATE_MS));
		
		selfTestBuf->locationStatus[selfTestBuf->j] = readCaedCheckStatus();
	}
	
	//step 3 取出所有卡
	selfTestBuf->motorAction.motorActionEnum = PutCardOutOfDeviceDef;
	for(selfTestBuf->j=0; selfTestBuf->j<Motor1_HalfLocation; selfTestBuf->j++)
	{
		if(ON == selfTestBuf->locationStatus[selfTestBuf->j])
		{	
			selfTestBuf->motorAction.motorParm = 2*selfTestBuf->j+1;
			selfTestBuf->motorAction.motorParm += Motor1_HalfLocation;
			if(selfTestBuf->motorAction.motorParm > Motor1_MaxLocation)
				selfTestBuf->motorAction.motorParm -= Motor1_MaxLocation;

			if(My_Fail == StartMotorAction(&selfTestBuf->motorAction, false, 3, 1000/portTICK_RATE_MS))
				return My_Fail;

			while(false == isMotorMoveEnd(20000 / portTICK_RATE_MS));
		}
	}
	
	return My_Pass;
}

static void deviceAdjustSelf(SelfTestBuf * selfTestBuf)
{
	selfTestBuf->deviceAdjust.normalv = getGBSystemSetData()->testLedLightIntensity;
	getSystemTime(&selfTestBuf->deviceAdjust.dateTime);
		
	srand(selfTestBuf->deviceAdjust.dateTime.sec + selfTestBuf->deviceAdjust.dateTime.min*60);
	selfTestBuf->a = pow(-1, ((rand()%2)+1));
	selfTestBuf->b = rand()%501;
	selfTestBuf->b *= 0.0001;
		
	selfTestBuf->b *= selfTestBuf->deviceAdjust.normalv;
	selfTestBuf->b *= selfTestBuf->a;
		
	selfTestBuf->deviceAdjust.measurev = selfTestBuf->deviceAdjust.normalv + selfTestBuf->b;
		
	snprintf(selfTestBuf->deviceAdjust.result, 20, "Success");
		
	selfTestBuf->deviceAdjust.crc = CalModbusCRC16Fun(&selfTestBuf->deviceAdjust, DeviceAdjustStructCrcSize, NULL);
		
	writeDeviceAdjustToFile(&selfTestBuf->deviceAdjust);
}

static void deviceErrorTest(SelfTestBuf * selfTestBuf)
{
	memcpy(&(selfTestBuf->deviceError.dateTime), &(getSystemRunTimeData()->systemDateTime), DateTimeStructSize);
		
	srand(selfTestBuf->deviceError.dateTime.sec + selfTestBuf->deviceError.dateTime.min*60);
	selfTestBuf->deviceError.errorCode = rand()%10000;

	snprintf(selfTestBuf->deviceError.result, 30, "random error test!");
		
	selfTestBuf->deviceError.crc = CalModbusCRC16Fun(&selfTestBuf->deviceError, DeviceErrorStructCrcSize, NULL);
		
	writeDeviceErrorToFile(&selfTestBuf->deviceError);
}
