/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"PaiDuiPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"

#include	"SystemSetPage.h"
#include	"MyMem.h"
#include	"TimeDownNorPage.h"
#include	"PreReadCardPage.h"
#include	"TM1623_Driver.h"
#include	"Motor_Fun.h"
#include	"PlaySong_Task.h"
#include	"CardLimit_Driver.h"
#include	"SampleIDPage.h"
#include	"MyTools.h"
#include	"MyTest_Data.h"
#include	"LunchPage.h"
#include	"System_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static PaiDuiPageBuffer * S_PaiDuiPageBuffer = NULL;
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
MyRes createPaiDuiActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, paiduiActivityName, activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	if(S_PaiDuiPageBuffer)
	{
		//��������Ƚ�ֹ�忨�Զ��½����Թ���
		timer_set(&(S_PaiDuiPageBuffer->timer0), 65535);
		
		//����Ŷ�ģ��ʧ��������ʾ
		timer_set(&(S_PaiDuiPageBuffer->timer1), 1);
	}
	
	SelectPage(93);
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
	if(S_PaiDuiPageBuffer)
	{
		/*����*/
		S_PaiDuiPageBuffer->lcdinput[0] = pbuf[4];
		S_PaiDuiPageBuffer->lcdinput[0] = (S_PaiDuiPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_PaiDuiPageBuffer->lcdinput[0] == 0x1500)
		{
			//�����У���������
			if(NULL != GetCurrentTestItem())
			{
				SendKeyCode(4);
				AddNumOfSongToList(21, 0);
			}
			//�������ԣ���������
			else if(GetMinWaitTime() < 40)
			{
				SendKeyCode(3);
				AddNumOfSongToList(20, 0);
			}
			else if(true == isSomePaiduiInOutTimeStatus())
			{
				SendKeyCode(3);
				AddNumOfSongToList(20, 0);
			}
			else
			{
				//ҳ������ˢ�����ݣ�æ
				while(S_PaiDuiPageBuffer->pageisbusy)
				{
					vTaskDelay(100 / portTICK_RATE_MS);
				}
				
				backToActivity(lunchActivityName);
			}
		}
		//��������
		else if(S_PaiDuiPageBuffer->lcdinput[0] == 0x1501)
		{
			S_PaiDuiPageBuffer->error = CreateANewTest(PaiDuiTestType);
			//�����ɹ�
			if(Error_OK == S_PaiDuiPageBuffer->error)
			{
				startActivity(createSampleActivity, NULL, NULL);
			}
			//�Ŷ�λ������������
			else if(Error_PaiduiFull == S_PaiDuiPageBuffer->error)
			{
				SendKeyCode(2);
				AddNumOfSongToList(19, 0);
			}
			//����ʧ��
			else if(Error_Mem == S_PaiDuiPageBuffer->error)
			{
				SendKeyCode(1);
				AddNumOfSongToList(7, 0);
			}
			//�п���������
			else if(Error_PaiDuiBusy == S_PaiDuiPageBuffer->error)
			{
				SendKeyCode(3);
				AddNumOfSongToList(20, 0);
			}
			//�����н�ֹ���
			else if(Error_PaiduiTesting == S_PaiDuiPageBuffer->error)
			{
				SendKeyCode(4);
				AddNumOfSongToList(21, 0);
			}
			//�Ŷ�ģ��ʧ��
			else if(Error_PaiduiDisconnect == S_PaiDuiPageBuffer->error)
			{
				SendKeyCode(5);
				AddNumOfSongToList(58, 0);
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
	unsigned char index = 0;
	
	if(S_PaiDuiPageBuffer)
	{
		S_PaiDuiPageBuffer->count2++;
		
		//����æ
		S_PaiDuiPageBuffer->pageisbusy = true;

		//���һ���Ŷ�ģ���Ƿ�����
		if(Connect_Error == getSystemRunTimeData()->paiduiModuleStatus)
		{
			if(TimeOut == timer_expired(&(S_PaiDuiPageBuffer->timer1)))
			{
				timer_set(&(S_PaiDuiPageBuffer->timer1), 5);
				SendKeyCode(6);
				AddNumOfSongToList(57, 2);
			}
		}
			
		
		//500msˢ��һ�ν���
		if(S_PaiDuiPageBuffer->count2 % 5 == 0)
		{
			//�����ǰ���ܴ��ڽ�ֹ״̬���ҵ��λ�ô�������г̣��ҿ����޿���������Ŀ����� �����ò忨�Զ���������
			if((S_PaiDuiPageBuffer->timer0.interval == 65535) && (MaxLocation == getSystemRunTimeData()->motorData.location) && (!CardPinIn))
			{
				timer_set(&(S_PaiDuiPageBuffer->timer0), 1);
			}

			//�����ǰ���У���ɨ��ʱ�䵽�������Ƿ�忨��
			if(TimeOut == timer_expired(&(S_PaiDuiPageBuffer->timer0)))
			{
				//�����ǰ����,���Ѿ��忨
				if((CardPinIn) && (NULL == GetCurrentTestItem()))
				{
					S_PaiDuiPageBuffer->error = CreateANewTest(PaiDuiTestType);
					//�����ɹ�
					if(Error_OK == S_PaiDuiPageBuffer->error)
					{
						vTaskDelay(1000 / portTICK_RATE_MS);
						//�����ɹ�����ʹ���Զ�룬��ֹ�û��ο�
						MotorMoveTo(1000, 1);			
						startActivity(createSampleActivity, NULL, NULL);		
						return;
					}
					//�Ŷ�λ������������
					else if(Error_PaiduiFull == S_PaiDuiPageBuffer->error)
					{
						SendKeyCode(2);
						AddNumOfSongToList(19, 2);
						timer_set(&(S_PaiDuiPageBuffer->timer0), 65535);
					}
					//����ʧ��
					else if(Error_Mem == S_PaiDuiPageBuffer->error)
					{
						SendKeyCode(1);
						AddNumOfSongToList(7, 0);
						timer_set(&(S_PaiDuiPageBuffer->timer0), 65535);
					}
					//�п���������
					else if(Error_PaiDuiBusy == S_PaiDuiPageBuffer->error)
					{
						SendKeyCode(3);
						AddNumOfSongToList(20, 0);
						timer_set(&(S_PaiDuiPageBuffer->timer0), 65535);
					}
					//�����н�ֹ���
					else if(Error_PaiduiTesting == S_PaiDuiPageBuffer->error)
					{
						SendKeyCode(4);
						AddNumOfSongToList(21, 0);
						timer_set(&(S_PaiDuiPageBuffer->timer0), 65535);
					}
					//�Ŷ�ģ��ʧ��
					else if(Error_PaiduiDisconnect == S_PaiDuiPageBuffer->error)
					{
						SendKeyCode(5);
						AddNumOfSongToList(58, 0);
					}
				}
			}
			//���µ���ʱ����
			for(index=0; index<PaiDuiWeiNum; index++)
			{
				S_PaiDuiPageBuffer->tempd2 = GetTestItemByIndex(index);
				
				if(S_PaiDuiPageBuffer->tempd2)
				{
					DspNum(0x1506+index, S_PaiDuiPageBuffer->tempd2->statues, 2);
					//��ʱ
					if(isInTimeOutStatus(S_PaiDuiPageBuffer->tempd2))
					{
						S_PaiDuiPageBuffer->tempvalue1 = timer_Count(&(S_PaiDuiPageBuffer->tempd2->timer2));
						if(S_PaiDuiPageBuffer->tempvalue1 > 60)
							sprintf(S_PaiDuiPageBuffer->buf, "%02dM", S_PaiDuiPageBuffer->tempvalue1/60);
						else
							sprintf(S_PaiDuiPageBuffer->buf, "%02dS", S_PaiDuiPageBuffer->tempvalue1);
		
						WriteVarIcoNum(0x1510+index*16, 50);
					}
					else
					{
						S_PaiDuiPageBuffer->tempvalue1 = timer_surplus(&(S_PaiDuiPageBuffer->tempd2->timer));
						if(S_PaiDuiPageBuffer->tempvalue1 > 60)
							sprintf(S_PaiDuiPageBuffer->buf, "%02dM", S_PaiDuiPageBuffer->tempvalue1/60);
						else
							sprintf(S_PaiDuiPageBuffer->buf, "%02dS", S_PaiDuiPageBuffer->tempvalue1);							
						
						S_PaiDuiPageBuffer->tempvalue = S_PaiDuiPageBuffer->tempd2->testData.qrCode.CardWaitTime*60 - S_PaiDuiPageBuffer->tempvalue1;
						S_PaiDuiPageBuffer->tempvalue /= S_PaiDuiPageBuffer->tempd2->testData.qrCode.CardWaitTime*60;
						S_PaiDuiPageBuffer->tempvalue *= 50;

						WriteVarIcoNum(0x1510+index*16, (unsigned short)(S_PaiDuiPageBuffer->tempvalue));
					}
					
					DisText(0x1610+index*0x08, S_PaiDuiPageBuffer->buf, 10);
					
					if((S_PaiDuiPageBuffer->tempd2->statues == status_timedown) || (S_PaiDuiPageBuffer->tempd2->statues == status_timeup)){
						BasicPic(0x1590+index*0x10, 1, 138, 10+85*S_PaiDuiPageBuffer->tempd2->testData.qrCode.itemConstData.icoIndex, 10, 10+85*S_PaiDuiPageBuffer->tempd2->testData.qrCode.itemConstData.icoIndex+75, 10+285, 91+index*110, 190);
					}
					else
					{
						//��⿨ͼ����˸
						if((S_PaiDuiPageBuffer->count % 2) == 0)
							BasicPic(0x1590+index*0x10, 1, 138, 10+85*S_PaiDuiPageBuffer->tempd2->testData.qrCode.itemConstData.icoIndex, 10, 10+85*S_PaiDuiPageBuffer->tempd2->testData.qrCode.itemConstData.icoIndex+75, 10+285, 91+index*110, 190);
						else
							BasicPic(0x1590+index*0x10, 0, 138, 10+85*S_PaiDuiPageBuffer->tempd2->testData.qrCode.itemConstData.icoIndex, 10, 10+85*S_PaiDuiPageBuffer->tempd2->testData.qrCode.itemConstData.icoIndex+75, 10+285, 91+index*110, 190);
					}
				}
				else
				{
					//�������ʱʱ��
					ClearText(0x1610+index*0x08);
					ClearText(0x1650+index*0x08);
					
					//��ʾ������
					S_PaiDuiPageBuffer->myico.ICO_ID = 37;
					S_PaiDuiPageBuffer->myico.X = 69+index*119;
					S_PaiDuiPageBuffer->myico.Y = 135;
					BasicUI(0x1590+index*0x10 ,0x1907 , 0, &(S_PaiDuiPageBuffer->myico) , sizeof(Basic_ICO));
					
					//ʱ���������ʾ0
					WriteVarIcoNum(0x1510+index*16, 0);
				}
			}
			
			S_PaiDuiPageBuffer->count++;
			if(S_PaiDuiPageBuffer->count > 65535)
				S_PaiDuiPageBuffer->count = 0;
		}
		
		//�������
		S_PaiDuiPageBuffer->pageisbusy = false;
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
	//�����ǰҳ��ĸ澯������
	SendKeyCode(16);
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
	if(S_PaiDuiPageBuffer)
	{
		//��������Ƚ�ֹ�忨�Զ��½����Թ��ܣ�ͨ�����ó�����ʱ��
		timer_set(&(S_PaiDuiPageBuffer->timer0), 65535);
	}
	
	SelectPage(93);
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
	//�����ǰҳ��ĸ澯������
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
	if(NULL == S_PaiDuiPageBuffer)
	{
		S_PaiDuiPageBuffer = MyMalloc(sizeof(PaiDuiPageBuffer));
		
		if(S_PaiDuiPageBuffer)
		{
			memset(S_PaiDuiPageBuffer, 0, sizeof(PaiDuiPageBuffer));
	
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
	MyFree(S_PaiDuiPageBuffer);
	S_PaiDuiPageBuffer = NULL;
}


