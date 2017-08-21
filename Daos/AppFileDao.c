/***************************************************************************************************
*FileName: AppFileDao
*Description: �����������bin�ļ�
*Author: xsx_kair
*Data: 2017��2��10��16:22:59
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"AppFileDao.h"

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

/***************************************************************************************************
*FunctionName: WriteTestData
*Description: д�������ݵ��ļ�
*Input: testdata -- �������ݵ�ַ
*		writeIndex -- д������
*Output: None
*Return: 	My_Pass -- ����ɹ�
*			My_Fail -- ����ʧ��
*Author: xsx
*Date: 2016��12��8��10:55:53
***************************************************************************************************/
MyRes WriteAppFile(char * file, unsigned short len, bool isNew)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	
	if(myfile && file)
	{
		memset(myfile, 0, MyFileStructSize);
		
		if(isNew == true)
			myfile->res = f_open(&(myfile->file), "0:/UPDATE.BIN", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
		else
			myfile->res = f_open(&(myfile->file), "0:/UPDATE.BIN", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{	
			myfile->size = f_size(&(myfile->file));
			
			f_lseek(&(myfile->file), myfile->size);
			
			myfile->res = f_write(&(myfile->file), file, len, &(myfile->bw));
			if(myfile->res == FR_OK)
				statues = My_Pass;
			
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

/***************************************************************************************************
*FunctionName: ReadAppFile
*Description: ��ȡӦ�ó�������
*Input: startAddr -- ƫ�Ƶ�ַ�� dataBuf -- ����, size -- ��ȡ����, *br -- ʵ�ʶ�ȡ����
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��16��14:55:26
***************************************************************************************************/
MyRes ReadAppFile(unsigned int startAddr, unsigned char * dataBuf, unsigned short size, unsigned short *br,
	unsigned int *fileSize)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	
	*br = 0;
	
	if(myfile && dataBuf)
	{
		memset(myfile, 0, MyFileStructSize);

		myfile->res = f_open(&(myfile->file), "0:/UPDATE.BIN", FA_OPEN_EXISTING | FA_READ);

		if(FR_OK == myfile->res)
		{
			if(fileSize != NULL)
				*fileSize = f_size(&(myfile->file));
			
			f_lseek(&(myfile->file), startAddr);
			
			myfile->res = f_read(&(myfile->file), dataBuf, size, &(myfile->br));
			if(myfile->res == FR_OK)
			{
				statues = My_Pass;
				*br = myfile->br;
			}
			
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

/***************************************************************************************************
*FunctionName: checkNewAppFileIsExist
*Description: ����Ƿ�����³���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��16��15:12:19
***************************************************************************************************/
MyRes checkNewAppFileIsExist(void)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	
	if(myfile)
	{
		memset(myfile, 0, MyFileStructSize);
		
		myfile->res = f_open(&(myfile->file), "0:/UPDATE.BIN", FA_OPEN_EXISTING | FA_READ);

		if(FR_OK == myfile->res)
		{	
			statues = My_Pass;
			
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

/***************************************************************************************************
*FunctionName: deleteAppFileIfExist
*Description: ������ڣ���ɾ���³����ļ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��16��15:13:20
***************************************************************************************************/
MyRes deleteAppFileIfExist(void)
{
	FRESULT res;
	
	res = f_unlink("0:/UPDATE.BIN");
	
	if((FR_OK == res) || (FR_NO_FILE == res))
		return My_Pass;
	else
		return My_Fail;
}


/****************************************end of file************************************************/
