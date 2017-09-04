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
	char *buf;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	buf = MyMalloc(1024);
	
	if(myfile && pageBuffer && buf)
	{
		memset(myfile, 0, MyFileStructSize);
		
		myfile->res = f_open(&(myfile->file), "0:/laohua.csv", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			f_lseek(&(myfile->file), myfile->size);
			
			if(myfile->size == 0)
			{
				sprintf(buf, "测试次数,测试时间,测试时长(秒),结果描述,result-1,result-2,result-3,result-4,result-5,result-6,result-7,result-8\r\0");
				myfile->res = f_write(&(myfile->file), buf, strlen(buf), &(myfile->bw));
				if(FR_OK != myfile->res)
					goto END;
			}
			
			//保存测试数据
			sprintf(buf, "%d/%d,%d-%d-%d %d:%d:%d,%d,%s,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\r\0", pageBuffer->testCnt, pageBuffer->testTotalCnt, pageBuffer->dateTime.year
					, pageBuffer->dateTime.month, pageBuffer->dateTime.day, pageBuffer->dateTime.hour, pageBuffer->dateTime.min, pageBuffer->dateTime.sec
					, timer_Count(&pageBuffer->testTimer), pageBuffer->resultDesc, pageBuffer->result[0], pageBuffer->result[1], pageBuffer->result[2]
					, pageBuffer->result[3], pageBuffer->result[4], pageBuffer->result[5], pageBuffer->result[6], pageBuffer->result[7]);
			myfile->res = f_write(&(myfile->file), buf, strlen(buf), &(myfile->bw));
				
			if(FR_OK != myfile->res)
				goto END;
				
			statues = My_Pass;
			
			END:
				f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	MyFree(buf);
	
	return statues;
}

/****************************************end of file************************************************/
