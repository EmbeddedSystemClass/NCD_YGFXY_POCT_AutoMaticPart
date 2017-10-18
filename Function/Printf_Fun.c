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
#include 	"Usart1_Driver.h"
#include	"SystemSet_Data.h"
#include	"System_Data.h"
#include	"QueueUnits.h"
#include	"MyMem.h"
#include	"StringDefine.h"

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
	
	printfbuf = MyMalloc(100);
	tempTestData = MyMalloc(sizeof(TestData));

	if(printfbuf && testd2 && tempTestData)
	{
		//首先复制数据到自己的缓冲器，以防多任务下，其他任务释放原数据区
		memcpy(tempTestData, testd2, sizeof(TestData));
		
		sprintf(printfbuf, "%s\n", CompanyNameStr);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n", TesterNameStr, tempTestData->operator.name);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n", SampleIdStr, tempTestData->sampleid);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n", ItemNameStr, tempTestData->qrCode.ItemName);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		if(tempTestData->testResultDesc != ResultIsOK)
			sprintf(printfbuf, "%s: ERROR\n", ResultStr);
		else if(tempTestData->testSeries.result <= tempTestData->qrCode.itemConstData.lowstResult)
			sprintf(printfbuf, "%s: <%.*f %-8.8s\n", ResultStr, tempTestData->qrCode.itemConstData.pointNum, tempTestData->qrCode.itemConstData.lowstResult, tempTestData->qrCode.itemConstData.itemMeasure);
		else
			sprintf(printfbuf, "%s: %.*f %-8.8s\n", ResultStr, tempTestData->qrCode.itemConstData.pointNum, tempTestData->testSeries.result, tempTestData->qrCode.itemConstData.itemMeasure);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n", ReferenceValueStr, tempTestData->qrCode.itemConstData.normalResult);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfbuf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n", TestTimeStr, tempTestData->testDateTime.year, tempTestData->testDateTime.month, tempTestData->testDateTime.day
			, tempTestData->testDateTime.hour, tempTestData->testDateTime.min, tempTestData->testDateTime.sec);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		memcpy(&mytime, &(getSystemRunTimeData()->systemDateTime), sizeof(DateTime));
		sprintf(printfbuf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n", PrintTimeStr, mytime.year, mytime.month, mytime.day
			, mytime.hour, mytime.min, mytime.sec);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfbuf, "%s\n\n\n\n\n", StatementStr);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
	}
	
	MyFree(tempTestData);
	MyFree(printfbuf);
}

/****************************************end of file************************************************/
