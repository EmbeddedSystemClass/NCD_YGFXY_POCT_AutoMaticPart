/***************************************************************************************************
*FileName：Universal_Task
*Description：通用任务
*Author：xsx
*Data：2016年5月6日16:56:17
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/

#include	"Universal_Task.h"
#include	"Led_Driver.h"
#include	"Universal_Fun.h"
#include 	"stm324x7i_eval_sdio_sd.h"
#include	"Ads8325_Driver.h"

#include	"CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"

#include	"ff.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/

#define UniversalTask_PRIORITY			2			//看门狗任务优先级
const char * UniversalTaskName = "vUniversalTask";		//看门狗任务名

static SD_TestStruct sd_TestStruct;
static FatfsFileInfo_Def myfile;


char fileName[10];
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/

static void vUniversalTask( void *pvParameters );	//看门狗任务

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName：StartvSysLedTask
*Description：建立系统指示灯任务
*Input：None
*Output：None
*Author：xsx
*Data：2015年8月26日16:58:13
***************************************************************************************************/
char StartvUniversalTask(void)
{
	return xTaskCreate( vUniversalTask, UniversalTaskName, configMINIMAL_STACK_SIZE, NULL, UniversalTask_PRIORITY, NULL );
}

/***************************************************************************************************
*FunctionName：vSysLedTask
*Description：系统指示灯闪烁表面程序正常运行
*Input：None
*Output：None
*Author：xsx
*Data：2015年8月26日16:58:46
***************************************************************************************************/
static void vUniversalTask( void *pvParameters )
{
	static unsigned int count = 0;
	double tempv = 0;
	
//	FRESULT res = f_mount(&S_Fatfs, "0:", 1);					//挂载SD卡
//	res = FR_OK;
	while(1)
	{
		/*系统状态灯*/
		//LedToggle();
		
		tempv = ADS8325();
		
		tempv = 0;

		count++;
		
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

 MyRes sd_test(unsigned int count)
{
	unsigned short i = 0;
	MyRes statues = My_Fail;
	
	for(i=0; i<4096; i++)
		sd_TestStruct.testBuf[i] = i%256;
	sd_TestStruct.crc = CalModbusCRC16Fun(&sd_TestStruct, SD_TestStructCRCSize, NULL);
	
	memset(&myfile, 0, sizeof(FatfsFileInfo_Def));
	
	snprintf(fileName, 10, "0:/%d.n", count);
	myfile.res = f_open(&(myfile.file), fileName, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
	if(FR_OK == myfile.res)
	{
		myfile.res = f_lseek(&(myfile.file), 0);
			
		myfile.res = f_write(&(myfile.file), &sd_TestStruct, SD_TestStructSize, &(myfile.bw));
		if(myfile.res == FR_OK)
		{
			myfile.res = f_lseek(&(myfile.file), 0);
			memset(&sd_TestStruct, 0, SD_TestStructSize);
			myfile.res = f_read(&(myfile.file), &sd_TestStruct, SD_TestStructSize, &(myfile.br));
			if(sd_TestStruct.crc == CalModbusCRC16Fun(&sd_TestStruct, SD_TestStructCRCSize, NULL))
				statues = My_Pass;
		}
		f_close(&(myfile.file));
	}
	

	return statues;
}


