/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"UserMPage.h"
#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"SystemSetPage.h"
#include	"DeviceDao.h"
#include	"MyTools.h"
#include	"SleepPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static UserMPageBuffer * page = NULL;

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

static void showCurrentPageList(void);
static void clearPageList(void);
static void showCurrentOperatorInfo(void);
static void deleteCurrentOperator(void);
static MyRes addOrModifyOperator(void);
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
MyRes createUserManagerActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "UserManagerActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	page->myDeviceLock = getMyDeviceLock();
	page->isLocked = false;
	timer_set(&(page->timer), 1);
	
	clearPageList();
	
	SelectPage(106);
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
	/*����*/
	page->lcdinput[0] = pbuf[4];
	page->lcdinput[0] = (page->lcdinput[0]<<8) + pbuf[5];
		
	/*����*/
	if(page->lcdinput[0] == 0x1d00)
	{
		backToFatherActivity();
	}
		
	/*�Ϸ�Ҳ*/
	else if(page->lcdinput[0] == 0x1d03)
	{
		if(page->isLocked && page->pageindex > 0)
		{
			page->pageindex--;

			showCurrentPageList();
		}
	}
	/*�·�ҳ*/
	else if(page->lcdinput[0] == 0x1d04)
	{
		if(page->isLocked && ((page->pageindex+1) < MaxOperatorPageSize))
		{
			page->tempOperator = &page->device.operators[(page->pageindex+1)*MaxPageShowOperatorSize];
			
			if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
			{
				page->pageindex++; 

				showCurrentPageList();
			}
		}
	}
	/*ѡ�������*/
	else if((page->lcdinput[0] >= 0x1d07)&&(page->lcdinput[0] <= 0x1d0B))
	{
		page->tempOperator = &page->device.operators[(page->pageindex)*MaxPageShowOperatorSize + page->lcdinput[0] - 0x1d07];
		
		if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
		{
			page->tempV1 = page->lcdinput[0] - 0x1d07+1;
			BasicPic(0x1d40, 1, 137, 11, 10, 303, 79, 157, 135+(page->tempV1-1)*72);
			
			page->currentOperator = page->tempOperator;
			
			showCurrentOperatorInfo();
		}	
	}
	/*ɾ��*/
	else if(page->lcdinput[0] == 0x1d01)
	{
		if(page->isLocked)
			deleteCurrentOperator();
	}
	//�޸Ļ������
	else if(page->lcdinput[0] == 0x1d02)
	{
		if(strlen(page->newOperator.name) > 0)
		{
			if(My_Pass == addOrModifyOperator())
				SendKeyCode(1);
			else
				SendKeyCode(2);
		}
		else
			SendKeyCode(2);
	}
	//����
	else if(page->lcdinput[0] == 0x1d50)
	{
		getLcdInputData(page->newOperator.name, &pbuf[7]);
	}
	//����
	else if(page->lcdinput[0] == 0x1d60)
	{
		getLcdInputData(page->newOperator.age, &pbuf[7]);
	}
	//�Ա�
	else if(page->lcdinput[0] == 0x1d70)
	{
		getLcdInputData(page->newOperator.sex, &pbuf[7]);
	}
	//��ϵ��ʽ
	else if(page->lcdinput[0] == 0x1d80)
	{
		getLcdInputData(page->newOperator.phone, &pbuf[7]);
	}
	//ְλ
	else if(page->lcdinput[0] == 0x1d90)
	{
		getLcdInputData(page->newOperator.job, &pbuf[7]);
	}
	//��ע
	else if(page->lcdinput[0] == 0x1da0)
	{
		getLcdInputData(page->newOperator.department, &pbuf[7]);
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
	if(TimeOut == timer_expired(&(page->timer)))
	{
		//�ȸ��豸��Ϣ�ļ���������ֹ�����߳��޸�����
		if((page->isLocked == false) && (My_Pass == LockObject(page->myDeviceLock, &page, 1)))
		{
			/*��ȡ���в�����*/
			ReadDeviceFromFile(&(page->device));
			
			page->pageindex = 0;

			showCurrentPageList();
			
			page->isLocked = true;
		}
		
		timer_restart(&(page->timer));
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
	SelectPage(106);
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
	UnLockObject(page->myDeviceLock, &page);
	
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
	if(NULL == page)
	{
		page = MyMalloc(sizeof(UserMPageBuffer));
		
		if(page)
		{
			memset(page, 0, sizeof(UserMPageBuffer));
	
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
	MyFree(page);
	page = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName:  showCurrentPageList
*Description:  ��ʾ��ǰҳ�������б�
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��6�� 14:16:50
***************************************************************************************************/
static void showCurrentPageList(void)
{
	page->tempV1 = page->pageindex*MaxPageShowOperatorSize;
	
	page->tempOperator = &(page->device.operators[page->tempV1]);
	
	/*��ʾ�б�����*/
	for(page->tempV1=0; page->tempV1<MaxOperatorSize; page->tempV1++)
	{
		if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
			snprintf(page->buf, OperatorNameLen, "%s", page->tempOperator->name);
		else
			memset(page->buf, 0, 10);
		
		DisText(0x1d10+8*page->tempV1, page->buf, strlen(page->buf)+1);
		
		page->tempOperator++;
	}
	
	page->currentOperator = NULL;
	
	showCurrentOperatorInfo();
}

/***************************************************************************************************
*FunctionName:  clearPageList
*Description:  ����б�����
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��7�� 08:43:04
***************************************************************************************************/
static void clearPageList(void)
{
	/*��ʾ�б�����*/
	for(page->tempV1=0; page->tempV1<MaxPageShowOperatorSize; page->tempV1++)
	{
		ClearText(0x1d10+8*page->tempV1);
	}
	
	page->currentOperator = NULL;
	
	showCurrentOperatorInfo();
}

/***************************************************************************************************
*FunctionName:  showCurrentOperatorInfo
*Description:  ��ʾ��ǰ��������Ϣ
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��6�� 14:19:13
***************************************************************************************************/
static void showCurrentOperatorInfo(void)
{
	if(page->currentOperator)
	{
		snprintf(page->buf, OperatorNameLen, "%s", page->currentOperator->name);
		DisText(0x1d50, page->buf, strlen(page->buf)+1);
			
		snprintf(page->buf, 10, "%s", page->currentOperator->age);
		DisText(0x1d60, page->buf, strlen(page->buf)+1);
			
		snprintf(page->buf, 10, "%s", page->currentOperator->sex);
		DisText(0x1d70, page->buf, strlen(page->buf)+1);
			
		snprintf(page->buf, 20, "%s", page->currentOperator->phone);
		DisText(0x1d80, page->buf, strlen(page->buf)+1);
			
		snprintf(page->buf, 30, "%s", page->currentOperator->job);
		DisText(0x1d90, page->buf, strlen(page->buf)+1);
			
		snprintf(page->buf, 20, "%s", page->currentOperator->department);
		DisText(0x1da0, page->buf, strlen(page->buf)+1);
		
		memcpy(&page->newOperator, page->currentOperator, OneOperatorStructSize);
	}
	else
	{
		ClearText(0x1d50);
			
		ClearText(0x1d60);
				
		ClearText(0x1d70);
				
		ClearText(0x1d80);
				
		ClearText(0x1d90);
				
		ClearText(0x1da0);
		
		BasicPic(0x1d40, 0, 137, 11, 10, 303, 79, 157, 135);
		
		memset(&page->newOperator, 0, OneOperatorStructSize);
	}
}

/***************************************************************************************************
*FunctionName:  deleteCurrentOperator
*Description:  ɾ����ǰ������
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��6�� 15:25:44
***************************************************************************************************/
static void deleteCurrentOperator(void)
{
	if(page->currentOperator && page->currentOperator != &(page->newOperator))
	{
		//ɾ����ǰ������
		page->currentOperator->crc = 0;
		
		//�������в�����
		page->tempOperator = page->currentOperator;
		page->tempOperator++;
		memset(page->operatorCopy, 0, MaxOperatorSize*OneOperatorStructSize);
		page->tempV1 = OneOperatorStructSize * (&page->device.operators[MaxOperatorSize-1] - page->currentOperator);
		memcpy(page->operatorCopy, page->tempOperator, page->tempV1);
		
		memcpy(page->currentOperator, page->operatorCopy, page->tempV1);
		
		page->device.crc = CalModbusCRC16Fun(&(page->device), DeviceStructCrcSize, NULL);
		
		if(My_Pass != SaveDeviceToFile(&(page->device)))
			ReadDeviceFromFile(&page->device);
		
		page->pageindex = 0;

		showCurrentPageList();
	}
}

/***************************************************************************************************
*FunctionName:  addOrModifyOperator
*Description:  ��ӻ����޸Ĳ�����
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��7�� 16:07:17
***************************************************************************************************/
static MyRes addOrModifyOperator(void)
{
	page->newOperator.crc = CalModbusCRC16Fun(&page->newOperator, OneOperatorStructCrcSize, NULL);
	
	//�����Ƿ��������һ������
	page->tempV2 = MaxOperatorSize;
	page->tempOperator = page->device.operators;
	for(page->tempV1 = 0; page->tempV1 < MaxOperatorSize; page->tempV1++)
	{
		if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
		{
			if(CheckStrIsSame(page->newOperator.name, page->tempOperator->name, OperatorNameLen))
			{
				memcpy(page->tempOperator, &page->newOperator, OneOperatorStructSize);
				
				break;
			}
		}
		else if(page->tempV2 == MaxOperatorSize)
			page->tempV2 = page->tempV1;
		
		page->tempOperator++;
	}
	
	//��������ڣ�˵��Ҫ�½�
	if(page->tempV1 >= MaxOperatorSize)
	{
		//����������������޷����
		if(page->tempV2 == MaxOperatorSize)
			return My_Fail;
		else
			memcpy(&page->device.operators[page->tempV2], &page->newOperator, OneOperatorStructSize);
	}
	
	page->device.crc = CalModbusCRC16Fun(&(page->device), DeviceStructCrcSize, NULL);
		
	if(My_Pass == SaveDeviceToFile(&(page->device)))
	{
		page->pageindex = 0;

		showCurrentPageList();
	
		return My_Pass;
	}
	else
	{
		return My_Fail;
	}
		
	
}

