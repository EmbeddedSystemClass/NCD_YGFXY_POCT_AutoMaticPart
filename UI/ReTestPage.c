/***************************************************************************************************
*FileName:ReTestPage
*Description:�ϻ����Խ���
*Author: xsx_kair
*Data:2016��8��15��17:36:56
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"ReTestPage.h"
#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"SystemSetPage.h"
#include	"CodeScan_Task.h"
#include	"Test_Task.h"
#include	"Timer.h"
#include	"Ads8325_Driver.h"
#include	"LEDCheck_Driver.h"
#include	"Motor_Fun.h"
#include	"System_Data.h"
#include	"SystemSet_Data.h"
#include	"CardCheck_Driver.h"
#include	"ReTestDataDao.h"
#include	"MyTools.h"
#include	"Motor1_Driver.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static ReTestPageBuffer * pageBuffer = NULL;
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyRes activityBufferMalloc(void);
static void activityBufferFree(void);

static void dspTestInfo(void);
static void dspStatusInfo(char * infoStr, unsigned char * num);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: ����ѡ������˽���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyRes createReTestActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "ReTestActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: ��ʾ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:32
***************************************************************************************************/
static void activityStart(void)
{
//	dspReTestStatus("Stopped", NULL);
//	S_ReTestPageBuffer->isTestting = false;
//	timer_SetAndStart(&(S_ReTestPageBuffer->testTimer), 10);
	
	pageBuffer->testIndex = 0;
	pageBuffer->testNum = 0;
	timer_restart(&pageBuffer->paiduiUnitData[0].timeUp_timer);
	dspTestInfo();
	
	SelectPage(143);
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	pageBuffer->lcdinput[0] = pbuf[4];
	pageBuffer->lcdinput[0] = (pageBuffer->lcdinput[0]<<8) + pbuf[5];
		
	/*�˳�*/
	if(pageBuffer->lcdinput[0] == 0x2802)
	{
		pageBuffer->testStep = 2;			//�����˳�����
		pageBuffer->testNum = 0;
		pageBuffer->testIndex = 0;
		SendKeyCode(3);
	}
	/*��ʼ�ϻ�*/
	else if(pageBuffer->lcdinput[0] == 0x2800)
	{

	}
	/*ֹͣ�ϻ�*/
	else if(pageBuffer->lcdinput[0] == 0x2801)
	{
		
	}
	/*��ȡ�ϻ�����*/
	else if(pageBuffer->lcdinput[0] == 0x2820)
	{

	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: ����ˢ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:16
***************************************************************************************************/
static void activityFresh(void)
{
	if(pageBuffer->testStep == 0)
	{
		//find whitch
		if(pageBuffer->currentPaiduiUnitData == NULL)
		{			
			for(pageBuffer->i = 0; pageBuffer->i < PaiDuiWeiNum; pageBuffer->i++)
			{
				if(pageBuffer->paiduiUnitData[pageBuffer->i].statues == statusNull)
				{
					pageBuffer->currentPaiduiUnitData = &pageBuffer->paiduiUnitData[pageBuffer->i];
					pageBuffer->currentPaiduiUnitData->statues = statusMotorMoveWaitCard;
					pageBuffer->currentPaiduiUnitData->ledValue = getSystemTestLedLightIntensity();
					pageBuffer->currentPaiduiUnitData->index = pageBuffer->i;
					pageBuffer->currentPaiduiUnitData->cardLocation = 2*pageBuffer->i + 1;
					pageBuffer->currentPaiduiUnitData->testLocation = pageBuffer->currentPaiduiUnitData->cardLocation;
					pageBuffer->currentPaiduiUnitData->testLocation += PaiDuiWeiNum;
					if(pageBuffer->currentPaiduiUnitData->testLocation > PaiDuiWeiNum*2)
						pageBuffer->currentPaiduiUnitData->testLocation -= PaiDuiWeiNum*2;
					FormatParmAndStartMotorAction(&pageBuffer->motorAction, WaitCardPutInDef, pageBuffer->currentPaiduiUnitData->cardLocation, false);
					
					dspStatusInfo("׼����\0", &pageBuffer->currentPaiduiUnitData->index);
					break;
				}
			}
		}
		
		if(pageBuffer->currentPaiduiUnitData == NULL)
		{
			getSystemTime(&pageBuffer->paiduiUnitData[0].testData.testDateTime);
			timer_restart(&pageBuffer->paiduiUnitData[0].timeDown_timer);
			pageBuffer->testStep = 1;
			return;
		}
		
		//������Ƿ��ڲ忨λ�þ���
		if(pageBuffer->currentPaiduiUnitData->statues == statusMotorMoveWaitCard)
		{
			if(isMotorMoveEnd(FreeRTOSZeroDelay))
			{
				//not scan qr yet
				if(pageBuffer->currentPaiduiUnitData->testData.qrCode.CRC16 == 0)
				{
					pageBuffer->currentPaiduiUnitData->statues = statusWaitCardPutIn;
					dspStatusInfo("��忨\0", &pageBuffer->currentPaiduiUnitData->index);
				}
				//has scan qr
				else
				{
					pageBuffer->currentPaiduiUnitData->statues = statusWaitCardPutOut;
					dspStatusInfo("��ο�\0", &pageBuffer->currentPaiduiUnitData->index);
				}
			}
		}
		else if(pageBuffer->currentPaiduiUnitData->statues == statusWaitCardPutOut)
		{
			if(!ReadCardCheckPin)
			{
				pageBuffer->currentPaiduiUnitData->statues = statusWaitCardPutIn;
				dspStatusInfo("��忨\0", &pageBuffer->currentPaiduiUnitData->index);
			}
		}
		else if(pageBuffer->currentPaiduiUnitData->statues == statusWaitCardPutIn)
		{
			if(ReadCardCheckPin)
			{
				pageBuffer->currentPaiduiUnitData->statues = statusWaitScanQR;
				StartScanQRCode(&pageBuffer->currentPaiduiUnitData->testData.qrCode);
				dspStatusInfo("��ά���ȡ��\0", &pageBuffer->currentPaiduiUnitData->index);
			}
		}
		else if(pageBuffer->currentPaiduiUnitData->statues == statusWaitScanQR)
		{
			if(My_Pass == TakeScanQRCodeResult(&pageBuffer->cardScanResult))
			{
				if(pageBuffer->cardScanResult == CardCodeScanOK)
				{
					pageBuffer->currentPaiduiUnitData->statues = statusMotorOrigin;
					FormatParmAndStartMotorAction(&pageBuffer->motorAction, OriginLocationDef, pageBuffer->currentPaiduiUnitData->cardLocation, false);
				}
				else
				{
					pageBuffer->currentPaiduiUnitData->testData.qrCode.CRC16 = 0xff;
					pageBuffer->currentPaiduiUnitData->statues = statusMotorMoveWaitCard;
					FormatParmAndStartMotorAction(&pageBuffer->motorAction, WaitCardPutInDef, pageBuffer->currentPaiduiUnitData->cardLocation, false);
				}
			}
		}
		else if(statusMotorOrigin == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(isMotorMoveEnd(FreeRTOSZeroDelay))
			{
				pageBuffer->currentPaiduiUnitData->statues = status_start;
				pageBuffer->currentPaiduiUnitData = NULL;
			}
		}
	}
	else if(pageBuffer->testStep == 1)
	{
		if(pageBuffer->testIndex >= PaiDuiWeiNum)
		{
			//������һ�ֵĲ��Խ��
			writeReTestDataToFile(pageBuffer);
			dspTestInfo();
			
			//��ʼ��һ�ֲ���
			pageBuffer->testIndex = 0;
			pageBuffer->testNum++;
			getSystemTime(&pageBuffer->paiduiUnitData[0].testData.testDateTime);
			timer_restart(&pageBuffer->paiduiUnitData[0].timeDown_timer);
		}
		
		if(pageBuffer->currentPaiduiUnitData == NULL)
		{
			pageBuffer->currentPaiduiUnitData = &pageBuffer->paiduiUnitData[pageBuffer->testIndex];
			pageBuffer->currentPaiduiUnitData->testData.testResultDesc = NoResult;
			pageBuffer->currentPaiduiUnitData->statues = status_start;
		}
		
		if(status_start == pageBuffer->currentPaiduiUnitData->statues)
		{
			pageBuffer->currentPaiduiUnitData->statues = statusMotorMoveWaitCard;
			FormatParmAndStartMotorAction(&pageBuffer->motorAction, WaitCardPutInDef, pageBuffer->currentPaiduiUnitData->cardLocation, false);
		}
		else if(statusMotorMoveWaitCard == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(isMotorMoveEnd(FreeRTOSZeroDelay))
			{
				pageBuffer->currentPaiduiUnitData->statues = statusWaitScanQR;
				StartScanQRCode(&pageBuffer->currentPaiduiUnitData->testData.qrCode);
			}
		}
		else if(pageBuffer->currentPaiduiUnitData->statues == statusWaitScanQR)
		{
			if(My_Pass == TakeScanQRCodeResult(&pageBuffer->cardScanResult))
			{
				if(pageBuffer->cardScanResult != CardCodeScanOK)
				{
					//end error
					pageBuffer->currentPaiduiUnitData->testData.testResultDesc = qrError;
				}
				
				pageBuffer->currentPaiduiUnitData->statues = statusMotorOrigin;
				FormatParmAndStartMotorAction(&pageBuffer->motorAction, OriginLocationDef, pageBuffer->currentPaiduiUnitData->cardLocation, false);
			}
		}
		else if(statusMotorOrigin == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(isMotorMoveEnd(FreeRTOSZeroDelay))
			{
				if(NoResult == pageBuffer->currentPaiduiUnitData->testData.testResultDesc)
				{
					pageBuffer->currentPaiduiUnitData->statues = statusPrepareTest;
					FormatParmAndStartMotorAction(&pageBuffer->motorAction, StartTestDef, pageBuffer->currentPaiduiUnitData->testLocation, false);
				}
				else
				{
					pageBuffer->testIndex++;
					pageBuffer->currentPaiduiUnitData = NULL;
				}
			}
		}
		else if(statusPrepareTest == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(isMotorMoveEnd(FreeRTOSZeroDelay))
			{
				pageBuffer->currentPaiduiUnitData->statues = status_testting;
				StartTest(pageBuffer->currentPaiduiUnitData);
			}
		}
		else if(status_testting == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(My_Pass == TakeTestResult(&pageBuffer->currentPaiduiUnitData->testData.testResultDesc))
			{
				pageBuffer->currentPaiduiUnitData->statues = status_end;
				FormatParmAndStartMotorAction(&pageBuffer->motorAction, PutDownCardInTestPlaceDef, pageBuffer->currentPaiduiUnitData->testLocation, false);
			}
		}
		else if(status_end == pageBuffer->currentPaiduiUnitData->statues)
		{
			if(isMotorMoveEnd(FreeRTOSZeroDelay))
			{
				pageBuffer->testIndex++;
				pageBuffer->currentPaiduiUnitData = NULL;
			}
		}
	}
	else if(pageBuffer->testStep == 2)
	{
		if(pageBuffer->testNum == 0)
		{
			pageBuffer->currentPaiduiUnitData = NULL;
			
			if(Motor1Sensor1Triggered)
				pageBuffer->motorAction.motorActionEnum = PutCardOutOfDeviceIgnoreMotor1Def;	
			else if(Motor1Sensor2Triggered && readCaedCheckStatus() == ON)
				pageBuffer->motorAction.motorActionEnum = PutDownCardInPlaceDef;
			else
			{
				pageBuffer->testNum = 2;
				return;
			}
			
			if(My_Pass == StartMotorAction(&pageBuffer->motorAction, false))
				pageBuffer->testNum = 1;
		}
		else if(pageBuffer->testNum == 1)
		{
			if(isMotorMoveEnd(FreeRTOSZeroDelay))
				pageBuffer->testNum = 2;
		}
		else if(pageBuffer->testNum == 2)
		{
			if(pageBuffer->currentPaiduiUnitData == NULL)
			{
				for(pageBuffer->i = 0; pageBuffer->i < PaiDuiWeiNum; pageBuffer->i++)
				{
					if(pageBuffer->paiduiUnitData[pageBuffer->i].statues != statusNull)
					{
						pageBuffer->currentPaiduiUnitData = &pageBuffer->paiduiUnitData[pageBuffer->i];
						if(My_Pass == FormatParmAndStartMotorAction(&pageBuffer->motorAction, PutCardOutOfDeviceDef, pageBuffer->currentPaiduiUnitData->testLocation, false))
						{
							pageBuffer->currentPaiduiUnitData = &pageBuffer->paiduiUnitData[pageBuffer->i];
							break;
						}
						else
						{
							pageBuffer->currentPaiduiUnitData = NULL;
							return;
						}
					}
				}
			}
			
			if(pageBuffer->currentPaiduiUnitData == NULL)
				backToFatherActivity();
			else
			{
				if(isMotorMoveEnd(FreeRTOSZeroDelay))
				{
					pageBuffer->currentPaiduiUnitData->statues = statusNull;
					pageBuffer->currentPaiduiUnitData = NULL;
				}
			}
		}
	}
}

/***************************************************************************************************
*FunctionName: activityHide
*Description: ���ؽ���ʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:40
***************************************************************************************************/
static void activityHide(void)
{

}

/***************************************************************************************************
*FunctionName: activityResume
*Description: ����ָ���ʾʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:58
***************************************************************************************************/
static void activityResume(void)
{

}

/***************************************************************************************************
*FunctionName: activityDestroy
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:02:15
***************************************************************************************************/
static void activityDestroy(void)
{
	SendKeyCode(16);
	activityBufferFree();
}

/***************************************************************************************************
*FunctionName: activityBufferMalloc
*Description: ���������ڴ�����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static MyRes activityBufferMalloc(void)
{
	if(NULL == pageBuffer)
	{
		pageBuffer = MyMalloc(sizeof(ReTestPageBuffer));
		
		if(pageBuffer)
		{
			memset(pageBuffer, 0, sizeof(ReTestPageBuffer));
	
			return My_Pass;
		}
		else
			return My_Fail;
	}
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: activityBufferFree
*Description: �����ڴ��ͷ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:03:10
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(pageBuffer);
	pageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

static void dspStatusInfo(char * infoStr, unsigned char * num)
{
	if(num)
		sprintf(pageBuffer->buf, "%s %d", infoStr, *num);
	else
		sprintf(pageBuffer->buf, "%s", infoStr);
	DisText(0x2828, infoStr, strlen(infoStr));
}

static void dspTestInfo(void)
{
	//�Ѳ�
	DspNum(0x2810 , pageBuffer->testNum, 4);

	//�Ѳ�ʱ��
	DspNum(0x281a , timer_Count(&pageBuffer->paiduiUnitData[0].timeUp_timer), 4);
}

/****************************************end of file************************************************/
