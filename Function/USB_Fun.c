/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"USB_Fun.h"
#include	"QueueUnits.h"
#include	"StringDefine.h"

#include 	"usbd_hid_core.h"
#include 	"usbd_usr.h"
#include 	"usbd_desc.h"
#include 	"usb_conf.h" 

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/******************************************Static Variables***************************************/
/***************************************************************************************************/
extern USB_OTG_CORE_HANDLE USB_OTG_dev;
/***************************************************************************************************/
/******************************************Static Methods***************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
MyRes CommunicateWithServerByUSB(HttpBuf * httpBuffer)
{
	xQueueReset(GetUSBRXQueue());
	memset(httpBuffer->recvBuf, 0, 100);
	
	//start send data
	httpBuffer->tempP = httpBuffer->sendBuf;
	httpBuffer->tempInt = httpBuffer->sendDataLen;
	while(httpBuffer->tempInt > 0)
	{
		if(httpBuffer->tempInt > 64)
			httpBuffer->recvDataLen = 64;
		else
			httpBuffer->recvDataLen = httpBuffer->tempInt;
			
		USBD_HID_SendReport(&USB_OTG_dev, httpBuffer->tempP, 64);
		httpBuffer->tempInt -= httpBuffer->recvDataLen;
		httpBuffer->tempP += httpBuffer->recvDataLen;
	}
	
	//data send end ,wait host result
	ReceiveDataFromQueue(GetUSBRXQueue(), NULL, httpBuffer->recvBuf, 64, &httpBuffer->tempShort, 1, 1000 / portTICK_RATE_MS, FreeRTOSZeroDelay, FreeRTOSTenDelay);
	httpBuffer->recvDataLen = httpBuffer->tempShort;
	httpBuffer->tempP = strstr(httpBuffer->recvBuf, SuccessStr);
	if(httpBuffer->tempP)
		return My_Pass;
	else
		return My_Fail;
}
/****************************************end of file************************************************/
