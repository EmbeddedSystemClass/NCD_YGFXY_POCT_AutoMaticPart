/***************************************************************************************************
*FileName:WifiFunction
*Description:一些WIFI模块的操作函数,函数只能在线程中使用
*Author:xsx
*Data:2016年4月30日16:06:36
***************************************************************************************************/


/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"SIM800_Fun.h"

#include 	"Usart5_Driver.h"
#include	"QueueUnits.h"
#include	"AppFileDao.h"
#include	"System_Data.h"

#include	"MyMem.h"
#include	"Delay.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
static xSemaphoreHandle xGSMMutex = NULL;									//WIFI互斥量
const char * fileStartStr = "i am zhangxiong^*^!";
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
void createGSMMutex(void)
{
	vSemaphoreCreateBinary(xGSMMutex);
}
MyRes takeGSMMutex(portTickType xBlockTime)
{
	if(pdPASS == xSemaphoreTake(xGSMMutex, xBlockTime))
		return My_Pass;
	else
		return My_Fail;
}
void giveGSMxMutex(void)
{
	xSemaphoreGive(xGSMMutex);
}

/***************************************************************************************************
*FunctionName: ComWithWIFI与wifi模块通信
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年3月7日09:41:31
***************************************************************************************************/
MyRes ComWithSim800c(char * txBuf, const char *strcmp, char *rxBuf, unsigned short maxRxLen, portTickType delayTime, unsigned char maxErrorCnt)
{
	MyRes statues = My_Fail;
	unsigned char errorCnt = 0;

	while(statues == My_Fail && errorCnt < maxErrorCnt)
	{
		errorCnt++;
		
		if(txBuf)
		{
			while(pdPASS == ReceiveDataFromQueue(GetUsart5RXQueue(), NULL, rxBuf, maxRxLen, NULL, 1, 0, 0));
			
			if(pdFAIL == SendDataToQueue(GetUsart5TXQueue(), NULL, txBuf, strlen(txBuf), 1, delayTime, 0, EnableUsart5TXInterrupt))
				continue;
		}
		
		if(rxBuf)
		{
			memset(rxBuf, 0, maxRxLen);
				
			if(My_Pass == ReceiveDataFromQueue(GetUsart5RXQueue(), NULL, rxBuf, maxRxLen, NULL, 1, delayTime, 0))
			{
				if(strcmp)
				{
					if(strstr(rxBuf, strcmp))
						statues = My_Pass;
				}
				else
					statues = My_Pass;
			}
		}
		else
			statues = My_Pass;
	}

	return statues;
}

bool checkIsATMode(char * recvBuf)
{
	if(My_Fail == ComWithSim800c(ATStr, AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
		return false;
	else
		return true;
}

void readGSMInd(Sim800InitBuf * sim800InitBuf, unsigned char * ind)
{
	if(My_Fail == ComWithSim800c("AT+CSQ\r", NULL, sim800InitBuf->recvBuf, 100, 100 / portTICK_RATE_MS, 3))
		*ind = 0;
	else
	{
		sim800InitBuf->tempP = strstr((const char *)sim800InitBuf->recvBuf, "+CSQ:");
			
		if(sim800InitBuf->tempP)
		{
			sim800InitBuf->tempInt = strtol(sim800InitBuf->tempP+5, NULL, 10);
			if(sim800InitBuf->tempInt == 0)
				*ind = 4;
			else if(sim800InitBuf->tempInt == 1)
				*ind = 3;
			else if(sim800InitBuf->tempInt >= 2 && sim800InitBuf->tempInt <= 30)
				*ind = 2;
			else if(sim800InitBuf->tempInt >= 31 && sim800InitBuf->tempInt <= 98)
				*ind = 1;
			else
				*ind = 0;
		}
	}
}

void readGSMPhoneNum(Sim800InitBuf * sim800InitBuf, char * num)
{
	if(My_Fail == ComWithSim800c("AT+CNUM\r", NULL, sim800InitBuf->recvBuf, 100, 100 / portTICK_RATE_MS, 3))
		memset(num, 0, 11);
	else
	{
		sim800InitBuf->tempP = strstr((const char *)sim800InitBuf->recvBuf, "+86");
		if(sim800InitBuf->tempP)
			memcpy(num, sim800InitBuf->tempP, 14);
	}
}

void readGSMIccid(Sim800InitBuf * sim800InitBuf, char * iccid)
{	
	if(My_Fail == ComWithSim800c("AT+CCID\r", NULL, sim800InitBuf->recvBuf, 100, 100 / portTICK_RATE_MS, 3))
		memset(iccid, 0, 20);
	else
	{
		sim800InitBuf->tempP = strstr((const char *)sim800InitBuf->recvBuf, "CCID\r\r\n");
		if(sim800InitBuf->tempP)
			memcpy(iccid, sim800InitBuf->tempP + 7, 20);
	}
}

MyRes ConnectServer(char * recvBuf)
{
	MyRes status = My_Fail;

	if(recvBuf)
	{
		if(My_Fail == ComWithSim800c(ATStr, AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
			goto END;

		//"关闭移动场景\r");
        if(My_Fail == ComWithSim800c("AT+CIPSHUT\r", "SHUT OK", recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
			goto END;
		if(My_Fail == ComWithSim800c("AT+CIPCLOSE\r", NULL, recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
			goto END;
		
		//"设置GPRS移动台类型为B\r");
        if(My_Fail == ComWithSim800c("AT+CGCLASS=\"B\"\r", AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
			goto END;
		
		//"定义PDP上下文：1, IP, CMNET\r");
        if(My_Fail == ComWithSim800c("AT+CGDCONT=1,\"IP\",\"CMNET\"\r", AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
			goto END;
		
		//"附着GPRS业务\r");
        if(My_Fail == ComWithSim800c("AT+CGATT=1\r", AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
			goto END;
		
		//"设置GPRS连接模式为: GPRS\r");
        if(My_Fail == ComWithSim800c("AT+CIPCSGP=1,\"CMNET\"\r", AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
			goto END;
		
		//"设置为单IP连接模式\r");
        if(My_Fail == ComWithSim800c("AT+CIPMUX=0\r", AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
			goto END;
		
		//"设置接收数据显示IP头\r");
        if(My_Fail == ComWithSim800c("AT+CIPHEAD=0\r", AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
			goto END;
		
		//设置为透传模式
        if(My_Fail == ComWithSim800c("AT+CIPMODE=1\r", AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3))
			goto END;
		
		//"发起连接\r");
		if(My_Fail == ComWithSim800c("AT+CIPSTART=\"TCP\",\"116.62.108.201\",\"8080\"\r", AT_OK, recvBuf, 500, 20000 / portTICK_RATE_MS, 1))
			goto END;
	
		if(strstr(recvBuf, "CONNECT"))
			status = My_Pass;
		else
		{
			if(My_Pass == ComWithSim800c(NULL, "CONNECT", recvBuf, 500, 20000 / portTICK_RATE_MS, 3))
				status = My_Pass;
		}
	}
	
	END:
		return status;
}

MyRes CommunicateWithNcdServerInGPRS(HttpBuf * httpBuf)
{
	unsigned short i = 0;
	unsigned short readSize = 0;
	
//	if(My_Pass == takeGSMMutex(1000 / portTICK_RATE_MS))
	{
		//清空队列数据
		while(pdPASS == ReceiveDataFromQueue(GetUsart5RXQueue(), NULL, httpBuf->recvBuf, 1000, 
				NULL, 1, 0, 0));
		
		//发送数据
		readSize = httpBuf->sendDataLen;
		while(readSize > 800)
		{
			if(My_Pass == SendDataToQueue(GetUsart5TXQueue(), NULL, httpBuf->sendBuf + (httpBuf->sendDataLen - readSize), 
				800, 1, 1000 / portTICK_RATE_MS, 10 / portTICK_RATE_MS, EnableUsart5TXInterrupt))
			{
				readSize -= 800;
				vTaskDelay(100 / portTICK_RATE_MS);
			}
			else
				goto END;
		}
		
		if(readSize > 0)
		{
			if(My_Pass == SendDataToQueue(GetUsart5TXQueue(), NULL, httpBuf->sendBuf + (httpBuf->sendDataLen - readSize), 
				readSize, 1, 1000 / portTICK_RATE_MS, 10 / portTICK_RATE_MS, EnableUsart5TXInterrupt))
			{
				;
			}
			else
				goto END;
		}
		
		//接收数据,最好等待1s

		while(pdPASS == ReceiveDataFromQueue(GetUsart5RXQueue(), NULL, httpBuf->recvBuf, 2000, 
			&readSize, 1, 20000 / portTICK_RATE_MS, 1000 / portTICK_RATE_MS))
		{
			//如果发生的是GET请求，则说明是下载固件，需要保存
			if(!httpBuf->isPost)
			{
				if(i == 0)
				{
					//查找文件头
					httpBuf->tempP = strstr(httpBuf->recvBuf, fileStartStr);
					if(httpBuf->tempP)
					{
						httpBuf->tempInt = httpBuf->tempP - httpBuf->recvBuf + strlen(fileStartStr);
						readSize -= httpBuf->tempInt;
						WriteAppFile(httpBuf->recvBuf + httpBuf->tempInt, readSize, true);
						i++;
					}
				}
				else
				{
					WriteAppFile(httpBuf->recvBuf, readSize, false);
				}
			}
			httpBuf->recvDataLen += readSize;
		}
		
		END:
//			giveGSMxMutex();
		
		if(httpBuf->isPost)
		{
			httpBuf->tempP = strstr(httpBuf->recvBuf, "success");
			if(httpBuf->tempP)
				return My_Pass;
		}
		else
		{
			/*if(My_Pass == checkNewFirmwareIsSuccessDownload())
			{
				setIsSuccessDownloadFirmware(true);
				statues = My_Pass;
			}*/
			return My_Pass;
		}
	}
}
