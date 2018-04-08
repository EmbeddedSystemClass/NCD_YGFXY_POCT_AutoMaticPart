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
	PrintfBuf * printfBuf = NULL;

	printfBuf = MyMalloc(sizeof(PrintfBuf));

	if(printfBuf)
	{
		//首先复制数据到自己的缓冲器，以防多任务下，其他任务释放原数据区
		memcpy(&printfBuf->testData, testd2, TestDataStructSize);
		
		sprintf(printfBuf->buf, "\n%s\n", PrintCompanyName);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfBuf->buf, strlen(printfBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfBuf->buf, "%s: %s\n", TesterNameStr, printfBuf->testData.operator.name);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfBuf->buf, strlen(printfBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfBuf->buf, "%s: %s\n", SampleIdStr, printfBuf->testData.sampleid);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfBuf->buf, strlen(printfBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfBuf->buf, "%s: %s\n", ItemNameStr, printfBuf->testData.qrCode.itemConstData.itemName);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfBuf->buf, strlen(printfBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		if(printfBuf->testData.testResultDesc != ResultIsOK)
			sprintf(printfBuf->buf, "%s: ERROR\n", ResultStr);
		else if(printfBuf->testData.testSeries.result <= printfBuf->testData.qrCode.itemConstData.lowstResult)
			sprintf(printfBuf->buf, "%s: <%.*f %-8.8s\n", ResultStr, printfBuf->testData.qrCode.itemConstData.pointNum, printfBuf->testData.qrCode.itemConstData.lowstResult, printfBuf->testData.qrCode.itemConstData.itemMeasure);
		else
			sprintf(printfBuf->buf, "%s: %.*f %-8.8s\n", ResultStr, printfBuf->testData.qrCode.itemConstData.pointNum, printfBuf->testData.testSeries.result, printfBuf->testData.qrCode.itemConstData.itemMeasure);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfBuf->buf, strlen(printfBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfBuf->buf, "%s: %s\n", ReferenceValueStr, printfBuf->testData.qrCode.itemConstData.normalResult);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfBuf->buf, strlen(printfBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfBuf->buf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n", TestTimeStr, printfBuf->testData.testDateTime.year, printfBuf->testData.testDateTime.month, printfBuf->testData.testDateTime.day
			, printfBuf->testData.testDateTime.hour, printfBuf->testData.testDateTime.min, printfBuf->testData.testDateTime.sec);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfBuf->buf, strlen(printfBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		getSystemTime(&printfBuf->dateTime);
		sprintf(printfBuf->buf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n", PrintTimeStr, printfBuf->dateTime.year, printfBuf->dateTime.month, printfBuf->dateTime.day
			, printfBuf->dateTime.hour, printfBuf->dateTime.min, printfBuf->dateTime.sec);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfBuf->buf, strlen(printfBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
		
		sprintf(printfBuf->buf, "%s\n\n\n", StatementStr);
		SendDataToQueue(GetUsart1TXQueue(), NULL, printfBuf->buf, strlen(printfBuf->buf), 1, 50 / portTICK_RATE_MS, 50 / portTICK_RATE_MS, EnableUsart1TXInterrupt);
	}

	MyFree(printfBuf);
}

/****************************************end of file************************************************/
