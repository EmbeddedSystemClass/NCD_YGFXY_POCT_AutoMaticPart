/***************************************************************************************************
*FileName: TestDataDao
*Description: ��������dao
*Author: xsx_kair
*Data: 2016��12��8��10:43:26
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"RecordDataDao.h"

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
*FunctionName:  writeRecordDataToFile
*Description:  �����¼��sd�����˴�Ϊͨ��д������ֻ����д���ݺ͸�������ͷ������ �������ݵ�crc��Ҫ�ڴ�֮ǰ�������
*Input: 	fileName -- д�ļ���
*			recordData -- д������
*			recordItemSize -- д�����ݵ�Ԫ����
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��3�� 15:42:09
***************************************************************************************************/
MyRes writeRecordDataToFile(const char * fileName, void * recordData, unsigned int recordItemSize)
{
	FatfsFileInfo_Def * myfile = NULL;
	DeviceRecordHeader * deviceRecordHeader = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	deviceRecordHeader = MyMalloc(DeviceRecordHeaderStructSize);
	
	if(myfile && recordData && deviceRecordHeader && fileName)
	{
		memset(myfile, 0, MyFileStructSize);
		memset(deviceRecordHeader, 0, DeviceRecordHeaderStructSize);
		
		myfile->res = f_open(&(myfile->file), fileName, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			f_lseek(&(myfile->file), 0);
			
			//��ȡ����ͷ
			myfile->res = f_read(&(myfile->file), deviceRecordHeader, DeviceRecordHeaderStructSize, &(myfile->br));
			if(deviceRecordHeader->crc != CalModbusCRC16Fun(deviceRecordHeader, DeviceRecordHeaderStructCrcSize, NULL))
			{
				deviceRecordHeader->itemSize = 0;
				deviceRecordHeader->userUpLoadIndex = 0;
				deviceRecordHeader->ncdUpLoadIndex = 0;
				deviceRecordHeader->crc = CalModbusCRC16Fun(deviceRecordHeader, DeviceRecordHeaderStructCrcSize, NULL);
			}
			
			//��дһ������ͷ����ֹû������ͷд�����쳣
			if(0 == myfile->size)
			{
				f_lseek(&(myfile->file), 0);
				myfile->res = f_write(&(myfile->file), deviceRecordHeader, DeviceRecordHeaderStructSize, &(myfile->bw));
				if((FR_OK != myfile->res) || (myfile->bw != DeviceRecordHeaderStructSize))
					goto Finally;
			}
			
			//д����
			f_lseek(&(myfile->file), deviceRecordHeader->itemSize * recordItemSize + DeviceRecordHeaderStructSize);
			myfile->res = f_write(&(myfile->file), recordData, recordItemSize, &(myfile->bw));

			if((FR_OK != myfile->res) || (myfile->bw != recordItemSize))
				goto Finally;
			
			//��������ͷ
			deviceRecordHeader->itemSize += 1;
			deviceRecordHeader->crc = CalModbusCRC16Fun(deviceRecordHeader, DeviceRecordHeaderStructCrcSize, NULL);
			
			f_lseek(&(myfile->file), 0);
			myfile->res = f_write(&(myfile->file), deviceRecordHeader, DeviceRecordHeaderStructSize, &(myfile->bw));
			if((FR_OK == myfile->res) && (myfile->bw == DeviceRecordHeaderStructSize))
				statues = My_Pass;
			
			Finally:
				f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	MyFree(deviceRecordHeader);
	
	return statues;
}

/***************************************************************************************************
*FunctionName:  readRecordDataFromFile
*Description:  ��sd��ȡ��¼���ݣ��������pageRequest��ȡ�����ȡ���ݱ�����recordData1��
*	����recordData1�б�������û��ϴ�������ȡ�����ݣ�recordData2�������Ŧ�����ϴ�������ȡ������
*Input:  	fileName -- �ļ���
*			pageRequest -- ��ҳ�������
*			deviceRecordHeader -- ��ȡ����ͷ��ַ
*			recordData1 -- �����ȡ�����ݵ�ַ1
*			recordData2 -- �����ȡ�����ݵ�ַ2
*			recordItemSize -- ��ȡ������Ŀ��С
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��3�� 16:14:55
***************************************************************************************************/
MyRes readRecordDataFromFileByPageRequest(const char * fileName, PageRequest * pageRequest, DeviceRecordHeader * deviceRecordHeader, 
	Page * page, unsigned short ItemByteSize)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);

	if(myfile && fileName && deviceRecordHeader && page && pageRequest)
	{
		memset(myfile, 0, MyFileStructSize);
		page->totalPageSize = 0;
		page->totalItemSize = 0;
		page->readItemSize = 0;
		
		myfile->res = f_open(&(myfile->file), fileName, FA_READ);
		
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			
			f_lseek(&(myfile->file), 0);
			
			//��ȡ����ͷ
			deviceRecordHeader->crc = 0;
			myfile->res = f_read(&(myfile->file), deviceRecordHeader, DeviceRecordHeaderStructSize, &(myfile->br));
			if(deviceRecordHeader->crc != CalModbusCRC16Fun(deviceRecordHeader, DeviceRecordHeaderStructCrcSize, NULL))
				goto Finally;
			else
			{
				page->totalItemSize = deviceRecordHeader->itemSize;
				page->totalPageSize = ((page->totalItemSize % pageRequest->pageSize) == 0) ? (page->totalItemSize / pageRequest->pageSize)
					: ((page->totalItemSize / pageRequest->pageSize)+1);
				
				if(pageRequest->pageIndex < page->totalPageSize)
				{
					
					myfile->tempValue1 = pageRequest->pageIndex;
					myfile->tempValue1 *= pageRequest->pageSize;
					
					//�ɶ�������ĿΪ0
					if(myfile->tempValue1 >= page->totalItemSize)
						goto Finally;
					else if(pageRequest->pageSize > (page->totalItemSize - myfile->tempValue1))
						page->readItemSize = page->totalItemSize - myfile->tempValue1;
					else
						page->readItemSize = pageRequest->pageSize;
					
					if(pageRequest->orderType == ASC)
					{
						myfile->tempValue1 += page->readItemSize;
						myfile->tempValue1 = page->totalItemSize - myfile->tempValue1;
					}

					myfile->tempValue1 *= ItemByteSize;
					myfile->tempValue1 += DeviceRecordHeaderStructSize;
					
					myfile->res = f_lseek(&(myfile->file), myfile->tempValue1);
					for(myfile->i=0; myfile->i<page->readItemSize; myfile->i++)
					{
						if(pageRequest->orderType == ASC)
							myfile->tempPoint = page->content[page->readItemSize - myfile->i - 1];
						else
							myfile->tempPoint = page->content[myfile->i];
						myfile->res = f_read(&(myfile->file), myfile->tempPoint, ItemByteSize, &(myfile->br));
						
						if(myfile->res != FR_OK)
							break;
					}
				}
			}
			
			statues = My_Pass;
			
			Finally:
				f_close(&(myfile->file));
		}
	}
	MyFree(myfile);
	
	return statues;
}

MyRes readRecordDataFromFileByUpLoadIndex(const char * fileName, bool isNcd, DeviceRecordHeader * deviceRecordHeader, Page * page, 
	unsigned short ItemByteSize)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);

	if(myfile && fileName && deviceRecordHeader && page)
	{
		memset(myfile, 0, MyFileStructSize);
		memset(deviceRecordHeader, 0, DeviceRecordHeaderStructSize);
		
		myfile->res = f_open(&myfile->file, fileName, FA_READ);
		
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			
			f_lseek(&myfile->file, 0);
			
			//��ȡ����ͷ
			myfile->res = f_read(&myfile->file, deviceRecordHeader, DeviceRecordHeaderStructSize, &(myfile->br));
			if(deviceRecordHeader->crc != CalModbusCRC16Fun(deviceRecordHeader, DeviceRecordHeaderStructCrcSize, NULL))
				goto Finally;
			
			//����ͷ��ȷ�������������ȡ����
			else
			{
				if(isNcd)
					myfile->tempValue1 = deviceRecordHeader->ncdUpLoadIndex;
				else
					myfile->tempValue1 = deviceRecordHeader->userUpLoadIndex;
				
				if(myfile->tempValue1 < deviceRecordHeader->itemSize)
				{
					myfile->tempValue1 *= ItemByteSize;
					myfile->tempValue1 += DeviceRecordHeaderStructSize;
					f_lseek(&myfile->file, myfile->tempValue1);

					for(myfile->i=0; myfile->i<MaxUpLoadTestDataNum; myfile->i++)
					{
						myfile->res = f_read(&myfile->file, page->content[myfile->i], ItemByteSize, &myfile->br);
						
						if(myfile->res != FR_OK)
							break;
					}
					page->readItemSize = myfile->i;
				}
			}
			
			statues = My_Pass;
			
			Finally:
				f_close(&(myfile->file));
		}
	}
	MyFree(myfile);
	
	return statues;
}

bool isRecordDataToBeUpLoad(const char * fileName, bool isNcd, DeviceRecordHeader * deviceRecordHeader)
{
	FatfsFileInfo_Def * myfile = NULL;
	bool isData = false;
	
	myfile = MyMalloc(MyFileStructSize);

	if(myfile && fileName && deviceRecordHeader)
	{
		memset(myfile, 0, MyFileStructSize);
		memset(deviceRecordHeader, 0, DeviceRecordHeaderStructSize);
		
		myfile->res = f_open(&myfile->file, fileName, FA_READ);
		
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			
			f_lseek(&myfile->file, 0);
			
			//��ȡ����ͷ
			myfile->res = f_read(&myfile->file, deviceRecordHeader, DeviceRecordHeaderStructSize, &(myfile->br));
			if(deviceRecordHeader->crc != CalModbusCRC16Fun(deviceRecordHeader, DeviceRecordHeaderStructCrcSize, NULL))
				isData = false;
			else
			{
				if(isNcd)
					myfile->tempValue1 = deviceRecordHeader->ncdUpLoadIndex;
				else
					myfile->tempValue1 = deviceRecordHeader->userUpLoadIndex;
				
				if(myfile->tempValue1 < deviceRecordHeader->itemSize)
					isData = true;
				else
					isData = false;
			}
			
			f_close(&(myfile->file));
		}
	}
	MyFree(myfile);
	
	return isData;
}
/***************************************************************************************************
*FunctionName:  plusRecordDataHeaderUpLoadIndexToFile
*Description:  �����ϴ���������ԭ�����ϼ���һ������
*Input:  	fileName -- �ļ���
*			userIndexPlusNum -- �û��ϴ������ӵĴ�С
*			ncdIndexPlusNum -- Ŧ�����ϴ������ӵĴ�С
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��3�� 16:16:44
***************************************************************************************************/
MyRes plusRecordDataHeaderUpLoadIndexToFile(const char * fileName, unsigned char userIndexPlusNum, unsigned char ncdIndexPlusNum)
{
	FatfsFileInfo_Def * myfile = NULL;
	DeviceRecordHeader * deviceRecordHeader = NULL;
	MyRes statues = My_Fail;
	
	myfile = MyMalloc(MyFileStructSize);
	deviceRecordHeader = MyMalloc(DeviceRecordHeaderStructSize);

	if(myfile && deviceRecordHeader)
	{
		memset(myfile, 0, MyFileStructSize);

		myfile->res = f_open(&(myfile->file), fileName, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
		
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);

			myfile->res = f_read(&(myfile->file), deviceRecordHeader, DeviceRecordHeaderStructSize, &(myfile->br));
			if((FR_OK == myfile->res) && (DeviceRecordHeaderStructSize == myfile->br) 
				&& (deviceRecordHeader->crc == CalModbusCRC16Fun(deviceRecordHeader, DeviceRecordHeaderStructCrcSize, NULL)))
			{
				deviceRecordHeader->userUpLoadIndex += userIndexPlusNum;
				deviceRecordHeader->ncdUpLoadIndex += ncdIndexPlusNum;
				deviceRecordHeader->crc = CalModbusCRC16Fun(deviceRecordHeader, DeviceRecordHeaderStructCrcSize, NULL);
				
				f_lseek(&(myfile->file), 0);
				myfile->res = f_write(&(myfile->file), deviceRecordHeader, DeviceRecordHeaderStructSize, &(myfile->bw));
				if((FR_OK == myfile->res)&&(myfile->bw == DeviceRecordHeaderStructSize))
					statues = My_Pass;
			}
			
			f_close(&(myfile->file));
		}
	}
	MyFree(myfile);
	MyFree(deviceRecordHeader);
	
	return statues;
}

/***************************************************************************************************
*FunctionName:  deleteRecordDataFile
*Description:  ɾ�����ݼ�¼�ļ�
*Input:  	fileName -- ɾ�����ļ���
*Output:  
*Return:  
*Author:  xsx
*Date: 2017��7��3�� 16:18:06
***************************************************************************************************/
MyRes deleteRecordDataFile(const char * fileName)
{
	FRESULT res;
	
	res = f_unlink(fileName);
	
	if((FR_OK == res) || (FR_NO_FILE == res))
		return My_Pass;
	else
		return My_Fail;
}
/****************************************end of file************************************************/
