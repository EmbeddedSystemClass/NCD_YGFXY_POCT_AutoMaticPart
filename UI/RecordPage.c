/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"RecordPage.h"
#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"
#include	"SystemSetPage.h"
#include	"ShowResultPage.h"
#include	"CRC16.h"
#include	"Printf_Fun.h"
#include	"System_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static RecordPageBuffer * S_RecordPageBuffer = NULL;
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

static void ShowRecord(void);
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
MyRes createRecordActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "RecordActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	S_RecordPageBuffer->pageRequest.pageIndex = 0;
	S_RecordPageBuffer->pageRequest.pageSize = TestDataRecordPageShowNum;
	S_RecordPageBuffer->pageRequest.orderType = ASC;
	
	for(S_RecordPageBuffer->i=0; S_RecordPageBuffer->i <TestDataRecordPageShowNum ; S_RecordPageBuffer->i++)
		S_RecordPageBuffer->page.content[S_RecordPageBuffer->i] = &S_RecordPageBuffer->testData[S_RecordPageBuffer->i];
	
	ShowRecord();
	
	SelectPage(114);
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
	if(S_RecordPageBuffer)
	{
		/*����*/
		S_RecordPageBuffer->lcdinput[0] = pbuf[4];
		S_RecordPageBuffer->lcdinput[0] = (S_RecordPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_RecordPageBuffer->lcdinput[0] == 0x2000)
		{
			backToFatherActivity();
		}
		//�鿴
		else if(S_RecordPageBuffer->lcdinput[0] == 0x2001)
		{
			//if((S_RecordPageBuffer->selectindex > 0) && (S_RecordPageBuffer->selectindex <= S_RecordPageBuffer->page.ElementsSize))
			//	startActivity(createShowResultActivity, createIntent(&S_RecordPageBuffer->page.testData[S_RecordPageBuffer->page.ElementsSize - S_RecordPageBuffer->selectindex], sizeof(TestData)));
		}
		/*��һҳ*/
		else if(S_RecordPageBuffer->lcdinput[0] == 0x2002)
		{
			if(S_RecordPageBuffer->page.totalPageSize > 0)
			{
				if(S_RecordPageBuffer->pageRequest.pageIndex >= 1)
					S_RecordPageBuffer->pageRequest.pageIndex -= 1;
				else
					S_RecordPageBuffer->pageRequest.pageIndex = S_RecordPageBuffer->page.totalPageSize - 1;
				
				ShowRecord();
			}
		}
		/*��һҳ*/
		else if(S_RecordPageBuffer->lcdinput[0] == 0x2003)
		{
			if(S_RecordPageBuffer->page.totalPageSize > 0)
			{
				if((S_RecordPageBuffer->pageRequest.pageIndex + 1) >= S_RecordPageBuffer->page.totalPageSize)
					S_RecordPageBuffer->pageRequest.pageIndex = 0;
				else
					S_RecordPageBuffer->pageRequest.pageIndex += 1;
				
				ShowRecord();
			}
		}
		//ѡ������
		else if((S_RecordPageBuffer->lcdinput[0] >= 0x2004)&&(S_RecordPageBuffer->lcdinput[0] <= 0x200b))
		{
			S_RecordPageBuffer->tempvalue1 = S_RecordPageBuffer->lcdinput[0] - 0x2004;
			
			if(S_RecordPageBuffer->tempvalue1 < S_RecordPageBuffer->page.readItemSize)
			{
				BasicPic(0x2020, 1, 137, 11, 142, 940, 179, 38, 149+(S_RecordPageBuffer->tempvalue1)*40);
				startActivity(createShowResultActivity, createIntent(&(S_RecordPageBuffer->testData[S_RecordPageBuffer->tempvalue1]), TestDataStructSize), NULL);
			}
		}
		//��ҳ
		else if(S_RecordPageBuffer->lcdinput[0] == 0x2010)
		{
			S_RecordPageBuffer->tempvalue1 = strtol((char *)(&pbuf[7]), NULL, 10);
			if( (S_RecordPageBuffer->tempvalue1 > 0) && (S_RecordPageBuffer->tempvalue1 <= S_RecordPageBuffer->page.totalPageSize))
			{
				S_RecordPageBuffer->pageRequest.pageIndex = S_RecordPageBuffer->tempvalue1;

				ShowRecord();
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
	if(S_RecordPageBuffer)
	{

	}
	
	SelectPage(114);
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
	if(NULL == S_RecordPageBuffer)
	{
		S_RecordPageBuffer = MyMalloc(sizeof(RecordPageBuffer));
		
		if(S_RecordPageBuffer)
		{
			memset(S_RecordPageBuffer, 0, sizeof(RecordPageBuffer));
	
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
	MyFree(S_RecordPageBuffer);
	S_RecordPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

static void ShowRecord(void)
{
	//��ȡ����
	memset(S_RecordPageBuffer->testData, 0, TestDataRecordPageShowNum * TestDataStructSize);
	readTestDataFromFileByPageRequest(&(S_RecordPageBuffer->pageRequest), &(S_RecordPageBuffer->deviceRecordHeader), &S_RecordPageBuffer->page);
		
	BasicPic(0x2020, 0, 100, 11, 142, 940, 179, 39, 140);
	
	vTaskDelay(10 / portTICK_RATE_MS);

	for(S_RecordPageBuffer->i=0; S_RecordPageBuffer->i<TestDataRecordPageShowNum; S_RecordPageBuffer->i++)
	{
		S_RecordPageBuffer->tempdata = &S_RecordPageBuffer->testData[S_RecordPageBuffer->i];
		S_RecordPageBuffer->tempvalue1 = S_RecordPageBuffer->i*0x40;
		
		if(S_RecordPageBuffer->tempdata->crc == CalModbusCRC16Fun(S_RecordPageBuffer->tempdata, TestDataStructCrcSize, NULL))
		{
			//��ʾ����
			if(S_RecordPageBuffer->pageRequest.orderType == ASC)
				snprintf(S_RecordPageBuffer->buf, 10, "%d", S_RecordPageBuffer->page.totalItemSize - S_RecordPageBuffer->pageRequest.pageIndex*TestDataRecordPageShowNum-S_RecordPageBuffer->i);
			else
				snprintf(S_RecordPageBuffer->buf, 10, "%d", S_RecordPageBuffer->pageRequest.pageIndex*TestDataRecordPageShowNum+S_RecordPageBuffer->i+1);
			DisText(0x2030+S_RecordPageBuffer->tempvalue1, S_RecordPageBuffer->buf, strlen(S_RecordPageBuffer->buf)+1);
				
			//��ʾ��Ŀ
			snprintf(S_RecordPageBuffer->buf, ItemNameLen, "%.11s", S_RecordPageBuffer->tempdata->qrCode.itemConstData.itemName);
			DisText(0x2036+S_RecordPageBuffer->tempvalue1, S_RecordPageBuffer->buf, strlen(S_RecordPageBuffer->buf)+1);
				
			//��ʾ��Ʒ���
			snprintf(S_RecordPageBuffer->buf, 16, "%.15s", S_RecordPageBuffer->tempdata->sampleid);
			DisText(0x2040+S_RecordPageBuffer->tempvalue1, S_RecordPageBuffer->buf, strlen(S_RecordPageBuffer->buf)+1);
				
			//��ʾ���
			if(S_RecordPageBuffer->tempdata->testResultDesc != ResultIsOK)
				snprintf(S_RecordPageBuffer->buf, 10, "Error");
			else if(IsShowRealValue())
				snprintf(S_RecordPageBuffer->buf, 20, "%.*f %s", S_RecordPageBuffer->tempdata->qrCode.itemConstData.pointNum, 
					S_RecordPageBuffer->tempdata->testSeries.result, S_RecordPageBuffer->tempdata->qrCode.itemConstData.itemMeasure);
			else if(S_RecordPageBuffer->tempdata->testSeries.result <= S_RecordPageBuffer->tempdata->qrCode.itemConstData.lowstResult)
				snprintf(S_RecordPageBuffer->buf, 20, "<%.*f %s", S_RecordPageBuffer->tempdata->qrCode.itemConstData.pointNum, 
					S_RecordPageBuffer->tempdata->qrCode.itemConstData.lowstResult, S_RecordPageBuffer->tempdata->qrCode.itemConstData.itemMeasure);
            else if(S_RecordPageBuffer->tempdata->testSeries.result >= S_RecordPageBuffer->tempdata->qrCode.itemConstData.highestResult)
				snprintf(S_RecordPageBuffer->buf, 20, ">%.*f %s", S_RecordPageBuffer->tempdata->qrCode.itemConstData.pointNum, 
					S_RecordPageBuffer->tempdata->qrCode.itemConstData.highestResult, S_RecordPageBuffer->tempdata->qrCode.itemConstData.itemMeasure);
			else
				snprintf(S_RecordPageBuffer->buf, 20, "%.*f %s", S_RecordPageBuffer->tempdata->qrCode.itemConstData.pointNum, 
					S_RecordPageBuffer->tempdata->testSeries.result, S_RecordPageBuffer->tempdata->qrCode.itemConstData.itemMeasure);
			DisText(0x204C+S_RecordPageBuffer->tempvalue1, S_RecordPageBuffer->buf, strlen(S_RecordPageBuffer->buf)+1);
				
			//��ʾʱ��
			snprintf(S_RecordPageBuffer->buf, 15, "%02d-%02d-%02d %02d:%02d", S_RecordPageBuffer->tempdata->testDateTime.year, 
				S_RecordPageBuffer->tempdata->testDateTime.month, S_RecordPageBuffer->tempdata->testDateTime.day,
				S_RecordPageBuffer->tempdata->testDateTime.hour, S_RecordPageBuffer->tempdata->testDateTime.min);
			DisText(0x2058+S_RecordPageBuffer->tempvalue1, S_RecordPageBuffer->buf, strlen(S_RecordPageBuffer->buf)+1);
				
			//��ʾ������
			snprintf(S_RecordPageBuffer->buf, 15, "%s", S_RecordPageBuffer->tempdata->operator.name);
			DisText(0x2065+S_RecordPageBuffer->tempvalue1, S_RecordPageBuffer->buf, strlen(S_RecordPageBuffer->buf)+1);

			S_RecordPageBuffer->tempdata--;
		}
		else
		{
			ClearText(0x2030+S_RecordPageBuffer->tempvalue1);
			ClearText(0x2036+S_RecordPageBuffer->tempvalue1);
			ClearText(0x2040+S_RecordPageBuffer->tempvalue1);
			ClearText(0x204c+S_RecordPageBuffer->tempvalue1);
			ClearText(0x2058+S_RecordPageBuffer->tempvalue1);
			ClearText(0x2065+S_RecordPageBuffer->tempvalue1);
		}
		
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}

