/***************************************************************************************************
*FileName��Universal_Task
*Description��ͨ������
*Author��xsx
*Data��2016��5��6��16:56:17
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
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
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

#define UniversalTask_PRIORITY			2			//���Ź��������ȼ�
const char * UniversalTaskName = "vUniversalTask";		//���Ź�������

static SD_TestStruct sd_TestStruct;
static FatfsFileInfo_Def myfile;


char fileName[10];
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

static void vUniversalTask( void *pvParameters );	//���Ź�����

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��StartvSysLedTask
*Description������ϵͳָʾ������
*Input��None
*Output��None
*Author��xsx
*Data��2015��8��26��16:58:13
***************************************************************************************************/
char StartvUniversalTask(void)
{
	return xTaskCreate( vUniversalTask, UniversalTaskName, configMINIMAL_STACK_SIZE, NULL, UniversalTask_PRIORITY, NULL );
}

/***************************************************************************************************
*FunctionName��vSysLedTask
*Description��ϵͳָʾ����˸���������������
*Input��None
*Output��None
*Author��xsx
*Data��2015��8��26��16:58:46
***************************************************************************************************/
static void vUniversalTask( void *pvParameters )
{
	static unsigned int count = 0;
	double tempv = 0;
	
//	FRESULT res = f_mount(&S_Fatfs, "0:", 1);					//����SD��
//	res = FR_OK;
	while(1)
	{
		/*ϵͳ״̬��*/
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


