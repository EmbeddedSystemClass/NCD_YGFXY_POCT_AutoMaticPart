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
#include	"TestDataDao.h"
#include	"SIM800_Fun.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"StringDefine.h"
#include 	"usbd_hid_core.h"

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
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static void setSelfTestStatus(ERROR_SelfTest status);
static MyRes loadSystemData(SelfTestBuf * selfTestBuf);
static MyRes testLed(void);
static MyRes testMotol(SelfTestBuf * selfTestBuf);

#if (DeviceUseType == Device_Demo)
static void deviceAdjustSelf(SelfTestBuf * selfTestBuf);
static void deviceErrorTest(SelfTestBuf * selfTestBuf);
#endif
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
	SelfTestBuf * s_SelfTestBuf = NULL;
	
	setSelfTestStatus(SelfTestting);
	createGSMMutex();
	
	s_SelfTestBuf = MyMalloc(sizeof(SelfTestBuf));
	
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
		
		//检测led
		if(My_Pass != testLed())
		{
			setSelfTestStatus(Light_Error);
			goto END;
		}

		//测试传动模块
		if(My_Pass != testMotol(s_SelfTestBuf))
		{
			setSelfTestStatus(Motol_ERROR);
			goto END;
		}
		
		#if (DeviceUseType == Device_Demo)
		//设备校准
		deviceAdjustSelf(s_SelfTestBuf);
		
		deviceErrorTest(s_SelfTestBuf);
		#endif
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
	for(selfTestBuf->i=0; selfTestBuf->i<3; selfTestBuf->i++)
	{
		vTaskDelay(500 / portTICK_RATE_MS);
		
		//读取设备信息
		ReadDeviceFromFile(&selfTestBuf->device);
		
		ReadSystemSetData(&selfTestBuf->systemSetData);

		if(selfTestBuf->device.crc != CalModbusCRC16Fun(&selfTestBuf->device, DeviceStructCrcSize, NULL))
		{
			memset(&selfTestBuf->device, 0, DeviceStructSize);
			memcpy(selfTestBuf->device.deviceId, DefaultDeviceId, strlen(DefaultDeviceId));
			
			selfTestBuf->device.crc = CalModbusCRC16Fun(&selfTestBuf->device, DeviceStructCrcSize, NULL);
			
			if(My_Fail == SaveDeviceToFile(&selfTestBuf->device))
				continue;
		}
		
		//更新内存中设备id的副本
		setSystemDeviceId(selfTestBuf->device.deviceId);
		
		//如果crc错误表示读取失败，或者设备第一次开机，不存在配置文件，则恢复出厂设置
		if(selfTestBuf->systemSetData.crc != CalModbusCRC16Fun(&selfTestBuf->systemSetData, SystemSetDataStructCrcSize, NULL))
		{
			setDefaultSystemSetData(&selfTestBuf->systemSetData);								//恢复出厂设置
			if(My_Fail == SaveSystemSetData(&selfTestBuf->systemSetData))
				continue;
		}
		else
			upDateSystemSetData(&selfTestBuf->systemSetData);									//将读取的配置更新到内存中
		
		return My_Pass;
	}
	
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
	//while(1)
	{
		SetLedVol(0);
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	if(ON == readLedCheckStatus())
		return My_Fail;
	
	SetLedVol(800);
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
/*	while(1)
	{
		sprintf(selfTestBuf->deviceAdjust.result, "%d", readJuliValue());
		USBDebug(selfTestBuf->deviceAdjust.result, strlen(selfTestBuf->deviceAdjust.result));
		vTaskDelay(100 / portTICK_RATE_MS);
	}*/
	
#if(Motor4Type == Motor4UsartMotor)

	//初始化电动夹爪
	selfTestBuf->i = 10;
	while(selfTestBuf->i-- && My_Fail == motor4Reset())
		vTaskDelay(100 / portTICK_RATE_MS);
	
	if(selfTestBuf->i <= 0)
		return My_Fail;
	
	FormatParmAndStartMotorAction(&selfTestBuf->motorAction, Motor4MoveDef, Motor4_OpenLocation, true);

#elif(Motor4Type == Motor4IOMotor)
	
	motor4MoveStep(false, 10000, false);
	selfTestBuf->i = 10;
	selfTestBuf->motor = getMotor(Motor_4);
	while(selfTestBuf->i-- && selfTestBuf->motor->motorLocation != 0)
		vTaskDelay(100 / portTICK_RATE_MS);
	
	if(selfTestBuf->motor->motorLocation != 0)
	{
		motor4MoveStep(true, 4000, false);
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
	
	motor4MoveStep(false, 10000, true);
	
	if(!Motor4Sensor1Triggered)
		return My_Fail;

	FormatParmAndStartMotorAction(&selfTestBuf->motorAction, Motor4MoveDef, Motor4_CloseLocation, true);
#endif
	
	//step 1 定位爪子位置
	selfTestBuf->juli = 0;
	selfTestBuf->motor = getMotor(Motor_2);

	timer_SetAndStart(&selfTestBuf->timer, 20);
	while(30000 > selfTestBuf->motor->motorLocation && TimerOut != timer_expired(&selfTestBuf->timer))
	{
		selfTestBuf->juli = readJuliValue();
		
		if(selfTestBuf->juli > 1060)
			selfTestBuf->motor->isFront = false;
		
		if(selfTestBuf->juli < 960)
			selfTestBuf->motor->isFront = true;
		
		motor2MoveStep(selfTestBuf->motor->isFront, 500, false);
		vTaskDelay(30 / portTICK_RATE_MS);
	}
	
	if(30000 > selfTestBuf->motor->motorLocation)
		return My_Fail;

/*	//step 2 判断插卡口是否有卡
	if(Motor1Sensor2Triggered && readCaedCheckStatus() == ON)
		FormatParmAndStartMotorAction(&selfTestBuf->motorAction, PutDownCardInPlaceDef, 0, true);

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
		FormatParmAndStartMotorAction(&selfTestBuf->motorAction, Motor1MoveDef, 2*selfTestBuf->j + 1, true);
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

			StartMotorAction(&selfTestBuf->motorAction, true);
		}
	}
	*/
	return My_Pass;
}

#if (DeviceUseType == Device_Demo)
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
#endif
