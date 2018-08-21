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
#include	"System_Data.h"

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
static MyRes CheckCardIsTimeOut(ReadQRCodeBuffer * readQRCodeBuffer);
static ScanCodeResult AnalysisQRVersion0(ReadQRCodeBuffer * readQRCodeBuffer);
static ScanCodeResult AnalysisQRVersion2(ReadQRCodeBuffer * readQRCodeBuffer);
static ScanCodeResult AnalysisQRVersion3(ReadQRCodeBuffer * readQRCodeBuffer);
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
		StartMotorAction(&readQRCodeBuffer->motorAction, true);
		
		OpenCodeScanner();
	
		xQueueReset(GetUsart3RXQueue());
		
		//���ö�ʱ��
		timer_SetAndStart(&(readQRCodeBuffer->timer), MAX_SCAN_QR_TIME);
		
		readQRCodeBuffer->motorAction.motorActionEnum = Motor2MoveDef;
		
		readQRCodeBuffer->scanResult = CardCodeScanning;
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
					StartMotorAction(&readQRCodeBuffer->motorAction, true);
				}
				else
				{
					readQRCodeBuffer->motorAction.motorParm = Motor2_PutDownCardLocation;
					StartMotorAction(&readQRCodeBuffer->motorAction, true);
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
	StartMotorAction(&readQRCodeBuffer->motorAction, true);
					
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
	ReceiveDataFromQueue(GetUsart3RXQueue(), NULL, readQRCodeBuffer->originalcode , MAX_QR_CODE_LENGHT, &(readQRCodeBuffer->originalCodeLen), 1, FreeRTOSDelay_50
		, FreeRTOSDelay_50, FreeRTOSDelay_50);
	
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
	/*���ݽ���ʧ��*/
	if(pdFAIL == MyDencrypt(readQRCodeBuffer->originalcode, readQRCodeBuffer->decryptcode, readQRCodeBuffer->originalCodeLen))
	{
		readQRCodeBuffer->scanResult = CardCodeScanFail;
		return;
	}

	memcpy(readQRCodeBuffer->originalcode, readQRCodeBuffer->decryptcode, readQRCodeBuffer->originalCodeLen);
	readQRCodeBuffer->pbuf2 = readQRCodeBuffer->originalcode;
	
	//check qr version
	readQRCodeBuffer->cardQR->qrVersion = readQRCodeBuffer->decryptcode[0];
	if(readQRCodeBuffer->cardQR->qrVersion >= '0' && readQRCodeBuffer->cardQR->qrVersion<= '9')
		;
	//have no qr version filed
	else
		readQRCodeBuffer->cardQR->qrVersion = QRVersion0Define;			//version 0

	switch(readQRCodeBuffer->cardQR->qrVersion)
	{
		case QRVersion0Define: readQRCodeBuffer->scanResult = AnalysisQRVersion0(readQRCodeBuffer); break;
		
		case QRVersion2Define: readQRCodeBuffer->scanResult = AnalysisQRVersion2(readQRCodeBuffer); break;
		
		case QRVersion3Define: readQRCodeBuffer->scanResult = AnalysisQRVersion3(readQRCodeBuffer); break;
		
		default : readQRCodeBuffer->scanResult = CardUnsupported; break;
	}
	
	#if(DeviceUseType == Device_Demo)
		readQRCodeBuffer->cardQR->CardWaitTime *= 20;
	#elif(DeviceUseType == Device_FastDemo)
		readQRCodeBuffer->cardQR->CardWaitTime *= 3;
    #else
        readQRCodeBuffer->cardQR->CardWaitTime *= 60;
	#endif
}

static MyRes CheckCardIsTimeOut(ReadQRCodeBuffer * readQRCodeBuffer)
{
	if(readQRCodeBuffer)
	{
		getSystemTime(&readQRCodeBuffer->dateTime);
		
		if(readQRCodeBuffer->cardQR->CardBaoZhiQi.year == readQRCodeBuffer->dateTime.year)
		{
			if(readQRCodeBuffer->cardQR->CardBaoZhiQi.month == readQRCodeBuffer->dateTime.month)
			{
				if(readQRCodeBuffer->cardQR->CardBaoZhiQi.day >= readQRCodeBuffer->dateTime.day)
					return My_Pass;
			}
			else if(readQRCodeBuffer->cardQR->CardBaoZhiQi.month > readQRCodeBuffer->dateTime.month)
				return My_Pass;
		}
		else if(readQRCodeBuffer->cardQR->CardBaoZhiQi.year > readQRCodeBuffer->dateTime.year)
			return My_Pass;
	}
	
	return My_Fail;
}

static ScanCodeResult AnalysisQRVersion0(ReadQRCodeBuffer * readQRCodeBuffer)
{
	if(readQRCodeBuffer == NULL)
		return CardCodeScanFail;

	readQRCodeBuffer->cardQR->calMode = 1;				//T/C
	readQRCodeBuffer->cardQR->qu1Ise = 0;
	readQRCodeBuffer->cardQR->qu2Ise = 0;
	readQRCodeBuffer->cardQR->qu3Ise = 0;
	readQRCodeBuffer->cardQR->ItemBiaoQu[0][3] = 0;
	readQRCodeBuffer->cardQR->ItemBiaoQu[1][3] = 0;
	readQRCodeBuffer->cardQR->ItemBiaoQu[2][3] = 0;
		
	//item
	readQRCodeBuffer->pbuf1 = strtok(readQRCodeBuffer->decryptcode, "#");
	if(readQRCodeBuffer->pbuf1)
	{
		if(getItemConstData(&(readQRCodeBuffer->cardQR->itemConstData), readQRCodeBuffer->pbuf1) == My_Fail)
			return CardUnsupported;
	}
	else
		return CardCodeScanFail;
	
	//channel num
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ChannelNum = strtol(readQRCodeBuffer->pbuf1, NULL, 10);
	else
		return CardCodeScanFail;
	
	//t location
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemLocation = strtol(readQRCodeBuffer->pbuf1 , NULL, 10);
	else
		return CardCodeScanFail;
	
	//fenduan 1
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
	
	//fenduan 2
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;

	//quxian 1
	for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[0][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
	}
	
	/*��׼����2*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[0] > 0)
	{
		for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[1][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
	
	/*��׼����3*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[1] > 0)
	{
		for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[2][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
		
	/*��ȡ��⿨��Ӧʱ��*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->cardQR->CardWaitTime = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	}
	else
		return CardCodeScanFail;
		
	/*��ȡ��⿨C��λ��*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CLineLocation = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;


	/*��ȡ��⿨����*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->PiHao, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;
	
	
	/*��ȡ��⿨����*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->piNum, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;

	/*��ȡ��⿨������*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		static char year[10] ,month[10],day[10];
		memcpy(year, readQRCodeBuffer->pbuf1, 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.year = strtol(year , NULL , 10);

		memcpy(month, &readQRCodeBuffer->pbuf1[2], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.month = (unsigned char)strtod(month , NULL );

		memcpy(day, &readQRCodeBuffer->pbuf1[4], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.day = (unsigned char)strtod(day , NULL );
		
		if(My_Fail == CheckCardIsTimeOut(readQRCodeBuffer))
			return CardCodeTimeOut;
	}
	else
		return CardCodeScanFail;

	/*��ȡ��ά��CRC*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->tempV1 = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
		
		if(readQRCodeBuffer->tempV1 != CalModbusCRC16Fun(readQRCodeBuffer->originalcode , readQRCodeBuffer->originalCodeLen - strlen(readQRCodeBuffer->pbuf1), NULL))
			return CardCodeCRCError;
	}
	else
		return CardCodeScanFail;
	
	return CardCodeScanOK;
}

static ScanCodeResult AnalysisQRVersion2(ReadQRCodeBuffer * readQRCodeBuffer)
{
	if(readQRCodeBuffer == NULL)
		return CardCodeScanFail;

	readQRCodeBuffer->cardQR->qu1Ise = 0;
	readQRCodeBuffer->cardQR->qu2Ise = 0;
	readQRCodeBuffer->cardQR->qu3Ise = 0;
	readQRCodeBuffer->cardQR->ItemBiaoQu[0][3] = 0;
	readQRCodeBuffer->cardQR->ItemBiaoQu[1][3] = 0;
	readQRCodeBuffer->cardQR->ItemBiaoQu[2][3] = 0;
	
	//check crc
	readQRCodeBuffer->pbuf1 = strtok(readQRCodeBuffer->decryptcode, "&");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->pbuf2 = strtok(NULL, "&");
		readQRCodeBuffer->tempV1 = strtol(readQRCodeBuffer->pbuf2, NULL, 10);
		if(readQRCodeBuffer->tempV1 != CalModbusCRC16Fun(readQRCodeBuffer->pbuf1 , strlen(readQRCodeBuffer->pbuf1), NULL))
			return CardCodeCRCError;
	}
	else
		return CardCodeScanFail;
		
	//qr versin
	readQRCodeBuffer->pbuf1 = strtok(readQRCodeBuffer->decryptcode, "#");
	if(!readQRCodeBuffer->pbuf1)
		return CardCodeScanFail;
					
	//item
	readQRCodeBuffer->pbuf1 = strtok(NULL, "#");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->tempV1 = strtol(readQRCodeBuffer->pbuf1, NULL, 10);
		if(getItemConstDataByIndex(&(readQRCodeBuffer->cardQR->itemConstData), readQRCodeBuffer->tempV1) == My_Fail)
			return CardUnsupported;
	}
	else
		return CardCodeScanFail;
	
	//channel num
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ChannelNum = strtol(readQRCodeBuffer->pbuf1, NULL, 10);
	else
		return CardCodeScanFail;
	
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->calMode = strtol(readQRCodeBuffer->pbuf1, NULL, 10);
	else
		return CardCodeScanFail;
		
	//t location
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemLocation = strtol(readQRCodeBuffer->pbuf1 , NULL, 10);
	else
		return CardCodeScanFail;
	
	//fenduan 1
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
	
	//fenduan 2
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;

	//quxian 1
	for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[0][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
	}
	
	/*��׼����2*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[0] > 0)
	{
		for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[1][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
	
	/*��׼����3*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[1] > 0)
	{
		for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[2][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
		
	/*��ȡ��⿨��Ӧʱ��*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CardWaitTime = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;
		
	/*��ȡ��⿨C��λ��*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CLineLocation = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;


	/*��ȡ��⿨����*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->PiHao, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;
	
	
	/*��ȡ��⿨����*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->piNum, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;

	/*��ȡ��⿨������*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		static char year[10] ,month[10],day[10];
		memcpy(year, readQRCodeBuffer->pbuf1, 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.year = strtol(year , NULL , 10);

		memcpy(month, &readQRCodeBuffer->pbuf1[2], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.month = (unsigned char)strtod(month , NULL );

		memcpy(day, &readQRCodeBuffer->pbuf1[4], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.day = (unsigned char)strtod(day , NULL );
		
		if(My_Fail == CheckCardIsTimeOut(readQRCodeBuffer))
			return CardCodeTimeOut;
	}
	else
		return CardCodeScanFail;

	return CardCodeScanOK;
}

static ScanCodeResult AnalysisQRVersion3(ReadQRCodeBuffer * readQRCodeBuffer)
{
	if(readQRCodeBuffer == NULL)
		return CardCodeScanFail;
	
	//check crc
	readQRCodeBuffer->pbuf1 = strtok(readQRCodeBuffer->decryptcode, "&");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->pbuf2 = strtok(NULL, "&");
		readQRCodeBuffer->tempV1 = strtol(readQRCodeBuffer->pbuf2, NULL, 10);
		if(readQRCodeBuffer->tempV1 != CalModbusCRC16Fun(readQRCodeBuffer->pbuf1 , strlen(readQRCodeBuffer->pbuf1), NULL))
			return CardCodeCRCError;
	}
	else
		return CardCodeScanFail;

	//item
	readQRCodeBuffer->tempV1 = readQRCodeBuffer->decryptcode[1]-0x30;
	if(getItemConstDataByIndex(&(readQRCodeBuffer->cardQR->itemConstData), readQRCodeBuffer->tempV1) == My_Fail)
		return CardUnsupported;
			
	//channel num
	readQRCodeBuffer->cardQR->ChannelNum = readQRCodeBuffer->decryptcode[2]-0x30;
	//T/C or T/T+C
	readQRCodeBuffer->cardQR->calMode = readQRCodeBuffer->decryptcode[3]-0x30;
	//ָ����־
	readQRCodeBuffer->tempV1 = readQRCodeBuffer->decryptcode[4]-0x30;
	if(readQRCodeBuffer->tempV1&0x01)
		readQRCodeBuffer->cardQR->qu1Ise = 1;
	else
		readQRCodeBuffer->cardQR->qu1Ise = 0;
			
	if(readQRCodeBuffer->tempV1&0x02)
		readQRCodeBuffer->cardQR->qu2Ise = 1;
	else
		readQRCodeBuffer->cardQR->qu2Ise = 0;
			
	if(readQRCodeBuffer->tempV1&0x04)
		readQRCodeBuffer->cardQR->qu3Ise = 1;
	else
		readQRCodeBuffer->cardQR->qu3Ise = 0;
			
	//T location
	readQRCodeBuffer->pbuf1 = strtok(&readQRCodeBuffer->decryptcode[5] , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemLocation = strtol(readQRCodeBuffer->pbuf1 , NULL, 10);
	else
		return CardCodeScanFail;

	/*��ȡ��⿨��׼�����ٽ�Ũ��1*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
	
	/*��ȡ��⿨��׼�����ٽ�Ũ��2*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;

	/*��׼����1*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemBiaoQu[0][0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
		
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemBiaoQu[0][1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
		
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemBiaoQu[0][2] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
		
	if(readQRCodeBuffer->cardQR->qu1Ise)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[0][3] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
	}
	
	/*��׼����2*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[0] > 0)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[1][0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[1][1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[1][2] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		if(readQRCodeBuffer->cardQR->qu2Ise)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[1][3] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
	
	/*��׼����3*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[1] > 0)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[2][0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[2][1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[2][2] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		if(readQRCodeBuffer->cardQR->qu3Ise)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[2][3] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
		
	/*��ȡ��⿨��Ӧʱ��*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CardWaitTime = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;
		
	/*��ȡ��⿨C��λ��*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CLineLocation = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;


	/*��ȡ��⿨����*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->PiHao, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;
	
	
	/*��ȡ��⿨����*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->piNum, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;

	/*��ȡ��⿨������*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		static char year[10] ,month[10],day[10];
		memcpy(year, readQRCodeBuffer->pbuf1, 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.year = strtol(year , NULL , 10);

		memcpy(month, &readQRCodeBuffer->pbuf1[2], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.month = (unsigned char)strtod(month , NULL );

		memcpy(day, &readQRCodeBuffer->pbuf1[4], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.day = (unsigned char)strtod(day , NULL );
		
		if(My_Fail == CheckCardIsTimeOut(readQRCodeBuffer))
			return CardCodeTimeOut;
	}
	else
		return CardCodeScanFail;

	return CardCodeScanOK;
}
