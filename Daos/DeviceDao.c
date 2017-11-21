/***************************************************************************************************
*FileName: UserDao
*Description: ������dao
*Author: xsx_kair
*Data: 2017��2��16��11:31:22
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"DeviceDao.h"
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
static MyLock myDeviceLock;													//�豸��Ϣ�ļ���
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

MyRes SaveDeviceToFile(Device * device)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	
	if(myfile && device)
	{
		memset(myfile, 0, MyFileStructSize);

		myfile->res = f_open(&(myfile->file), DeviceFileName, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
			
			device->crc = CalModbusCRC16Fun(device, DeviceStructCrcSize, NULL);
			myfile->res = f_write(&(myfile->file), device, DeviceStructSize, &(myfile->bw));
				
			if((FR_OK == myfile->res)&&(myfile->bw == DeviceStructSize))
				statues = My_Pass;
				
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

MyRes ReadDeviceFromFile(Device * device)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);

	if(myfile && device)
	{
		memset(myfile, 0, MyFileStructSize);
		memset(device, 0, DeviceStructSize);

		myfile->res = f_open(&(myfile->file), DeviceFileName, FA_READ);
		
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
					
			f_read(&(myfile->file), device, DeviceStructSize, &(myfile->br));
			
			if(device->crc == CalModbusCRC16Fun(device, DeviceStructCrcSize, NULL))
				statues = My_Pass;
			
			f_close(&(myfile->file));
		}
	}	
	MyFree(myfile);
	
	return statues;
}

MyRes deleteDeviceFile(void)
{
	FRESULT res;
	
	res = f_unlink(DeviceFileName);
	
	if((FR_OK == res) || (FR_NO_FILE == res))
		return My_Pass;
	else
		return My_Fail;
}

MyLock * getMyDeviceLock(void)
{
	return &myDeviceLock;
}

/****************************************end of file************************************************/
