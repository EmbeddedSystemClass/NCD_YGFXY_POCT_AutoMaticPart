/***************************************************************************************************
*FileName: TestDataDao
*Description: 测试数据dao
*Author: xsx_kair
*Data: 2016年12月8日10:43:26
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"ReTestDataDao.h"

#include	"MyMem.h"

#include	"ff.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
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

MyRes writeReTestDataToFile(ReTestPageBuffer * pageBuffer)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	
	if(myfile && pageBuffer)
	{
		memset(myfile, 0, MyFileStructSize);
		
		myfile->res = f_open(&(myfile->file), "0:/laohua.csv", FA_OPEN_ALWAYS | FA_WRITE);
			
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			f_lseek(&(myfile->file), myfile->size);
			
			if(myfile->size == 0)
			{
				sprintf(pageBuffer->longBuffer, "测试次数,测试时间,测试时长(秒),result-1,result-2,result-3,result-4,result-5,result-6,result-7,result-8,result-9\r");
				myfile->res = f_write(&(myfile->file), pageBuffer->longBuffer, strlen(pageBuffer->longBuffer), &(myfile->bw));
				if(FR_OK != myfile->res)
					goto END;
			}
			
			//保存测试数据
			sprintf(pageBuffer->longBuffer, "%d,20%02d-%02d-%02d %02d:%02d:%02d,%d", pageBuffer->testNum, pageBuffer->paiduiUnitData[0].testData.testDateTime.year, 
				pageBuffer->paiduiUnitData[0].testData.testDateTime.month, pageBuffer->paiduiUnitData[0].testData.testDateTime.day,
				pageBuffer->paiduiUnitData[0].testData.testDateTime.hour, pageBuffer->paiduiUnitData[0].testData.testDateTime.min, 
				pageBuffer->paiduiUnitData[0].testData.testDateTime.sec, timer_Count(&pageBuffer->paiduiUnitData[0].timeDown_timer));
			
			for(pageBuffer->i=0; pageBuffer->i<PaiDuiWeiNum; pageBuffer->i++)
			{
				if(pageBuffer->paiduiUnitData[pageBuffer->i].testData.testResultDesc == ResultIsOK)
					sprintf(pageBuffer->buf, ",%.3f", pageBuffer->paiduiUnitData[pageBuffer->i].testData.testSeries.result);
				else if(pageBuffer->paiduiUnitData[pageBuffer->i].testData.testResultDesc == qrError)
					sprintf(pageBuffer->buf, ",二维码读取失败");
				else if(pageBuffer->paiduiUnitData[pageBuffer->i].testData.testResultDesc == NoSample)
					sprintf(pageBuffer->buf, ",未加样");
				else if(pageBuffer->paiduiUnitData[pageBuffer->i].testData.testResultDesc == PeakError)
					sprintf(pageBuffer->buf, ",峰错误");
				else
					sprintf(pageBuffer->buf, ",test error");
				strcat(pageBuffer->longBuffer, pageBuffer->buf);
			}
			strcat(pageBuffer->longBuffer, "\r\0");
			
			myfile->res = f_write(&(myfile->file), pageBuffer->longBuffer, strlen(pageBuffer->longBuffer), &(myfile->bw));
				
			if(FR_OK != myfile->res)
				goto END;
				
			statues = My_Pass;
			
			END:
				f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

/****************************************end of file************************************************/
