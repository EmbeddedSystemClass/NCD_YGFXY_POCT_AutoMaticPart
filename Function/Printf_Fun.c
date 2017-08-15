/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Printf_Fun.h"
#include 	"Usart3_Driver.h"
#include	"SystemSet_Data.h"
#include	"System_Data.h"
#include	"QueueUnits.h"
#include	"MyMem.h"

#include 	"FreeRTOS.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void PrintfData(TestData * testd2)
{
	char * printfbuf = NULL;
	TestData * tempTestData = NULL;
	DateTime mytime;
	float tempvalue = 0.0;
	
	printfbuf = MyMalloc(100);
	tempTestData = MyMalloc(sizeof(TestData));

	if(printfbuf && testd2 && tempTestData)
	{
		//���ȸ������ݵ��Լ��Ļ��������Է��������£����������ͷ�ԭ������
		memcpy(tempTestData, testd2, sizeof(TestData));
		
		sprintf(printfbuf, "�人Ŧ��������Ƽ��ɷ����޹�˾\n\0");
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "������: %s\n\0", tempTestData->operator.name);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "��Ʒ���: %s\n\0", tempTestData->sampleid);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "������Ŀ: %s\n\0", tempTestData->qrCode.ItemName);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		tempvalue = tempTestData->testSeries.AdjustResult;
		if(tempTestData->testResultDesc != ResultIsOK)
			sprintf(printfbuf, "���Խ��: ERROR\n\0");
		else if(IsShowRealValue() == true)
			sprintf(printfbuf, "���Խ��: %.*f %-8.8s\n\0", tempTestData->qrCode.itemConstData.pointNum, tempTestData->testSeries.AdjustResult, tempTestData->qrCode.itemConstData.itemMeasure);
		else if(tempvalue <= tempTestData->qrCode.itemConstData.lowstResult)
			sprintf(printfbuf, "���Խ��: <%.*f %-8.8s\n\0", tempTestData->qrCode.itemConstData.pointNum, tempTestData->qrCode.itemConstData.lowstResult, tempTestData->qrCode.itemConstData.itemMeasure);
		else if(tempvalue >= tempTestData->qrCode.itemConstData.highestResult)
			sprintf(printfbuf, "���Խ��: >%.*f %-8.8s\n\0", tempTestData->qrCode.itemConstData.pointNum, tempTestData->qrCode.itemConstData.highestResult, tempTestData->qrCode.itemConstData.itemMeasure);
		else
			sprintf(printfbuf, "���Խ��: %.*f %-8.8s\n\0", tempTestData->qrCode.itemConstData.pointNum, tempTestData->testSeries.AdjustResult, tempTestData->qrCode.itemConstData.itemMeasure);
		
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "�ο�ֵ: %s\n\0", tempTestData->qrCode.itemConstData.normalResult);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "����ʱ��: 20%02d-%02d-%02d %02d:%02d:%02d\n\0", tempTestData->testDateTime.year, tempTestData->testDateTime.month, tempTestData->testDateTime.day
			, tempTestData->testDateTime.hour, tempTestData->testDateTime.min, tempTestData->testDateTime.sec);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		memcpy(&mytime, &(getSystemRunTimeData()->systemDateTime), sizeof(DateTime));
		sprintf(printfbuf, "��ӡʱ��: 20%02d-%02d-%02d %02d:%02d:%02d\n\0", mytime.year, mytime.month, mytime.day
			, mytime.hour, mytime.min, mytime.sec);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "����: ��������Ա��걾����\n\n\n\n\n\0");
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
	}
	
	MyFree(tempTestData);
	MyFree(printfbuf);
}

/****************************************end of file************************************************/
