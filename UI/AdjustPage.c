/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"AdjustPage.h"
#include	"LCD_Driver.h"
#include	"CardLimit_Driver.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"SystemSetPage.h"
#include	"PlaySong_Task.h"
#include	"CodeScan_Task.h"
#include	"Test_Task.h"
#include	"SDFunction.h"
#include	"Motor_Fun.h"
#include	"SystemSet_Dao.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static AdjustPageBuffer * S_AdjustPageBuffer = NULL;
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

static void dspAdjStatus(char * str);
static void dspTestResult(void);
static void dspAdjResult(void);
static void CheckQRCode(void);
static void CheckPreTestCard(void);
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
MyRes createAdjActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "AdjActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	if(S_AdjustPageBuffer)
	{		
		//��ȡ����led����ֵ
		S_AdjustPageBuffer->paiduiUnitData.ledLight = getGBSystemSetData()->testLedLightIntensity;
		
		dspAdjStatus("Waitting\0");
	}

	SelectPage(118);
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
	if(S_AdjustPageBuffer)
	{
		/*����*/
		S_AdjustPageBuffer->lcdinput[0] = pbuf[4];
		S_AdjustPageBuffer->lcdinput[0] = (S_AdjustPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*�˳�*/
		if(S_AdjustPageBuffer->lcdinput[0] == 0x2701)
		{
			if(S_AdjustPageBuffer->isAdjjing == true)
				SendKeyCode(3);
			else
				backToFatherActivity();
		}
		//��ʼ����
		else if(S_AdjustPageBuffer->lcdinput[0] == 0x270a)
		{
			if(CardPinIn == CardIN)
			{
				if(S_AdjustPageBuffer->isAdjjing == true)
					dspAdjStatus("Already Test\0");
				else
				{
					S_AdjustPageBuffer->isAdjjing = true;
					
					StartScanQRCode(&(S_AdjustPageBuffer->paiduiUnitData.testData.qrCode));
				
					dspAdjStatus("Scanning QR\0");
				}
			}
			else
			{
				dspAdjStatus("No Card\0");
			}
		}
		/*У׼*/
		else if(S_AdjustPageBuffer->lcdinput[0] == 0x2702)
		{
			if((S_AdjustPageBuffer->paiduiUnitData.testData.testSeries.BasicResult != 0) && (S_AdjustPageBuffer->targetresult != 0))
			{
				S_AdjustPageBuffer->adjustData.parm = S_AdjustPageBuffer->targetresult / S_AdjustPageBuffer->paiduiUnitData.testData.testSeries.BasicResult;

				dspAdjResult();
				
				dspAdjStatus("Success\0");
			}
		}
		/*�����׼ֵ*/
		else if(S_AdjustPageBuffer->lcdinput[0] == 0x2740)
		{
			memset(S_AdjustPageBuffer->buf, 0 , 100);
			memcpy(S_AdjustPageBuffer->buf, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_AdjustPageBuffer->targetresult = strtod(S_AdjustPageBuffer->buf, NULL);
		}
		//����У׼���
		else if(S_AdjustPageBuffer->lcdinput[0] == 0x2700)
		{
			//��ȡ���µ�ϵͳ����
			memcpy(&(S_AdjustPageBuffer->systemSetData), getGBSystemSetData(), SystemSetDataStructSize);
			
			//�޸�ϵͳ���������ж�����Ŀ��У׼ֵ
			if(My_Pass == addAdjPram(&(S_AdjustPageBuffer->systemSetData), &(S_AdjustPageBuffer->adjustData)))
			{
				if(My_Pass == SaveSystemSetData(&(S_AdjustPageBuffer->systemSetData)))
					SendKeyCode(1);
				else
					SendKeyCode(2);
			}
			else
				SendKeyCode(2);
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
	if(S_AdjustPageBuffer)
	{
		/*�Ƿ�忨*/
		if(CardPinIn == NoCard)
		{
			if(S_AdjustPageBuffer->isAdjjing == false)
			{
				dspAdjStatus("Waitting\0");
			}
		}
		
		CheckQRCode();
		
		CheckPreTestCard();
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
	if(NULL == S_AdjustPageBuffer)
	{
		S_AdjustPageBuffer = MyMalloc(sizeof(AdjustPageBuffer));
		
		if(S_AdjustPageBuffer)
		{
			memset(S_AdjustPageBuffer, 0, sizeof(AdjustPageBuffer));
	
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
	MyFree(S_AdjustPageBuffer);
	S_AdjustPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

static void dspAdjStatus(char * str)
{
	memset(S_AdjustPageBuffer->buf, 0, 20);
	sprintf(S_AdjustPageBuffer->buf, "%-15s", str);
	DisText(0x2710, S_AdjustPageBuffer->buf, strlen(S_AdjustPageBuffer->buf));
}

static void dspTestResult(void)
{
	sprintf(S_AdjustPageBuffer->buf, "(%d,%d)", S_AdjustPageBuffer->paiduiUnitData.testData.testSeries.T_Point[1], S_AdjustPageBuffer->paiduiUnitData.testData.testSeries.T_Point[0]);
	DisText(0x2720, S_AdjustPageBuffer->buf, strlen(S_AdjustPageBuffer->buf));
	
	sprintf(S_AdjustPageBuffer->buf, "(%d,%d)", S_AdjustPageBuffer->paiduiUnitData.testData.testSeries.C_Point[1], S_AdjustPageBuffer->paiduiUnitData.testData.testSeries.C_Point[0]);
	DisText(0x2728, S_AdjustPageBuffer->buf, strlen(S_AdjustPageBuffer->buf));
	
	sprintf(S_AdjustPageBuffer->buf, "%.3f", S_AdjustPageBuffer->paiduiUnitData.testData.testSeries.BasicBili);
	DisText(0x2730, S_AdjustPageBuffer->buf, strlen(S_AdjustPageBuffer->buf));
	
	sprintf(S_AdjustPageBuffer->buf, "%.*f", S_AdjustPageBuffer->paiduiUnitData.testData.qrCode.itemConstData.pointNum, 
		S_AdjustPageBuffer->paiduiUnitData.testData.testSeries.BasicResult);
	DisText(0x2738, S_AdjustPageBuffer->buf, strlen(S_AdjustPageBuffer->buf));
}

static void dspAdjResult(void)
{
	sprintf(S_AdjustPageBuffer->buf, "%.3f", S_AdjustPageBuffer->adjustData.parm);
	DisText(0x2748, S_AdjustPageBuffer->buf, strlen(S_AdjustPageBuffer->buf));
	
	sprintf(S_AdjustPageBuffer->buf, "%.*f", S_AdjustPageBuffer->paiduiUnitData.testData.qrCode.itemConstData.pointNum, 
		S_AdjustPageBuffer->adjResult);
	DisText(0x2750, S_AdjustPageBuffer->buf, strlen(S_AdjustPageBuffer->buf));
}

static void CheckQRCode(void)
{
	if(My_Pass == TakeScanQRCodeResult(&(S_AdjustPageBuffer->scancode)))
	{
		//��ά���ȡʧ��
		if((S_AdjustPageBuffer->scancode == CardCodeScanFail) || (S_AdjustPageBuffer->scancode == CardCodeCardOut) ||
			(S_AdjustPageBuffer->scancode == CardCodeScanTimeOut) || (S_AdjustPageBuffer->scancode == CardCodeCRCError))
		{
			MotorMoveTo(MaxLocation, 1);
			S_AdjustPageBuffer->isAdjjing = false;
			dspAdjStatus("Scan Fail\0");
		}
		//����
		else if(S_AdjustPageBuffer->scancode == CardCodeTimeOut)
		{
			MotorMoveTo(MaxLocation, 1);
			S_AdjustPageBuffer->isAdjjing = false;
			dspAdjStatus("Out Of Date\0");
		}
		//��ȡ�ɹ�
		else if(S_AdjustPageBuffer->scancode == CardCodeScanOK)
		{
			memset(S_AdjustPageBuffer->buf, 0, 20);
			sprintf(S_AdjustPageBuffer->buf, "%s", S_AdjustPageBuffer->paiduiUnitData.testData.qrCode.ItemName);
			DisText(0x2718, S_AdjustPageBuffer->buf, strlen(S_AdjustPageBuffer->buf));
			
			dspAdjStatus("Read Card\0");
			
			//������Ŀ���Ƶ�ǰAdjItemNameLen���ַ�
			memcpy(S_AdjustPageBuffer->adjustData.ItemName, S_AdjustPageBuffer->paiduiUnitData.testData.qrCode.ItemName, ItemNameLen);
			
			//���Խ��
			StartTest(&(S_AdjustPageBuffer->paiduiUnitData.testData));
		}
	}
}

static void CheckPreTestCard(void)
{
	if(My_Pass == TakeTestResult(&(S_AdjustPageBuffer->cardTestResult)))
	{
		MotorMoveTo(MaxLocation, 1);
		S_AdjustPageBuffer->isAdjjing = false;
		dspTestResult();
	}
}


