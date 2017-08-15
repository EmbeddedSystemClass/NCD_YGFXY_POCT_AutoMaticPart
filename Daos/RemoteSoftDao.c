/***************************************************************************************************
*FileName: RemoteSoftDao
*Description: �̼���Ϣ
*Author: xsx_kair
*Data: 2017��2��16��15:23:25
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"RemoteSoftDao.h"

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
*FunctionName: WriteRemoteSoftInfo
*Description: ������¹̼�����Ϣ
*Input: remoteSoftInfo -- �̼���Ϣ
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��16��15:25:32
***************************************************************************************************/
MyRes WriteRemoteSoftInfo(RemoteSoftInfo * remoteSoftInfo)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	
	if(myfile && remoteSoftInfo)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));

		myfile->res = f_open(&(myfile->file), "0:/SINFO.NCD", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);

		if(FR_OK == myfile->res)
		{	
			f_lseek(&(myfile->file), 0);
			
			myfile->res = f_write(&(myfile->file), remoteSoftInfo, sizeof(RemoteSoftInfo), &(myfile->bw));
			if(myfile->res == FR_OK)
				statues = My_Pass;
			
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

/***************************************************************************************************
*FunctionName: ReadRemoteSoftInfo
*Description: ��ȡ�̼���Ϣ
*Input: remoteSoftInfo -- �̼���Ϣ
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��16��15:25:44
***************************************************************************************************/
MyRes ReadRemoteSoftInfo(RemoteSoftInfo * remoteSoftInfo)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	
	if(myfile && remoteSoftInfo)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));

		myfile->res = f_open(&(myfile->file), "0:/SINFO.NCD", FA_OPEN_EXISTING | FA_READ);

		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
			
			myfile->res = f_read(&(myfile->file), remoteSoftInfo, sizeof(RemoteSoftInfo), &(myfile->br));
			if((myfile->res == FR_OK) && (sizeof(RemoteSoftInfo) == myfile->br))
				statues = My_Pass;
			
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

/****************************************end of file************************************************/
