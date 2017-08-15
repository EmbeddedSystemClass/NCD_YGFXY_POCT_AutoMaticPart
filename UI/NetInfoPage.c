/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"NetInfoPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"SDFunction.h"
#include	"NetPreSetPage.h"
#include	"MyMem.h"
#include	"WifiFunction.h"
#include	"SleepPage.h"
#include	"System_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static NetInfoPageBuffer *S_NetInfoPageBuffer = NULL;
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

static void ReadNetInfo(void);
static void ShowNetInfo(void);
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
MyRes createNetInfoActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "NetInfoActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	if(S_NetInfoPageBuffer)
	{		
		timer_set(&(S_NetInfoPageBuffer->timer), 10);
	
		SelectPage(145);
		
		ReadNetInfo();
		
		ShowNetInfo();	
	}

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
	if(S_NetInfoPageBuffer)
	{
		/*����*/
		S_NetInfoPageBuffer->lcdinput[0] = pbuf[4];
		S_NetInfoPageBuffer->lcdinput[0] = (S_NetInfoPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		//����
		if(S_NetInfoPageBuffer->lcdinput[0] == 0x1ca0)
		{
			RestartWifi();
			
			backToFatherActivity();
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
	if(S_NetInfoPageBuffer)
	{
		if(TimeOut == timer_expired(&(S_NetInfoPageBuffer->timer)))
		{
			ReadNetInfo();
			ShowNetInfo();
			timer_restart(&(S_NetInfoPageBuffer->timer));
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
	if(S_NetInfoPageBuffer)
	{
		timer_restart(&(S_NetInfoPageBuffer->timer));
	}
	
	SelectPage(145);
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
	
	giveWifixMutex();
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
	if(NULL == S_NetInfoPageBuffer)
	{
		S_NetInfoPageBuffer = MyMalloc(sizeof(NetInfoPageBuffer));
		
		if(S_NetInfoPageBuffer)
		{
			memset(S_NetInfoPageBuffer, 0, sizeof(NetInfoPageBuffer));
	
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
	MyFree(S_NetInfoPageBuffer);
	S_NetInfoPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: ReadNetInfo
*Description: ��ȡ������Ϣ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��5��15:38:30
***************************************************************************************************/
static void ReadNetInfo(void)
{
	S_NetInfoPageBuffer->wifiico.ICO_ID = 32;
	S_NetInfoPageBuffer->wifiico.X = 189;
	S_NetInfoPageBuffer->wifiico.Y = 315;
	
	memset(&(S_NetInfoPageBuffer->WifiIP), 0, sizeof(IP));
	memset(S_NetInfoPageBuffer->WifiSSID, 0, 30);
		
	memset(S_NetInfoPageBuffer->WifiMAC, 0, 13);
	
	if(S_NetInfoPageBuffer->isGetWifiControl == false)
	{
		if(My_Fail == takeWifiMutex(100 / portTICK_RATE_MS))
		{
			goto END;
		}
		else
			S_NetInfoPageBuffer->isGetWifiControl = true;
	}
	/*�������atģʽ�������atģʽ*/
	SetWifiWorkInAT(AT_Mode);
	
	if(My_Pass == WifiIsConnectted(S_NetInfoPageBuffer->WifiSSID))
	{
		//��ȡIP
		GetWifiStaIP(&(S_NetInfoPageBuffer->WifiIP));
		
		//��ȡmac
		GetWifiStaMac(S_NetInfoPageBuffer->WifiMAC);
		
		//��ȡ�ź�ǿ��
		S_NetInfoPageBuffer->WifiIndicator = GetWifiIndicator();
		
		if(S_NetInfoPageBuffer->WifiIndicator <= 10)
			S_NetInfoPageBuffer->wifiico.ICO_ID = 36;
		else if(S_NetInfoPageBuffer->WifiIndicator < 40)
			S_NetInfoPageBuffer->wifiico.ICO_ID = 35;
		else if(S_NetInfoPageBuffer->WifiIndicator < 70)
			S_NetInfoPageBuffer->wifiico.ICO_ID = 34;
		else
			S_NetInfoPageBuffer->wifiico.ICO_ID = 33;
		
		//��ʾwifi״̬ͼ��
		BasicUI(0x1CB8 ,0x1807 , 1, &(S_NetInfoPageBuffer->wifiico) , sizeof(Basic_ICO));
		//��ʾssid
		memset(S_NetInfoPageBuffer->tempbuffer1, 0, 100);
		sprintf(S_NetInfoPageBuffer->tempbuffer1, "%s", S_NetInfoPageBuffer->WifiSSID);
		DisText(0x1Cf0, S_NetInfoPageBuffer->tempbuffer1, 30);
		
		//��ʾip
		sprintf(S_NetInfoPageBuffer->tempbuffer1, "%03d.%03d.%03d.%03d\0", S_NetInfoPageBuffer->WifiIP.ip_1, S_NetInfoPageBuffer->WifiIP.ip_2, 
			S_NetInfoPageBuffer->WifiIP.ip_3, S_NetInfoPageBuffer->WifiIP.ip_4);
		DisText(0x1CC8, S_NetInfoPageBuffer->tempbuffer1, 15);
		//��ʾmac
		sprintf(S_NetInfoPageBuffer->tempbuffer1, "%.2s-%.2s-%.2s-%.2s-%.2s-%.2s\0", S_NetInfoPageBuffer->WifiMAC, &(S_NetInfoPageBuffer->WifiMAC[2]), &(S_NetInfoPageBuffer->WifiMAC[4]),
			&(S_NetInfoPageBuffer->WifiMAC[6]), &(S_NetInfoPageBuffer->WifiMAC[8]), &(S_NetInfoPageBuffer->WifiMAC[10]));
		DisText(0x1Ce0, S_NetInfoPageBuffer->tempbuffer1, 20);
		
		return;
	}
	
	END:
		//��ʾwifi״̬ͼ��
		BasicUI(0x1CB8 ,0x1807 , 0, &(S_NetInfoPageBuffer->wifiico) , sizeof(Basic_ICO));
		//��ʾssid
		sprintf(S_NetInfoPageBuffer->tempbuffer1, "Disconnected\0");
		DisText(0x1Cf0, S_NetInfoPageBuffer->tempbuffer1, strlen(S_NetInfoPageBuffer->tempbuffer1)+1);
			
		//��ʾip
		sprintf(S_NetInfoPageBuffer->tempbuffer1, "000.000.000.000\0");
		DisText(0x1CC8, S_NetInfoPageBuffer->tempbuffer1, 15);
		//��ʾmac
		sprintf(S_NetInfoPageBuffer->tempbuffer1, "00-00-00-00-00-00\0");
		DisText(0x1Ce0, S_NetInfoPageBuffer->tempbuffer1, 20);
}

/***************************************************************************************************
*FunctionName: ShowNetInfo
*Description: ��ʾ������Ϣ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��5��15:38:44
***************************************************************************************************/
static void ShowNetInfo(void)
{
	memcpy(&(S_NetInfoPageBuffer->systemData), getSystemRunTimeData(), sizeof(SystemData));
	//��ʾ��������Ϣ
	
	S_NetInfoPageBuffer->lineico.ICO_ID = 31;
	S_NetInfoPageBuffer->lineico.X = 189;
	S_NetInfoPageBuffer->lineico.Y = 130;
	
	if(LinkDown == S_NetInfoPageBuffer->systemData.wireNetInfo.lineStatus)
		BasicUI(0x1CB0 ,0x1807 , 0, &(S_NetInfoPageBuffer->lineico) , sizeof(Basic_ICO));
	else
		BasicUI(0x1CB0 ,0x1807 , 1, &(S_NetInfoPageBuffer->lineico) , sizeof(Basic_ICO));
	
	sprintf(S_NetInfoPageBuffer->tempbuffer1, "%03d.%03d.%03d.%03d", S_NetInfoPageBuffer->systemData.wireNetInfo.ip.ip_1, S_NetInfoPageBuffer->systemData.wireNetInfo.ip.ip_2, 
		S_NetInfoPageBuffer->systemData.wireNetInfo.ip.ip_3, S_NetInfoPageBuffer->systemData.wireNetInfo.ip.ip_4);
	DisText(0x1CC0, S_NetInfoPageBuffer->tempbuffer1, 15);
	
	sprintf(S_NetInfoPageBuffer->tempbuffer1, "%02X-%02X-%02X-%02X-%02X-%02X\0", S_NetInfoPageBuffer->systemData.wireNetInfo.LineMAC[0], S_NetInfoPageBuffer->systemData.wireNetInfo.LineMAC[1], 
		S_NetInfoPageBuffer->systemData.wireNetInfo.LineMAC[2], S_NetInfoPageBuffer->systemData.wireNetInfo.LineMAC[3], S_NetInfoPageBuffer->systemData.wireNetInfo.LineMAC[4], 
		S_NetInfoPageBuffer->systemData.wireNetInfo.LineMAC[5]);
	DisText(0x1CD0, S_NetInfoPageBuffer->tempbuffer1, strlen(S_NetInfoPageBuffer->tempbuffer1)+1);	
}
