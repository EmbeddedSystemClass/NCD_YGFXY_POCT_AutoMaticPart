/***************************************************************************************************
*FileName:
*Description:
*Author:
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"NcdClient_Fun.h"
#include	"TestDataDao.h"
#include	"DeviceDao.h"
#include	"SIM800_Fun.h"
#include	"StringDefine.h"
#include	"RX8025_Driver.h"
#include	"RemoteSoftDao.h"
#include	"IAP_Fun.h"

#include	"HttpBuf.h"
#include	"MyMem.h"
#include	"CRC16.h"

#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
static HttpBuf * httpBuf = NULL;
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static MyRes readServerTime(HttpBuf * httpBuf);
static void UpLoadDeviceInfo(HttpBuf * httpBuf);
static void UpLoadTestData(HttpBuf * httpBuf);
static void readRemoteFirmwareVersion(HttpBuf * httpBuf);
static void DownLoadFirmware(HttpBuf * httpBuf);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void NcdClientFunction(void)
{
	while(1)
	{
		//upload data every hour
		if(My_Pass == takeGSMMutex(1000 / portTICK_RATE_MS))
		{
				//vTaskDelay(60000 / portTICK_RATE_MS);
			
			httpBuf = MyMalloc(HttpBufStructSize);
			if(httpBuf)
			{
				//if(!getSystemTimeIsRead())
				//	readServerTime(httpBuf);
				
				//readRemoteFirmwareVersion(httpBuf);
				
				//DownLoadFirmware(httpBuf);
						/*UpLoadDeviceInfo(httpBuf);
						vTaskDelay(1000 / portTICK_RATE_MS);
					
				
						UpLoadTestData(httpBuf);
						vTaskDelay(1000 / portTICK_RATE_MS);*/

					
				MyFree(httpBuf);
			}
				
			giveGSMxMutex();
		}

		vTaskDelay(10000 / portTICK_RATE_MS);
	}
}

static MyRes readServerTime(HttpBuf * httpBuf)
{
	MyRes status = My_Fail;
	
	checkIsATMode(httpBuf->recvBuf);
	vTaskDelay(1000 / portTICK_RATE_MS);
	if(My_Pass == ConnectServer(httpBuf->recvBuf))
	{
		readSystemDeviceId(httpBuf->deviceId);
		
		sprintf(httpBuf->sendBuf, "POST %s HTTP/1.1\nHost: 116.62.108.201:8080\nConnection: keep-alive\nContent-Length:[##]\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\ndid=%.*s", 
			"/NCD_Server/up_dtime", DeviceIdLen+1, httpBuf->deviceId);
		httpBuf->tempP = strstr(httpBuf->sendBuf, "8\n\n");
		httpBuf->sendDataLen = strlen(httpBuf->tempP)-3;
		
		httpBuf->tempP = strstr(httpBuf->sendBuf, "[##]");
		sprintf(httpBuf->tempBuf2, "%4d", httpBuf->sendDataLen);
		memcpy(httpBuf->tempP, httpBuf->tempBuf2, 4);
		httpBuf->sendDataLen = strlen(httpBuf->sendBuf);
		httpBuf->isPost = true;
		if(My_Pass == CommunicateWithNcdServerInGPRS(httpBuf))
		{
			status = My_Pass;
			httpBuf->tempP = strstr(httpBuf->recvBuf, "success");
			if(RTC_SetTimeData2(httpBuf->tempP+7) == My_Pass)
				setSystemTimeIsRead(true);
		}
	}
	
	return status;
}
/*
static void UpLoadDeviceInfo(HttpBuf * httpBuf)
{
	if(true == getSystemDeviceInfoStatus())
	{
		if(My_Pass == ReadDeviceFromFile(&httpBuf->device))
		{
			readGbSystemSetData(&httpBuf->systemSetData);
			
			httpBuf->isPost = true;
			sprintf(httpBuf->tempBuf, "did=%s&dversion=%d&addr=%s\0",httpBuf->systemSetData.deviceId,  GB_SoftVersion, httpBuf->device.addr);
			sprintf(httpBuf->sendBuf, "POST %s HTTP/1.1\nHost: 116.62.108.201:8080\nConnection: keep-alive\nContent-Length: %d\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\n%s", 
				"/NCD_Server/up_device", strlen(httpBuf->tempBuf), httpBuf->tempBuf);
			httpBuf->sendDataLen = strlen(httpBuf->sendBuf);
			if(My_Pass == CommunicateWithNcdServerInGPRS(httpBuf))
			{
				setSystemDeviceInfoStatus(false);
			}
		}
	}
}*/

/***************************************************************************************************
*FunctionName: UpLoadTestData
*Description: 上传测试数据（数据和曲线）
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月20日16:43:44
***************************************************************************************************/
/*static void UpLoadTestData(HttpBuf * httpBuf)
{
	for(httpBuf->i=0; httpBuf->i<PageContentItemNum; httpBuf->i++)
	{
		httpBuf->page.content[httpBuf->i] = MyMalloc(TestDataStructSize);
		if(httpBuf->page.content[httpBuf->i] == NULL)
			goto END;
		httpBuf->testDataPoint = httpBuf->page.content[httpBuf->i];
		httpBuf->testDataPoint->crc = 0;
	}

	if(My_Pass != readTestDataFromFileByPageRequest(NULL, &httpBuf->deviceRecordHeader, &httpBuf->page))
		goto END;
		
	httpBuf->testDataPoint = httpBuf->page.content[0];
	for(httpBuf->i=0; httpBuf->i<PageContentItemNum; httpBuf->i++)
	{
		//如果crc校验正确，则开始上传
		if(httpBuf->testDataPoint->crc == CalModbusCRC16Fun(httpBuf->testDataPoint, TestDataStructCrcSize, NULL))
		{
			sprintf(httpBuf->tempBuf, "cardnum=%s&qrdata.cid=%s&device.did=%s&tester=%s&sampleid=%s&testtime=20%d-%d-%d %d:%d:%d&overtime=%d&cline=%d&tline=%d&bline=%d&t_c_v=%.3f&testv=%.*f&serialnum=%s-%s&t_isok=\0",
				httpBuf->testDataPoint->qrCode.piNum, httpBuf->testDataPoint->qrCode.PiHao, httpBuf->systemSetData.deviceId, httpBuf->testDataPoint->operator.name, 
				httpBuf->testDataPoint->sampleid, httpBuf->testDataPoint->testDateTime.year, httpBuf->testDataPoint->testDateTime.month, 
				httpBuf->testDataPoint->testDateTime.day, httpBuf->testDataPoint->testDateTime.hour, httpBuf->testDataPoint->testDateTime.min, 
				httpBuf->testDataPoint->testDateTime.sec, httpBuf->testDataPoint->time, httpBuf->testDataPoint->testSeries.C_Point.x, 
				httpBuf->testDataPoint->testSeries.T_Point.x, httpBuf->testDataPoint->testSeries.B_Point.x, httpBuf->testDataPoint->testSeries.t_c, 
				httpBuf->testDataPoint->qrCode.itemConstData.pointNum, httpBuf->testDataPoint->testSeries.result, httpBuf->testDataPoint->qrCode.PiHao, 
				httpBuf->testDataPoint->qrCode.piNum);
	
			if(httpBuf->testDataPoint->testResultDesc != ResultIsOK)
				strcat(httpBuf->tempBuf, "false\0");
			else
				strcat(httpBuf->tempBuf, "true\0");
			
			for(httpBuf->j=0; httpBuf->j<100; httpBuf->j++)
			{
				if(httpBuf->j == 0)
					sprintf(httpBuf->tempBuf2, "&series=[%d,%d,%d\0", httpBuf->testDataPoint->testSeries.TestPoint[httpBuf->j*3],
						httpBuf->testDataPoint->testSeries.TestPoint[httpBuf->j*3+1], httpBuf->testDataPoint->testSeries.TestPoint[httpBuf->j*3+2]);
				else
					sprintf(httpBuf->tempBuf2, ",%d,%d,%d\0", httpBuf->testDataPoint->testSeries.TestPoint[httpBuf->j*3],
						httpBuf->testDataPoint->testSeries.TestPoint[httpBuf->j*3+1], httpBuf->testDataPoint->testSeries.TestPoint[httpBuf->j*3+2]);
				strcat(httpBuf->tempBuf, httpBuf->tempBuf2);
			}
					
			strcat(httpBuf->tempBuf, "]\0");
			
			if(httpBuf->i < UserePageContentIndex)
			{
				//上传用户服务器，暂无
			}
			else
			{
				sprintf(httpBuf->sendBuf, "POST %s HTTP/1.1\nHost: 116.62.108.201:8080\nConnection: keep-alive\nContent-Length: %d\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\n%s", 
					"/NCD_Server/UpLoadYGFXY", strlen(httpBuf->tempBuf), httpBuf->tempBuf);
				httpBuf->sendDataLen = strlen(httpBuf->sendBuf);
				httpBuf->isPost = true;
				if(My_Pass == CommunicateWithNcdServerInGPRS(httpBuf))
					plusTestDataHeaderUpLoadIndexToFile(0, 1);
				else
					break;
			}
		}
	}
	
	END:	
		for(httpBuf->i=0; httpBuf->i<PageContentItemNum; httpBuf->i++)
			MyFree(httpBuf->page.content[httpBuf->i]);
}
*/
/***************************************************************************************************
*FunctionName: 
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static void readRemoteFirmwareVersion(HttpBuf * httpBuf)
{
	checkIsATMode(httpBuf->recvBuf);
	vTaskDelay(1000 / portTICK_RATE_MS);
	if(My_Pass == ConnectServer(httpBuf->recvBuf))
	{
		sprintf(httpBuf->sendBuf, "POST %s HTTP/1.1\nHost: 116.62.108.201:8080\nConnection: keep-alive\nContent-Length:[##]\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\ndeviceType=%s", 
			"/NCD_Server/deviceSoftInfo", DeviceTypeString);
		httpBuf->tempP = strstr(httpBuf->sendBuf, "8\n\n");
		httpBuf->sendDataLen = strlen(httpBuf->tempP)-3;
		
		httpBuf->tempP = strstr(httpBuf->sendBuf, "[##]");
		sprintf(httpBuf->tempBuf2, "%4d", httpBuf->sendDataLen);
		memcpy(httpBuf->tempP, httpBuf->tempBuf2, 4);
		httpBuf->sendDataLen = strlen(httpBuf->sendBuf);
		httpBuf->isPost = true;
		if(My_Pass == CommunicateWithNcdServerInGPRS(httpBuf))
		{
			httpBuf->tempP = strstr(httpBuf->recvBuf, "success");
			//解析最新固件版本
			httpBuf->remoteSoftInfo.RemoteFirmwareVersion = strtol(httpBuf->tempP+16, NULL, 10);
			
			//如果读取到的版本，大于当前版本，且大于当前保存的最新远程版本，则此次读取的是最新的
			if((httpBuf->remoteSoftInfo.RemoteFirmwareVersion > GB_SoftVersion) &&
				(httpBuf->remoteSoftInfo.RemoteFirmwareVersion > getGbRemoteFirmwareVersion()))
			{
				//解析最新固件MD5
				httpBuf->tempP2 = strtok(httpBuf->tempP, "#");
				if(httpBuf->tempP2)
				{
					httpBuf->tempP2 = strtok(NULL, "#");
					
					memcpy(httpBuf->remoteSoftInfo.md5, httpBuf->tempP2+4, 32);
					
					if(My_Pass == WriteRemoteSoftInfo(&(httpBuf->remoteSoftInfo)))
					{
						//md5保存成功后，才更新最新版本号，保存最新固件版本
						setGbRemoteFirmwareVersion(httpBuf->remoteSoftInfo.RemoteFirmwareVersion);
						setGbRemoteFirmwareMd5(httpBuf->remoteSoftInfo.md5);
						
						setIsSuccessDownloadFirmware(false);
					}
				}
			}	
		}
	}
}

static void DownLoadFirmware(HttpBuf * httpBuf)
{
	if((getGbRemoteFirmwareVersion() <= GB_SoftVersion) || (true == getIsSuccessDownloadFirmware()))
		return;
	
	checkIsATMode(httpBuf->recvBuf);
	vTaskDelay(1000 / portTICK_RATE_MS);
	if(My_Pass == ConnectServer(httpBuf->recvBuf))
	{
		sprintf(httpBuf->sendBuf, "GET %s?softName=%s HTTP/1.1\nHost: 116.62.108.201:8080\nConnection: keep-alive\n\n", 
			"/NCD_Server/DownloadSoftFile", DeviceTypeString);

		httpBuf->sendDataLen = strlen(httpBuf->sendBuf);
		httpBuf->isPost = false;
		CommunicateWithNcdServerInGPRS(httpBuf);
		if(My_Pass == checkNewFirmwareIsSuccessDownload())
			setIsSuccessDownloadFirmware(true);
	}
/*	UpLoadDeviceDataBuffer * upLoadDeviceDataBuffer = NULL;
	MyState_TypeDef status = My_Fail;
	
	//检查是否有更新，且未成功下载，则需要下载
	if((getGbRemoteFirmwareVersion() > GB_SoftVersion) && (false == getIsSuccessDownloadFirmware()))
	{
		upLoadDeviceDataBuffer = MyMalloc(sizeof(UpLoadDeviceDataBuffer));
	
		if(upLoadDeviceDataBuffer)
		{
			memset(upLoadDeviceDataBuffer, 0, sizeof(UpLoadDeviceDataBuffer));
			
			UpLoadData("/NCD_Server/deviceCodeDownload", NULL, 0, NULL, 0, "GET");
		}
		MyFree(upLoadDeviceDataBuffer);
	}*/
}

