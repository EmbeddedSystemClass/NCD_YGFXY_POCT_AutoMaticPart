/***************************************************************************************************
*FileName:	SystemSet_Dao
*Description:	ϵͳ����dao
*Author: xsx_kair
*Data:	2016��12��16��16:52:22
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/

#include	"SystemSet_Dao.h"

#include	"CRC16.h"
#include	"MyMem.h"

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
*FunctionName: SaveSystemSetData
*Description: ����ϵͳ���ò���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��16��16:57:32
***************************************************************************************************/
MyRes SaveSystemSetData(SystemSetData * systemSetData)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes status = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	
	if(myfile && systemSetData)
	{
		memset(myfile, 0, MyFileStructSize);

		myfile->res = f_open(&(myfile->file), "0:/SysSet.ncd", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
			
			systemSetData->crc = CalModbusCRC16Fun(systemSetData, sizeof(SystemSetData)-2, NULL);
			
			myfile->res = f_write(&(myfile->file), systemSetData, sizeof(SystemSetData), &(myfile->bw));
			
			//���д��ɹ���������ڴ��е��豸��Ϣ
			if((FR_OK == myfile->res)&&(myfile->bw == sizeof(SystemSetData)))
			{
				status = My_Pass;
				
				//����ɹ��󣬸����ڴ��е�����
				upDateSystemSetData(systemSetData);
			}
				
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return status;
}

/***************************************************************************************************
*FunctionName: ReadSystemSetData
*Description: ��sd����ȡϵͳ���ò����������ȡʧ�ܣ�������ڴ��е����ݣ��������ûָ�Ĭ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��16��16:57:06
***************************************************************************************************/
MyRes ReadSystemSetData(SystemSetData * systemSetData)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	
	if(myfile && systemSetData)
	{
		memset(myfile, 0, MyFileStructSize);

		myfile->res = f_open(&(myfile->file), "0:/SysSet.ncd", FA_READ);
		
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
					
			myfile->res = f_read(&(myfile->file), systemSetData, sizeof(SystemSetData), &(myfile->br));
			
			//�����ȡ�ɹ���Ҳ�����ڴ��е��豸��Ϣ����
			if((FR_OK == myfile->res)&&(myfile->br == sizeof(SystemSetData)))
				statues = My_Pass;
			else
				memset(systemSetData, 0, sizeof(SystemSetData));

			f_close(&(myfile->file));
		}
	}
	MyFree(myfile);
	
	return statues;
}

/****************************************end of file************************************************/
