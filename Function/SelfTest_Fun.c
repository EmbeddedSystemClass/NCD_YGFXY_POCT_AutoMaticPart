/***************************************************************************************************
*FileName��SelfTest_Fun
*Description���Լ칦��
*Author��xsx
*Data��2016��1��27��10:29:14
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"SelfTest_Fun.h"

#include	"SystemSet_Data.h"
#include	"DeviceDao.h"
#include	"LedCheck_Driver.h"
#include	"DA_Driver.h"
#include	"SystemSet_Dao.h"
#include	"Motor1_Driver.h"
#include	"Motor2_Driver.h"
#include	"Motor4_Driver.h"
#include	"Motor1_Fun.h"
#include	"Motor2_Fun.h"
#include	"Motor4_Fun.h"

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
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static ERROR_SelfTest SelfTestStatus = SelfTestting;							//����ÿ���Լ�״̬
static FATFS S_Fatfs;				/* File system object */
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static void setSelfTestStatus(ERROR_SelfTest status);
static MyRes loadSystemData(void);
static MyRes testLed(void);
static MyRes testMotol(void);
static void deviceAdjustSelf(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
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
*FunctionName��SelfTest_Function
*Description���Լ칦��ʵ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��1��27��10:38:06
***************************************************************************************************/
void SelfTest_Function(void)
{
	setSelfTestStatus(SelfTestting);
	
	//SD����ʼ��
	if(FR_OK != f_mount(&S_Fatfs, "0:", 1))
	{
		setSelfTestStatus(SD_ERROR);
		return;
	}
	
	//����ϵͳ���ݣ����������ؽ��,�������ʧ�ܣ�����ֹ�Լ����
	if(My_Pass != loadSystemData())
	{
		setSelfTestStatus(SystemData_ERROR);
		return;
	}
	
	//���led
	if(My_Pass != testLed())
	{
		setSelfTestStatus(Light_Error);
		return;
	}

	//���Դ���ģ��
	if(My_Pass != testMotol())
	{
		setSelfTestStatus(Motol_ERROR);
		return;
	}
	
	//�豸У׼
//	deviceAdjustSelf();
	
	//�Լ���ɣ����ͽ��
	setSelfTestStatus(SelfTest_OK);
}

/***************************************************************************************************
*FunctionName: loadSystemData
*Description: ����ϵͳ����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��23��09:06:16
***************************************************************************************************/
static MyRes loadSystemData(void)
{
	SystemSetData * systemSetData = NULL;
	MyRes status = My_Fail;
	
	systemSetData = MyMalloc(SystemSetDataStructSize);

	if(systemSetData)
	{
		memset(systemSetData, 0, SystemSetDataStructSize);
		//��ȡSD���е������ļ�
		ReadSystemSetData(systemSetData);
		
		//���crc�����ʾ��ȡʧ�ܣ������豸��һ�ο����������������ļ�����ָ���������
		if(systemSetData->crc != CalModbusCRC16Fun(systemSetData, SystemSetDataStructCrcSize, NULL))
			setDefaultSystemSetData(systemSetData);								//�ָ���������
		else
			upDateSystemSetData(systemSetData);									//����ȡ�����ø��µ��ڴ���

		//�����Ƿ�ɹ���ȡ�������ļ���������SD��һ�Σ����Բ���SD���Ƿ�����
		if(My_Pass == SaveSystemSetData(systemSetData))
		{
			status = My_Pass;
		}
	}
	
	MyFree(systemSetData);
	
	return status;
}

/***************************************************************************************************
*FunctionName: testLed
*Description: ����led�Ƿ񷢹�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��23��15:39:25
***************************************************************************************************/
static MyRes testLed(void)
{
/*	SetLedVol(0);
	vTaskDelay(100 / portTICK_RATE_MS);
	if(ON == readLedCheckStatus())
		return My_Fail;
	
	SetLedVol(300);
	vTaskDelay(100 / portTICK_RATE_MS);
	if(OFF == readLedCheckStatus())
	{
		SetLedVol(0);
		return My_Fail;
	}

	SetLedVol(0);*/
	return My_Pass;
}

/***************************************************************************************************
*FunctionName: testMotol
*Description: ���Դ���ģ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��23��16:21:14
***************************************************************************************************/
static MyRes testMotol(void)
{
	//���Ե��1
	motor1MoveToNum(1, 20000);
	
	if(getMotorxLocation(Motor_1) != 1)
		return My_Fail;
	
	//�����2
	motor2MoveTo(0, 15000);
	if(!Motor2Sensor1Triggered)
		return My_Fail;
	
	motor2MoveTo(60000, 20000);
	if(!Motor2Sensor3Triggered)
		return My_Fail;
	
	motor2MoveTo(Motor2_MidLocation, 20000);
	
	//�����4
	motor4MoveTo(0, 5000);
	if(!Motor4Sensor1Triggered)
		return My_Fail;
	
	motor4MoveTo(Motor4_OpenLocation, 5000);
	vTaskDelay(3000 / portTICK_RATE_MS);

	return My_Pass;
}

static void deviceAdjustSelf(void)
{

}
