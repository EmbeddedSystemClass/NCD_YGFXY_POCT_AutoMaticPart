/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SelectUserPage.h"
#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"SampleIDPage.h"
#include	"MyTest_Data.h"
#include	"Maintenance_Data.h"
#include	"CRC16.h"
#include	"PlaySong_Task.h"
#include	"DeviceDao.h"
#include	"UserMPage.h"
#include	"Quality_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static UserPageBuffer * S_UserPageBuffer = NULL;

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

static void ShowList(void);
static void SelectUser(unsigned char index);
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
MyRes createSelectUserActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SelectUserActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		if(pram)
		{
			memcpy(&(S_UserPageBuffer->targetOperator), pram->data, pram->datalen);
		}
		
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
	if(S_UserPageBuffer)
	{	
		/*��ȡ�豸��Ϣ*/
		ReadDeviceFromFile(&(S_UserPageBuffer->device));
	
		S_UserPageBuffer->pageindex = 1;
		S_UserPageBuffer->selectindex = 0;
	
		ShowList();
		SelectUser(S_UserPageBuffer->selectindex);
		
		AddNumOfSongToList(9, 0);
	}
	
	SelectPage(84);
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
	if(S_UserPageBuffer)
	{
		/*����*/
		S_UserPageBuffer->lcdinput[0] = pbuf[4];
		S_UserPageBuffer->lcdinput[0] = (S_UserPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_UserPageBuffer->lcdinput[0] == 0x1200)
		{
			DeleteCurrentTest();
			deleteGB_DeviceMaintenance();
			deleteGB_DeviceQuality();
			backToFatherActivity();
		}
		
		/*�Ϸ�Ҳ*/
		else if(S_UserPageBuffer->lcdinput[0] == 0x1203)
		{			
			if(S_UserPageBuffer->pageindex > 1)
			{
				S_UserPageBuffer->pageindex--;
						
				S_UserPageBuffer->selectindex = 0;
						
				ShowList();
				SelectUser(S_UserPageBuffer->selectindex);
			}
		}
		/*�·�ҳ*/
		else if(S_UserPageBuffer->lcdinput[0] == 0x1204)
		{			
			if(S_UserPageBuffer->pageindex < (MaxOperatorSize / MaxPageShowOperatorSize))
			{
				S_UserPageBuffer->tempUser = &S_UserPageBuffer->device.operators[(S_UserPageBuffer->pageindex)*MaxPageShowOperatorSize];
			
				if(S_UserPageBuffer->tempUser->crc == CalModbusCRC16Fun(S_UserPageBuffer->tempUser, sizeof(Operator)-2, NULL))
				{
					S_UserPageBuffer->pageindex++;
						
					S_UserPageBuffer->selectindex = 0;
						
					ShowList();
					SelectUser(S_UserPageBuffer->selectindex);
				}
			}
		}
		/*ȷ��*/
		else if(S_UserPageBuffer->lcdinput[0] == 0x1201)
		{
			if(S_UserPageBuffer->tempUser2 != NULL)
			{
				//������ŶӲ��ԣ��򱣴�����˵��ŶӲ��Թ��ò�����
				SetPaiduiUser(S_UserPageBuffer->tempUser2);
				
				/*�Ե�ǰѡ��Ĳ�������Ϊ���β������ݵĲ�����*/
				memcpy(S_UserPageBuffer->targetOperator, S_UserPageBuffer->tempUser2, sizeof(Operator));
			
				gotoChildActivity(NULL, NULL);
			}
			else
			{
				AddNumOfSongToList(9, 0);
				SendKeyCode(1);
			}
		}
		/*ѡ�������*/
		else if((S_UserPageBuffer->lcdinput[0] >= 0x1205)&&(S_UserPageBuffer->lcdinput[0] <= 0x1209))
		{			
			S_UserPageBuffer->selectindex = S_UserPageBuffer->lcdinput[0] - 0x1205+1;
			SelectUser(S_UserPageBuffer->selectindex);
		}
		//�༭������
		if(S_UserPageBuffer->lcdinput[0] == 0x120a)
		{
			startActivity(createUserManagerActivity, NULL, NULL);
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
	if(S_UserPageBuffer)
	{	
		/*��ȡ���в�����*/
		ReadDeviceFromFile(&(S_UserPageBuffer->device));
	
		S_UserPageBuffer->pageindex = 1;
		S_UserPageBuffer->selectindex = 0;
	
		ShowList();
		SelectUser(S_UserPageBuffer->selectindex);
		
		AddNumOfSongToList(9, 0);
	}
	
	SelectPage(84);
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
	if(NULL == S_UserPageBuffer)
	{
		S_UserPageBuffer = MyMalloc(sizeof(UserPageBuffer));
		
		if(S_UserPageBuffer)	
		{
			memset(S_UserPageBuffer, 0, sizeof(UserPageBuffer));
	
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
	MyFree(S_UserPageBuffer);
	S_UserPageBuffer = NULL;
}



/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��ShowList
*Description����ʾ�б�����
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:44:00
***************************************************************************************************/
static void ShowList(void)
{
	unsigned char i = 0;
	
	i = (S_UserPageBuffer->pageindex-1)*MaxPageShowOperatorSize;
	
	S_UserPageBuffer->tempUser = &(S_UserPageBuffer->device.operators[i]);
	
	/*��ʾ�б�����*/
	for(i=0; i<MaxPageShowOperatorSize; i++)
	{
		DisText(0x1210+i*8, S_UserPageBuffer->tempUser->name, OperatorNameLen);
		
		S_UserPageBuffer->tempUser++;
	}
}

/***************************************************************************************************
*FunctionName��SelectUser
*Description��ѡ��һ�������ˣ����ı���ɫ
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:44:28
***************************************************************************************************/
static void SelectUser(unsigned char index)
{
	unsigned char i = 0;
	
	BasicPic(0x1240, 0, 140, 506, 402, 798, 470, 364, 142+(S_UserPageBuffer->selectindex-1)*72);
	
	if((S_UserPageBuffer->selectindex > 0) && (S_UserPageBuffer->selectindex <= MaxPageShowOperatorSize))
	{
		i = (S_UserPageBuffer->pageindex-1)*MaxPageShowOperatorSize + S_UserPageBuffer->selectindex-1;
		
		S_UserPageBuffer->tempUser2 = &(S_UserPageBuffer->device.operators[i]);
		
		if(S_UserPageBuffer->tempUser2->crc == CalModbusCRC16Fun(S_UserPageBuffer->tempUser2, sizeof(Operator)-2, NULL))
		{
			BasicPic(0x1240, 1, 137, 11, 10, 303, 79, 363, 141+(S_UserPageBuffer->selectindex-1)*72);	
		}
		else
		{
			S_UserPageBuffer->tempUser2 = NULL;
			S_UserPageBuffer->selectindex = 0;
		}
	}
}

