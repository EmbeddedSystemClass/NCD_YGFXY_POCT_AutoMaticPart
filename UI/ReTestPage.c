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
#include	"CardLimit_Driver.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"SystemSetPage.h"
#include	"PlaySong_Task.h"
#include	"CodeScan_Task.h"
#include	"Test_Task.h"
#include	"SDFunction.h"
#include	"Timer.h"
#include	"OutModel_Fun.h"
#include	"PlaySong_Task.h"
#include	"Ads8325_Driver.h"
#include	"TLV5617_Driver.h"
#include	"LEDCheck_Driver.h"
#include 	"MLX90614_Driver.h"
#include	"Motor_Fun.h"
#include	"System_Data.h"
#include	"SystemSet_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static ReTestPageBuffer * S_ReTestPageBuffer = NULL;
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

static void SetReTestCount(char *str, unsigned char len);
static void StartReTest(void);
static void StopReTest(void);
static void CheckQRCode(void);
static void CheckTestCard(void);

static void dspReTestStatus(char * str);
static void EndOneReTest(char *result);
static void dspReTestLogs(void);
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
	if(S_ReTestPageBuffer)
	{
		
		timer_set(&(S_ReTestPageBuffer->timer), 10);
		
		dspReTestStatus("Stand By\0");
	}
	
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
	if(S_ReTestPageBuffer)
	{
		/*����*/
		S_ReTestPageBuffer->lcdinput[0] = pbuf[4];
		S_ReTestPageBuffer->lcdinput[0] = (S_ReTestPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*�˳�*/
		if(S_ReTestPageBuffer->lcdinput[0] == 0x2802)
		{
			if(S_ReTestPageBuffer->reTestData.reteststatus == 0)
				backToFatherActivity();
			else
				SendKeyCode(1);
		}
		/*��ʼ�ϻ�*/
		else if(S_ReTestPageBuffer->lcdinput[0] == 0x2800)
		{
			if((S_ReTestPageBuffer->reTestData.retestcount == 0) || (S_ReTestPageBuffer->reTestData.reteststatus > 0))
				SendKeyCode(1);
			else
				StartReTest();
		}
		/*ֹͣ�ϻ�*/
		else if(S_ReTestPageBuffer->lcdinput[0] == 0x2801)
		{
			S_ReTestPageBuffer->reTestData.retestsurpluscount = 0;
		}
		/*��ȡ�ϻ�����*/
		else if(S_ReTestPageBuffer->lcdinput[0] == 0x2820)
		{
			SetReTestCount((char *)(&pbuf[7]), GetBufLen(&pbuf[7] , 2*pbuf[6]));
		}
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
	if(S_ReTestPageBuffer->reTestData.reteststatus > 0)
	{
		if(S_ReTestPageBuffer->reTestData.retestStep == 1)
		{
			if(CardPinIn == CardIN)
			{
				//��⵽�忨���򱣳��¶Ⱥ�ʱ��
				S_ReTestPageBuffer->reTestData.paiduiUnitData.testData.temperature.O_Temperature = GetCardTemperature();
				S_ReTestPageBuffer->reTestData.paiduiUnitData.testData.temperature.E_Temperature = getSystemRunTimeData()->enTemperature;
				memcpy(&(S_ReTestPageBuffer->reTestData.paiduiUnitData.testData.testDateTime), &(getSystemRunTimeData()->systemDateTime), sizeof(DateTime));
				
				//�ϻ�AD,DA
				SetGB_LedValue(100);
				vTaskDelay(100 / portTICK_RATE_MS);
				S_ReTestPageBuffer->reTestData.advalue1 = ADS8325();
				
				SetGB_LedValue(200);
				vTaskDelay(100 / portTICK_RATE_MS);
				S_ReTestPageBuffer->reTestData.advalue2 = ADS8325();
				
				SetGB_LedValue(300);
				vTaskDelay(100 / portTICK_RATE_MS);
				S_ReTestPageBuffer->reTestData.advalue3 = ADS8325();
				
				S_ReTestPageBuffer->reTestData.ledstatus = ReadLEDStatus();
				
				S_ReTestPageBuffer->reTestData.retestStep = 2;
				StartScanQRCode(&(S_ReTestPageBuffer->reTestData.paiduiUnitData.testData.qrCode));
			}
			else if(TimeOut == timer_expired(&(S_ReTestPageBuffer->timer)))
			{
				dspReTestStatus("No Card\0");
				EndOneReTest("�ȴ���⿨��ʱ\0");
				S_ReTestPageBuffer->reTestData.retestsurpluscount = 0;
				S_ReTestPageBuffer->reTestData.retestStep = 0;
			}
		}
		else if(S_ReTestPageBuffer->reTestData.retestStep == 2)
			CheckQRCode();
		else if(S_ReTestPageBuffer->reTestData.retestStep == 3)
			CheckTestCard();

		//������Ƶ����
		if(0 == getPlayStatus())
		{
			if(timerIsStartted(&(S_ReTestPageBuffer->reTestData.oneplaytimer)))
			{
				memcpy(&(S_ReTestPageBuffer->reTestData.endplayTime), &(getSystemRunTimeData()->systemDateTime), sizeof(DateTime));
					
				//���浱ǰ������Ϣ
				if(My_Pass == SavereTestData(&(S_ReTestPageBuffer->reTestData), 1))
				{
					timer_stop(&(S_ReTestPageBuffer->reTestData.oneplaytimer));	
					
					//���ʣ����Դ�������0����ֹͣ�ϻ�����
					if(S_ReTestPageBuffer->reTestData.retestsurpluscount == 0)
						StopReTest();
				}
				else
				{
					dspReTestStatus("���沥����Ϣʧ��\0");

					StopReTest();
				}
			}
			else
			{
				S_ReTestPageBuffer->reTestData.playcount++;
				memcpy(&(S_ReTestPageBuffer->reTestData.startplayTime), &(getSystemRunTimeData()->systemDateTime), DateTimeStructSize);
				AddNumOfSongToList(55, 3);
				timer_set(&(S_ReTestPageBuffer->reTestData.oneplaytimer), 999999);
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
	if(NULL == S_ReTestPageBuffer)
	{
		S_ReTestPageBuffer = MyMalloc(sizeof(ReTestPageBuffer));
		
		if(S_ReTestPageBuffer)
		{
			memset(S_ReTestPageBuffer, 0, sizeof(ReTestPageBuffer));
	
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
	MyFree(S_ReTestPageBuffer);
	S_ReTestPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void SetReTestCount(char *str, unsigned char len)
{
	if(S_ReTestPageBuffer->reTestData.reteststatus == 0)
	{
		memset(S_ReTestPageBuffer->buf, 0, 100);
		memcpy(S_ReTestPageBuffer->buf, str, len);
		S_ReTestPageBuffer->reTestData.retestcount = strtol(S_ReTestPageBuffer->buf, NULL, 10);
		if(S_ReTestPageBuffer->reTestData.retestcount > 60000)
			S_ReTestPageBuffer->reTestData.retestcount = 60000;
	}
		
	memset(S_ReTestPageBuffer->buf, 0, 100);
	sprintf(S_ReTestPageBuffer->buf, "%d", S_ReTestPageBuffer->reTestData.retestcount);
	DisText(0x2820, S_ReTestPageBuffer->buf, strlen(S_ReTestPageBuffer->buf));
}
static void StartReTest(void)
{	
	//�����ϻ�����״̬
	dspReTestStatus("Testting\0");
		
	//�������ϻ�����
	S_ReTestPageBuffer->reTestData.retestedcount = 0;
	//����ʣ���ϻ�����
	S_ReTestPageBuffer->reTestData.retestsurpluscount = S_ReTestPageBuffer->reTestData.retestcount - S_ReTestPageBuffer->reTestData.retestedcount;

	//��ʼ��������ʱ����ʱ��
	timer_set(&(S_ReTestPageBuffer->reTestData.retesttimer), 999999);	
	//��ʼ����ǰ���Ե�ʱ����ʱ��
	timer_set(&(S_ReTestPageBuffer->reTestData.oneretesttimer), 999999);
	
	S_ReTestPageBuffer->reTestData.retestStep = 1;
	
	dspReTestLogs();
	
	//�����Ŷ�ģ��
	for(S_ReTestPageBuffer->buf[0]=0; S_ReTestPageBuffer->buf[0]<PaiDuiWeiNum; S_ReTestPageBuffer->buf[0]++)
		UpOneModelData(S_ReTestPageBuffer->buf[0], R_ON_G_OFF, R_OFF_G_ON, 5);
			
	//������Ƶ����
	timer_set(&(S_ReTestPageBuffer->reTestData.playtimer), 999999);
	S_ReTestPageBuffer->reTestData.playcount = 0;
	
	//�������в���
	S_ReTestPageBuffer->reTestData.reteststatus = 1;
	
}

static void StopReTest(void)
{
	//ֹͣ�Ŷ�ģ��
	OutModel_Init();
	
	//ֹͣ���в���
	S_ReTestPageBuffer->reTestData.reteststatus = 0;
}


static void EndOneReTest(char *result)
{
	memset(S_ReTestPageBuffer->reTestData.result, 0, 30);
	memcpy(S_ReTestPageBuffer->reTestData.result, result, strlen(result));
		
	//���浱ǰ���Խ��
	if(My_Pass == SavereTestData(&(S_ReTestPageBuffer->reTestData), 0))
	{
		//����ϴβ�������
		memset(&(S_ReTestPageBuffer->reTestData.paiduiUnitData), 0, sizeof(PaiduiUnitData));
			
		if(S_ReTestPageBuffer->reTestData.retestedcount < S_ReTestPageBuffer->reTestData.retestcount)
			S_ReTestPageBuffer->reTestData.retestedcount++;
		
		if(S_ReTestPageBuffer->reTestData.retestsurpluscount > 0)		
			S_ReTestPageBuffer->reTestData.retestsurpluscount = S_ReTestPageBuffer->reTestData.retestcount - S_ReTestPageBuffer->reTestData.retestedcount;
		
			
		dspReTestLogs();
			
		//���Դ�����
		if(S_ReTestPageBuffer->reTestData.retestsurpluscount == 0)
		{
			dspReTestStatus("Finish\0");
		}
		else
			S_ReTestPageBuffer->reTestData.retestStep = 1;
			
		//��ʼ����ǰ���Ե�ʱ����ʱ��
		timer_restart(&(S_ReTestPageBuffer->reTestData.oneretesttimer));
			
		//��ʼ���忨��ʱ
		timer_set(&(S_ReTestPageBuffer->timer), 10);
	}
	else
	{
		//״̬
		dspReTestStatus("Test Data Save Failed\0");
			
		S_ReTestPageBuffer->reTestData.retestsurpluscount = 0;
		S_ReTestPageBuffer->reTestData.retestStep = 4;
	}
}

static void CheckQRCode(void)
{
	if(My_Pass == TakeScanQRCodeResult(&(S_ReTestPageBuffer->scancode)))
	{
		if(S_ReTestPageBuffer->scancode == CardUnsupported)
		{
			dspReTestStatus("Unsupported\0");
			EndOneReTest("��֧�ֵ��Լ���\0");
			S_ReTestPageBuffer->reTestData.retestsurpluscount = 0;
			S_ReTestPageBuffer->reTestData.retestStep = 4;
			MotorMoveTo(MaxLocation, 0);
		}
		//����
		else if(S_ReTestPageBuffer->scancode == CardCodeTimeOut)
		{
			dspReTestStatus("Card Out Date\0");
			EndOneReTest("��ά���ȡ����\0");
			S_ReTestPageBuffer->reTestData.retestsurpluscount = 0;
			S_ReTestPageBuffer->reTestData.retestStep = 4;
			MotorMoveTo(MaxLocation, 0);
		}
		//��ȡ�ɹ�
		else if(S_ReTestPageBuffer->scancode == CardCodeScanOK)
		{
			//��ȡУ׼����
			memcpy(S_ReTestPageBuffer->reTestData.paiduiUnitData.testData.adjustData.ItemName, S_ReTestPageBuffer->reTestData.paiduiUnitData.testData.qrCode.ItemName, ItemNameLen);
			getAdjPram(getGBSystemSetData(), &(S_ReTestPageBuffer->reTestData.paiduiUnitData.testData.adjustData));

			S_ReTestPageBuffer->reTestData.retestStep = 3;
			S_ReTestPageBuffer->reTestData.paiduiUnitData.ledLight = getGBSystemSetData()->testLedLightIntensity;
			StartTest(&(S_ReTestPageBuffer->reTestData.paiduiUnitData.testData));
		}
		/*��������CardCodeScanFail, CardCodeCardOut, CardCodeScanTimeOut, CardCodeCRCError*/
		else
		{
			dspReTestStatus("QR Read Fail\0");
			EndOneReTest("��ά���ȡʧ��\0");
			S_ReTestPageBuffer->reTestData.retestsurpluscount = 0;
			S_ReTestPageBuffer->reTestData.retestStep = 4;
			MotorMoveTo(MaxLocation, 0);
		}
	}
}

static void CheckTestCard(void)
{
	if(My_Pass == TakeTestResult(&(S_ReTestPageBuffer->cardpretestresult)))
	{
		EndOneReTest("��������\0");
		MotorMoveTo(MaxLocation, 0);
	}
}

static void dspReTestStatus(char * str)
{
	sprintf(S_ReTestPageBuffer->buf, "%s\0", str);
	DisText(0x2828, S_ReTestPageBuffer->buf, strlen(S_ReTestPageBuffer->buf)+1);
}

static void dspReTestLogs(void)
{
	//�Ѳ�
	DspNum(0x2810 , S_ReTestPageBuffer->reTestData.retestedcount, 4);
	//ʣ��
	DspNum(0x2815 , S_ReTestPageBuffer->reTestData.retestsurpluscount, 4);
	//�Ѳ�ʱ��
	DspNum(0x281a , timer_Count(&(S_ReTestPageBuffer->reTestData.retesttimer)), 4);
}
/****************************************end of file************************************************/
