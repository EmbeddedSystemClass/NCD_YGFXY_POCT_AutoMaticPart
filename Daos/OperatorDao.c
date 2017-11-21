/***************************************************************************************************
*FileName: UserDao
*Description: 操作人dao
*Author: xsx_kair
*Data: 2017年2月16日11:31:22
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"OperatorDao.h"
#include	"StringDefine.h"

#include	"CRC16.h"
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

MyRes SaveAllOperatorToFile(Operator * operator)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	
	if(myfile && operator)
	{
		memset(myfile, 0, MyFileStructSize);

		myfile->res = f_open(&(myfile->file), OperatorFileName, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
			
			myfile->res = f_write(&myfile->file, operator, AllOperatorStructSize, &(myfile->bw));
				
			if(FR_OK == myfile->res && myfile->bw == AllOperatorStructSize)
				statues = My_Pass;
				
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

MyRes ReadAllOperatorFromFile(Operator * operator, unsigned short * operatorValidNum)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);

	if(myfile && operator)
	{
		memset(myfile, 0, MyFileStructSize);
		memset(operator, 0, AllOperatorStructSize);

		myfile->res = f_open(&(myfile->file), OperatorFileName, FA_READ);
		
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
			
			if(operatorValidNum)
				*operatorValidNum = 0;
			
			for(unsigned char i=0; i<MaxOperatorSize; i++)
			{
				f_read(&(myfile->file), operator, OneOperatorStructSize, &myfile->br);
				
				if(operator->crc == CalModbusCRC16Fun(operator, OneOperatorStructCrcSize, NULL))
				{
					operator++;
					(*operatorValidNum) += 1;
				}
			}
			
			statues = My_Pass;
			
			f_close(&(myfile->file));
		}
	}	
	MyFree(myfile);
	
	return statues;
}

MyRes deleteOperatorFile(void)
{
	FRESULT res;
	
	res = f_unlink(OperatorFileName);
	
	if((FR_OK == res) || (FR_NO_FILE == res))
		return My_Pass;
	else
		return My_Fail;
}

/****************************************end of file************************************************/
