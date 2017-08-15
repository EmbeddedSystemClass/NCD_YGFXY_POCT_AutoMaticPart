/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"PreReadCardPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"MyTest_Data.h"
#include 	"MLX90614_Driver.h"
#include	"LunchPage.h"
#include	"CodeScan_Task.h"
#include	"WaittingCardPage.h"
#include	"TimeDownNorPage.h"
#include	"CodeScanFunction.h"
#include	"PaiDuiPage.h"
#include	"Motor_Fun.h"
#include	"PlaySong_Task.h"
#include	"Test_Task.h"
#include	"SDFunction.h"
#include	"MyTools.h"
#include	"CRC16.h"
#include	"System_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static PreReadPageBuffer * S_PreReadPageBuffer = NULL;
const unsigned int TestLineHigh1 = 77010;	//��������������ʾ����߶��йأ�������治�ģ���������
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyRes activityBufferMalloc(void);
static void activityBufferFree(void);

static void clearPageText(void);
static void CheckQRCode(void);
static void ShowCardInfo(void);
static void CheckPreTestCard(void);
static void showTemperature(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: ����ѡ������˽���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyRes createPreReadCardActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "PreReadCardActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	if(S_PreReadPageBuffer)
	{
		S_PreReadPageBuffer->currenttestdata = GetCurrentTestItem();
		
		clearPageText();
		
		clearScanQRCodeResult();
		clearTestResult();
		
		vTaskDelay(500 / portTICK_RATE_MS);
		StartScanQRCode(&(S_PreReadPageBuffer->temperweima));
	}
	
	SelectPage(92);
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
	if(S_PreReadPageBuffer)
	{
		/*����*/
		S_PreReadPageBuffer->lcdinput[0] = pbuf[4];
		S_PreReadPageBuffer->lcdinput[0] = (S_PreReadPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*��ά���ȡʧ�ܣ����ڣ���ʹ��*/
		if((S_PreReadPageBuffer->lcdinput[0] >= 0x1400) && (S_PreReadPageBuffer->lcdinput[0] <= 0x1405))
		{
			/*����*/
			S_PreReadPageBuffer->lcdinput[1] = pbuf[7];
			S_PreReadPageBuffer->lcdinput[1] = (S_PreReadPageBuffer->lcdinput[1]<<8) + pbuf[8];
			
			/*������⿨*/
			if(S_PreReadPageBuffer->lcdinput[1] == 0x0001)
			{
				//������Ŷ��е��ٴ�Ԥ�����򷵻��Ŷӽ��棬״̬�л���֮ǰ��״̬
				if(S_PreReadPageBuffer->currenttestdata->statues == status_prereadagain_n)
					S_PreReadPageBuffer->currenttestdata->statues = status_incard_n;
				else if(S_PreReadPageBuffer->currenttestdata->statues == status_prereadagain_o)
					S_PreReadPageBuffer->currenttestdata->statues = status_incard_o;
				//����ǵ�һ��Ԥ��
				else if(S_PreReadPageBuffer->currenttestdata->statues == status_wait1)
					S_PreReadPageBuffer->currenttestdata->statues = status_wait1;
				
				backToFatherActivity();
			}
			//ȡ������
			else if(S_PreReadPageBuffer->lcdinput[1] == 0x0000)
			{
				DeleteCurrentTest();
				
				//������п����Ŷӣ�˵����������Ǵ��Ŷӽ�������ģ�ֻ�践�ص��Ŷӽ���
				if(IsPaiDuiTestting())
					backToActivity(paiduiActivityName);	
				else
					backToActivity(lunchActivityName);
			}
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
	CheckQRCode();
		
	CheckPreTestCard();
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
	if(NULL == S_PreReadPageBuffer)
	{
		S_PreReadPageBuffer = MyMalloc(sizeof(PreReadPageBuffer));
		
		if(S_PreReadPageBuffer)
		{
			memset(S_PreReadPageBuffer, 0, sizeof(PreReadPageBuffer));
	
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
	MyFree(S_PreReadPageBuffer);
	S_PreReadPageBuffer = NULL;
}



static void CheckQRCode(void)
{
	if(My_Pass == TakeScanQRCodeResult(&(S_PreReadPageBuffer->scancode)))
	{	
		//��֧�ֵ�Ʒ��
		if(S_PreReadPageBuffer->scancode == CardUnsupported)
		{
			MotorMoveTo(MaxLocation, 1);
			AddNumOfSongToList(56, 0);
			SendKeyCode(6);
		}
		//����
		else if(S_PreReadPageBuffer->scancode == CardCodeTimeOut)
		{
			MotorMoveTo(MaxLocation, 1);
			AddNumOfSongToList(15, 0);
			SendKeyCode(4);
		}
		//��ȡ�ɹ�
		else if(S_PreReadPageBuffer->scancode == CardCodeScanOK)
		{
			ShowCardInfo();
			
			//��ȡ���ά����Լ����ڿ����ڲ�����ʱ��ȡ�¶ȱȽϺ���
			showTemperature();
			
			//����ǵ�һ�ζ�ȡ��ά��
			if(S_PreReadPageBuffer->currenttestdata->statues == status_preread)
			{
				//����ȡ�Ķ�ά�����ݿ������������ݰ���
				memcpy(&(S_PreReadPageBuffer->currenttestdata->testData.qrCode), &(S_PreReadPageBuffer->temperweima), sizeof(QRCode));
				
				//���õ���ʱʱ��
				timer_set(&(S_PreReadPageBuffer->currenttestdata->timer), S_PreReadPageBuffer->currenttestdata->testData.qrCode.CardWaitTime*60);
			
				//��ȡУ׼����
				memcpy(S_PreReadPageBuffer->currenttestdata->testData.adjustData.ItemName, S_PreReadPageBuffer->currenttestdata->testData.qrCode.ItemName, ItemNameLen);
				getAdjPram(getGBSystemSetData(), &(S_PreReadPageBuffer->currenttestdata->testData.adjustData));
				
				S_PreReadPageBuffer->preTestErrorCount = 0;
				StartTest(&(S_PreReadPageBuffer->currenttestdata->testData));
			}
			else
			{
				//У���Ի������
				if((pdPASS == CheckStrIsSame(S_PreReadPageBuffer->currenttestdata->testData.qrCode.PiHao, S_PreReadPageBuffer->temperweima.PiHao, 15)) &&
					(pdPASS == CheckStrIsSame(S_PreReadPageBuffer->currenttestdata->testData.qrCode.piNum, S_PreReadPageBuffer->temperweima.piNum, 5)))
				{
					startActivity(createTimeDownActivity, NULL, NULL);
				}
				//�Լ������
				else
				{
					vTaskDelay(100 / portTICK_RATE_MS);
					MotorMoveTo(MaxLocation, 1);
					AddNumOfSongToList(13, 0);
					SendKeyCode(2);
				}
			}
		}
		/*��������CardCodeScanFail, CardCodeCardOut, CardCodeScanTimeOut, CardCodeCRCError*/
		else
		{
			MotorMoveTo(MaxLocation, 1);
			AddNumOfSongToList(12, 0);
			SendKeyCode(1);
		}
	}
}

static void CheckPreTestCard(void)
{
	if(My_Pass == TakeTestResult(&(S_PreReadPageBuffer->cardpretestresult)))
	{
		timer_restart(&(S_PreReadPageBuffer->currenttestdata->timer));
		
		//δ����
		if(S_PreReadPageBuffer->cardpretestresult == NoSample)
		{
			//δ�����ز�3�Σ�������δ������������δ����
			S_PreReadPageBuffer->preTestErrorCount++;
			if(S_PreReadPageBuffer->preTestErrorCount < 8)
			{	
				StartTest(&(S_PreReadPageBuffer->currenttestdata->testData));
			}
			else
			{
				MotorMoveTo(MaxLocation, 1);
				AddNumOfSongToList(16, 0);
				SendKeyCode(5);
			}
		}
		else if(S_PreReadPageBuffer->cardpretestresult == ResultIsOK)
		{
			MotorMoveTo(MaxLocation, 1);
			AddNumOfSongToList(14, 0);
			SendKeyCode(3);
		}
		else if(S_PreReadPageBuffer->cardpretestresult == PeakError)
		{
			//������Ŷ�ģʽ��������Ŷӽ���
			if(S_PreReadPageBuffer->currenttestdata->testlocation > 0)
			{
				MotorMoveTo(MaxLocation, 1);
				
				S_PreReadPageBuffer->currenttestdata->statues = status_start;

				startActivity(createPaiDuiActivity, NULL, NULL);
			}
			else
			{		
				startActivity(createTimeDownActivity, NULL, NULL);
			}
		}
		
	}
}

static void clearPageText(void)
{
	ClearText(0x1420);
	ClearText(0x1430);
	ClearText(0x1440);
	ClearText(0x1450);
	ClearText(0x1460);
	ClearText(0x1470);
}

static void ShowCardInfo(void)
{
	sprintf(S_PreReadPageBuffer->buf, "%s\0", S_PreReadPageBuffer->temperweima.ItemName);
	DisText(0x1420, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
		
	sprintf(S_PreReadPageBuffer->buf, "%s-%s\0", S_PreReadPageBuffer->temperweima.PiHao, S_PreReadPageBuffer->temperweima.piNum);
	DisText(0x1430, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
	
	sprintf(S_PreReadPageBuffer->buf, "%d S\0", S_PreReadPageBuffer->temperweima.CardWaitTime*60);
	DisText(0x1440, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
		
	sprintf(S_PreReadPageBuffer->buf, "20%02d��%02d��%02d��\0", S_PreReadPageBuffer->temperweima.CardBaoZhiQi.year, 
		S_PreReadPageBuffer->temperweima.CardBaoZhiQi.month, S_PreReadPageBuffer->temperweima.CardBaoZhiQi.day);
	DisText(0x1450, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
}

static void showTemperature(void)
{
	//��ȡ��⿨�¶�
	S_PreReadPageBuffer->currenttestdata->testData.temperature.O_Temperature = GetCardTemperature();
	S_PreReadPageBuffer->currenttestdata->testData.temperature.E_Temperature = getSystemRunTimeData()->enTemperature;
	
	sprintf(S_PreReadPageBuffer->buf, "%2.1f ��\0", S_PreReadPageBuffer->currenttestdata->testData.temperature.O_Temperature);
	DisText(0x1460, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
	
	sprintf(S_PreReadPageBuffer->buf, "%2.1f ��\0",S_PreReadPageBuffer->currenttestdata->testData.temperature.E_Temperature);
	DisText(0x1470, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf)+1);
}

