
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
#include	"StringDefine.h"
#include	"MyMem.h"
#include	"Delay.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
static xSemaphoreHandle xGSMMutex = NULL;									//WIFI互斥量
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
MyRes ComWithSim800c(char * txBuf, const char *strcmp, char *rxBuf, unsigned short maxRxLen, portTickType delayTime, unsigned char maxErrorCnt, portTickType waitTime)
{
	unsigned char errorCnt = 0;
    unsigned char recvLen = 0;

    //清空队列数据
    while(pdPASS == xQueueReceive(GetUsart5RXQueue(), rxBuf, 10 / portTICK_RATE_MS));
    
	for(errorCnt = 0; errorCnt < 1; errorCnt++)
	{
        SendDataToQueue(GetUsart5TXQueue(), NULL, txBuf, strlen(txBuf), 1, delayTime, 0, EnableUsart5TXInterrupt);

		if(rxBuf)
		{
			memset(rxBuf, 0, maxRxLen);
			
            while(pdPASS == xQueueReceive(GetUsart5RXQueue(), rxBuf+recvLen, delayTime))
            {
                recvLen++;
                if(strcmp)
				{
					if(strstr(rxBuf, strcmp))
                        return My_Pass;
				}
                else
                    return My_Pass;
            }
		}
		else
			return My_Pass;
	}

	return My_Fail;
}

bool checkIsATMode(char * recvBuf)
{
	if(My_Fail == ComWithSim800c(ATStr, AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3, 100/portTICK_RATE_MS))
		return false;
	else
		return true;
}

void readGSMInd(Sim800InitBuf * sim800InitBuf, unsigned char * ind)
{
	if(My_Fail == ComWithSim800c("AT+CSQ\r", NULL, sim800InitBuf->recvBuf, 100, 100 / portTICK_RATE_MS, 3, 100/portTICK_RATE_MS))
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
	if(My_Fail == ComWithSim800c("AT+CNUM\r", NULL, sim800InitBuf->recvBuf, 100, 2000 / portTICK_RATE_MS, 3, 2000/portTICK_RATE_MS))
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
	if(My_Fail == ComWithSim800c("AT+CCID\r", NULL, sim800InitBuf->recvBuf, 100, 100 / portTICK_RATE_MS, 3, 100/portTICK_RATE_MS))
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
		if(My_Fail == ComWithSim800c(ATStr, AT_OK, recvBuf, 100, 2000 / portTICK_RATE_MS, 3, 1000/portTICK_RATE_MS))
			goto END;

		//"关闭移动场景\r");
        if(My_Fail == ComWithSim800c("AT+CIPSHUT\r", "SHUT OK", recvBuf, 100, 5000 / portTICK_RATE_MS, 3, 1000/portTICK_RATE_MS))
			goto END;
		if(My_Fail == ComWithSim800c("AT+CIPCLOSE\r", NULL, recvBuf, 100, 5000 / portTICK_RATE_MS, 3, 1000/portTICK_RATE_MS))
			goto END;
		
		//"设置GPRS移动台类型为B\r");
        if(My_Fail == ComWithSim800c("AT+CGCLASS=\"B\"\r", AT_OK, recvBuf, 100, 5000 / portTICK_RATE_MS, 3, 1000/portTICK_RATE_MS))
			goto END;
		
		//"定义PDP上下文：1, IP, CMNET\r");
        if(My_Fail == ComWithSim800c("AT+CGDCONT=1,\"IP\",\"CMNET\"\r", AT_OK, recvBuf, 100, 5000 / portTICK_RATE_MS, 3, 1000/portTICK_RATE_MS))
			goto END;
		
		//"附着GPRS业务\r");
        if(My_Fail == ComWithSim800c("AT+CGATT=1\r", AT_OK, recvBuf, 100, 5000 / portTICK_RATE_MS, 3, 5000/portTICK_RATE_MS))
			goto END;
		
		//"设置GPRS连接模式为: GPRS\r");
        if(My_Fail == ComWithSim800c("AT+CIPCSGP=1,\"CMNET\"\r", AT_OK, recvBuf, 100, 5000 / portTICK_RATE_MS, 3, 1000/portTICK_RATE_MS))
			goto END;
		
		//"设置为单IP连接模式\r");
        if(My_Fail == ComWithSim800c("AT+CIPMUX=0\r", AT_OK, recvBuf, 100, 5000 / portTICK_RATE_MS, 3, 1000/portTICK_RATE_MS))
			goto END;
		
		//"设置接收数据显示IP头\r");
        if(My_Fail == ComWithSim800c("AT+CIPHEAD=0\r", AT_OK, recvBuf, 100, 5000 / portTICK_RATE_MS, 3, 1000/portTICK_RATE_MS))
			goto END;
		
		//设置为透传模式
        if(My_Fail == ComWithSim800c("AT+CIPMODE=1\r", AT_OK, recvBuf, 100, 5000 / portTICK_RATE_MS, 3, 1000/portTICK_RATE_MS))
			goto END;
		
		//"发起连接\r");
		if(My_Fail == ComWithSim800c("AT+CIPSTART=\"TCP\",\"116.62.108.201\",\"8080\"\r", "CONNECT", recvBuf, 500, 30000 / portTICK_RATE_MS, 1, 30000/portTICK_RATE_MS))
			goto END;
	
        vTaskDelay(1000 / portTICK_RATE_MS);
		status = My_Pass;
	}
	
	END:
		return status;
}

void closeSimModule(char * recvBuf)
{
	if(recvBuf)
	{
		if(My_Fail == ComWithSim800c(ATStr, AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3, 100/portTICK_RATE_MS))
			return;

		//"关闭移动场景\r");
        if(My_Fail == ComWithSim800c("AT+CIPSHUT\r", "SHUT OK", recvBuf, 100, 1000 / portTICK_RATE_MS, 3, 100/portTICK_RATE_MS))
			return;
		if(My_Fail == ComWithSim800c("AT+CIPCLOSE\r", NULL, recvBuf, 100, 1000 / portTICK_RATE_MS, 3, 100/portTICK_RATE_MS))
			return;

		//"附着GPRS业务\r");
        if(My_Fail == ComWithSim800c("AT+CGATT=0\r", AT_OK, recvBuf, 100, 1000 / portTICK_RATE_MS, 3, 100/portTICK_RATE_MS))
			return;
	}
}

MyRes CommunicateWithNcdServerInGPRS(HttpBuf * httpBuf)
{
	//清空队列数据
	xQueueReset(GetUsart5TXQueue());
	while(pdPASS == xQueueReceive(GetUsart5RXQueue(), httpBuf->recvBuf, 10 / portTICK_RATE_MS));
    
	//发送数据
	httpBuf->tempInt = httpBuf->sendDataLen;
	while(httpBuf->tempInt > 800)
	{
		if(My_Pass == SendDataToQueue(GetUsart5TXQueue(), NULL, httpBuf->sendBuf + (httpBuf->sendDataLen - httpBuf->tempInt), 
			800, 1, 1000 / portTICK_RATE_MS, 10 / portTICK_RATE_MS, EnableUsart5TXInterrupt))
		{
			httpBuf->tempInt -= 800;
			vTaskDelay(100 / portTICK_RATE_MS);
		}
		else
			goto END;
	}
		
	if(httpBuf->tempInt > 0)
	{
		if(My_Pass != SendDataToQueue(GetUsart5TXQueue(), NULL, httpBuf->sendBuf + (httpBuf->sendDataLen - httpBuf->tempInt), 
			httpBuf->tempInt, 1, 1000 / portTICK_RATE_MS, 10 / portTICK_RATE_MS, EnableUsart5TXInterrupt))
			goto END;
	}
		
	//接收数据,最好等待1s
	httpBuf->tempShort = 0;
	httpBuf->recvDataLen = 0;
	httpBuf->j = 0;
	memset(httpBuf->recvBuf, 0, HttpRecvBufSize);
	while(pdPASS == ReceiveDataFromQueue(GetUsart5RXQueue(), NULL, httpBuf->recvBuf, HttpRecvBufSize, 
		&httpBuf->tempShort, 1, 20000 / portTICK_RATE_MS, 1000 / portTICK_RATE_MS, 2000 / portTICK_RATE_MS))
	{
		if(httpBuf->isPost)
		{
			httpBuf->recvDataLen += httpBuf->tempShort;
			break;
		}
		
		//如果发生的是GET请求，则说明是下载固件，需要保存
		else
		{
			//查找文件长度参数
			if(httpBuf->j < 2)
			{
				httpBuf->tempP = strstr(httpBuf->recvBuf, fileLengthParmString);
				if(httpBuf->tempP)
				{
					httpBuf->fileLength = strtol(httpBuf->tempP+fileLengthParmStringLen, NULL, 10);
					httpBuf->fileLength -= fileStartStrLen;
					httpBuf->j++;
				}
				
				//查找文件头
				httpBuf->tempP = strstr(httpBuf->recvBuf, fileStartStr);
				if(httpBuf->tempP)
				{
					httpBuf->tempInt = httpBuf->tempP - httpBuf->recvBuf + fileStartStrLen;
					httpBuf->tempShort -= httpBuf->tempInt;
					WriteAppFile(httpBuf->recvBuf + httpBuf->tempInt, httpBuf->tempShort, true);
					httpBuf->recvDataLen += httpBuf->tempShort;
					httpBuf->j++;
				}
			}
			else
			{
				WriteAppFile(httpBuf->recvBuf, httpBuf->tempShort, false);
				httpBuf->recvDataLen += httpBuf->tempShort;
				
				if(httpBuf->recvDataLen >= httpBuf->fileLength)
					break;
			}
		}
	}
		
	END:
		if(httpBuf->isPost)
		{
			httpBuf->tempP = strstr(httpBuf->recvBuf, "success");
			if(httpBuf->tempP)
				return My_Pass;
		}
	
		return My_Fail;
}
