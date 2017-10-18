/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"CodeScanner_Driver.h"
#include	"CodeScanFunction.h"

#include	"MyEncryptTool.h"
#include	"QueueUnits.h"
#include	"ItemConst_Data.h"
#include	"Motor_Fun.h"
#include	"Motor1_Fun.h"
#include	"Motor2_Fun.h"
#include	"Motor4_Fun.h"
#include	"Motor_Data.h"
#include 	"Usart3_Driver.h"
#include	"CardCheck_Driver.h"

#include	"Define.h"
#include	"CRC16.h"
#include	"MyMem.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void ReadBasicCodeData(ReadQRCodeBuffer * readQRCodeBuffer);
static void AnalysisCode(ReadQRCodeBuffer * readQRCodeBuffer);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

ScanCodeResult ScanCodeFun(QRCode * cardQR)
{
	ReadQRCodeBuffer * readQRCodeBuffer;
	ScanCodeResult scanResult = CardCodeScanning;
	
	readQRCodeBuffer = MyMalloc(ReadQRCodeBufferStructSize);
	if(readQRCodeBuffer)
	{
		memset(readQRCodeBuffer, 0, ReadQRCodeBufferStructSize);
		
		readQRCodeBuffer->cardQR = cardQR;
		memset(readQRCodeBuffer->cardQR, 0, QRCodeStructSize);
		
		readQRCodeBuffer->motorAction.motorActionEnum = Motor4MoveDef;
		readQRCodeBuffer->motorAction.motorParm = Motor4_CardLocation;
		if(My_Fail == StartMotorAction(&readQRCodeBuffer->motorAction, false, 3, 1000/portTICK_RATE_MS))
			readQRCodeBuffer->scanResult = CardCodeScanFail;
		while(false == isMotorMoveEnd(10000 / portTICK_RATE_MS));
		
		OpenCodeScanner();
	
		xQueueReset(GetUsart3RXQueue());
		
		//���ö�ʱ��
		timer_SetAndStart(&(readQRCodeBuffer->timer), MAX_SCAN_QR_TIME);
		
		readQRCodeBuffer->motorAction.motorActionEnum = Motor2MoveDef;
		
		while(readQRCodeBuffer->scanResult == CardCodeScanning)
		{
			if(OFF == readCaedCheckStatus())				//�����γ�
			{
				readQRCodeBuffer->scanResult = CardCodeCardOut;
			}
			else
			{
				readQRCodeBuffer->motorDir++;
				
				if(readQRCodeBuffer->motorDir%2 == 0)
				{
					readQRCodeBuffer->motorAction.motorParm = Motor2_WaitCardLocation;
					if(My_Fail == StartMotorAction(&readQRCodeBuffer->motorAction, false, 3, 1000/portTICK_RATE_MS))
					{
						readQRCodeBuffer->scanResult = CardCodeScanFail;
						break;
					}
					
					while(false == isMotorMoveEnd(1000 / portTICK_RATE_MS));
				}
				else
				{
					readQRCodeBuffer->motorAction.motorParm = Motor2_PutDownCardLocation;
					if(My_Fail == StartMotorAction(&readQRCodeBuffer->motorAction, false, 3, 1000/portTICK_RATE_MS))
					{
						readQRCodeBuffer->scanResult = CardCodeScanFail;
						break;
					}
					
					while(false == isMotorMoveEnd(1000 / portTICK_RATE_MS));
				}
				
				ReadBasicCodeData(readQRCodeBuffer);

				vTaskDelay(2 / portTICK_RATE_MS);
				
				if(TimerOut == timer_expired(&(readQRCodeBuffer->timer)))
					readQRCodeBuffer->scanResult = CardCodeScanTimeOut;				
			}
		}
	}

	CloseCodeScanner();
	
	readQRCodeBuffer->motorAction.motorParm = Motor2_PutDownCardLocation;
	StartMotorAction(&readQRCodeBuffer->motorAction, false, 3, 1000/portTICK_RATE_MS);
	while(false == isMotorMoveEnd(1000 / portTICK_RATE_MS));
					
	scanResult = readQRCodeBuffer->scanResult;
		
	MyFree(readQRCodeBuffer);
	
	return scanResult;
}

/***************************************************************************************************
*FunctionName��ReadBasicCodeData
*Description����ȡԭʼ��ά��
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��25��14:07:38
***************************************************************************************************/
static void ReadBasicCodeData(ReadQRCodeBuffer * readQRCodeBuffer)
{
	ReceiveDataFromQueue(GetUsart3RXQueue(), NULL, readQRCodeBuffer->originalcode , MAX_QR_CODE_LENGHT, &(readQRCodeBuffer->originalCodeLen), 1, 10 / portTICK_RATE_MS
		, 10 / portTICK_RATE_MS);
	
	if(readQRCodeBuffer->originalCodeLen > 0)
	{
		readQRCodeBuffer->originalCodeLen -= 1;
		readQRCodeBuffer->originalcode[readQRCodeBuffer->originalCodeLen] = 0;
		
		AnalysisCode(readQRCodeBuffer);
	}
}

/***************************************************************************************************
*FunctionName��DecryptCode
*Description�����벢������ά������
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��25��14:08:25
***************************************************************************************************/
static void AnalysisCode(ReadQRCodeBuffer * readQRCodeBuffer)
{
	unsigned char i=0;

	/*���ݽ���ʧ��*/
	if(pdFAIL == MyDencrypt(readQRCodeBuffer->originalcode, readQRCodeBuffer->decryptcode, readQRCodeBuffer->originalCodeLen))
		goto END;
	readQRCodeBuffer->decryptcode[readQRCodeBuffer->originalCodeLen] = 0;
	
	memcpy(readQRCodeBuffer->originalcode, readQRCodeBuffer->decryptcode, readQRCodeBuffer->originalCodeLen);
	readQRCodeBuffer->pbuf2 = readQRCodeBuffer->originalcode;
	
	/*��ȡ������Ŀ����*/
	readQRCodeBuffer->pbuf1 = strtok(readQRCodeBuffer->decryptcode, "#");
	if(readQRCodeBuffer->pbuf1)
	{
		memcpy(readQRCodeBuffer->cardQR->ItemName, readQRCodeBuffer->pbuf1 ,strlen(readQRCodeBuffer->pbuf1));
		if(getItemConstData(&(readQRCodeBuffer->cardQR->itemConstData), readQRCodeBuffer->cardQR->ItemName) == My_Fail)
		{
			readQRCodeBuffer->scanResult = CardUnsupported;
			goto END;
		}
	}
	else
		goto END;
		
	//��ȡ��⿨�ϵļ��ͨ��
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ChannelNum = strtol(readQRCodeBuffer->pbuf1, NULL, 10);
	else
		goto END;
	
	/*��ȡ��⿨T��λ��*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemLocation = strtol(readQRCodeBuffer->pbuf1 , NULL, 10);
	else
		goto END;
		
	/*��ȡ��⿨��׼�����ٽ�Ũ��1*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		goto END;
	
	/*��ȡ��⿨��׼�����ٽ�Ũ��2*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		goto END;

	/*��׼����1*/
	for(i=0; i<3; i++)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[0][i] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			goto END;
	}
	
	/*��׼����2*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[0] > 0)
	{
		for(i=0; i<3; i++)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[1][i] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				goto END;
		}
	}
	
	/*��׼����3*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[1] > 0)
	{
		for(i=0; i<3; i++)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[2][i] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				goto END;
		}
	}
		
	/*��ȡ��⿨��Ӧʱ��*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CardWaitTime = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		goto END;
		
	/*��ȡ��⿨C��λ��*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CLineLocation = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		goto END;


	/*��ȡ��⿨����*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->PiHao, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		goto END;
	
	
	/*��ȡ��⿨����*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->piNum, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		goto END;

	/*��ȡ��⿨������*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->tempbuf[2] = 0;
		memcpy(readQRCodeBuffer->tempbuf, readQRCodeBuffer->pbuf1, 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.year = strtol(readQRCodeBuffer->tempbuf , NULL , 10);

		memcpy(readQRCodeBuffer->tempbuf, &readQRCodeBuffer->pbuf1[2], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.month = (unsigned char)strtod(readQRCodeBuffer->tempbuf , NULL );

		memcpy(readQRCodeBuffer->tempbuf, &readQRCodeBuffer->pbuf1[4], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.day = (unsigned char)strtod(readQRCodeBuffer->tempbuf , NULL );
	}
	else
		goto END;

	/*��ȡ��ά��CRC*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->cardQR->CRC16 = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
			
		readQRCodeBuffer->tempV1 = strlen(readQRCodeBuffer->pbuf1);
		goto END;
	}
	else
		goto END;
	
	END:
		if(readQRCodeBuffer->cardQR->CRC16 != CalModbusCRC16Fun(readQRCodeBuffer->pbuf2 , readQRCodeBuffer->originalCodeLen - 
			readQRCodeBuffer->tempV1, NULL))
			readQRCodeBuffer->scanResult = CardCodeCRCError;		
		//else if(My_Fail == CheckCardIsTimeOut(readQRCodeBuffer))
		//	readQRCodeBuffer->scanResult = CardCodeTimeOut;
		else
			readQRCodeBuffer->scanResult = CardCodeScanOK;
}

