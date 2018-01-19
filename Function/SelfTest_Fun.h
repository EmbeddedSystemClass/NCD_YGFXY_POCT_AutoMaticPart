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
	SystemData_OK = 0,								//ϵͳ���ݼ��سɹ�
	SystemData_ERROR = 1,							//ϵͳ���ݼ���ʧ��
	
	Light_OK = 2,									//led����
	Light_Error = 3,								//led�쳣
	
	AD_OK = 4,										//�ɼ�ģ������
	AD_ERROR = 5,									//�ɼ�ģ�����
	
	Motol_OK = 6,									//����ģ������
	Motol_ERROR = 7,								//����ģ�����
	
	Erweima_OK = 8,									//��ά��ģ������
	Erweima_ERROR = 9,								//��ά��ģ�����
	
	SD_OK = 10,
	SD_ERROR = 11,
	
	SelfTestting = 99,								//�Լ���
	SelfTest_OK = 100,								//�Լ�ɹ�
	SelfTest_Error = 101,								//�Լ�ɹ�
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

