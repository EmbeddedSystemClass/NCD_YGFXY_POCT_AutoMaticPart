/***************************************************************************************************
*FileName:RemoteSoft_Data
*Description：最新固件信息
*Author: xsx_kair
*Data:2017年2月21日09:18:33
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"RemoteSoft_Data.h"

#include	"MyTools.h"

#include	<string.h>
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static RemoteSoftInfo GB_RemoteSoftInfo;
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
*FunctionName: setGbRemoteFirmwareVersion, getGbRemoteFirmwareVersion
*Description: 更新和读取远程固件版本
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月20日16:42:29
***************************************************************************************************/
void setGbRemoteFirmwareVersion(unsigned short version)
{
	GB_RemoteSoftInfo.RemoteFirmwareVersion = version;
}

unsigned short getGbRemoteFirmwareVersion(void)
{
	return GB_RemoteSoftInfo.RemoteFirmwareVersion;
}

/***************************************************************************************************
*FunctionName: setIsSuccessDownloadFirmware， getIsSuccessDownloadFirmware
*Description: 更新和读取固件下载状态
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月21日09:09:50
***************************************************************************************************/
void setIsSuccessDownloadFirmware(bool status)
{
	GB_RemoteSoftInfo.isSuccessDownloadFirmware = status;
}

bool getIsSuccessDownloadFirmware(void)
{
	return GB_RemoteSoftInfo.isSuccessDownloadFirmware;
}

/***************************************************************************************************
*FunctionName: setGbRemoteFirmwareMd5,checkMd5IsSame
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月21日09:26:20
***************************************************************************************************/
void setGbRemoteFirmwareMd5(char * md5)
{
	memcpy(GB_RemoteSoftInfo.md5, md5, 32);
}
bool checkMd5IsSame(char * sMd5, char * dMd5)
{
	if(true == CheckStrIsSame(sMd5, dMd5, 32))
		return true;
	else
		return false;
}

/***************************************************************************************************
*FunctionName: isNewVersionToBeDownload
*Description: 是否有新版本需要下载
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2018年1月24日 14:02:13
***************************************************************************************************/
bool isNewVersionToBeDownload(void)
{
	if(GB_RemoteSoftInfo.isSuccessDownloadFirmware == false && GB_RemoteSoftInfo.RemoteFirmwareVersion > GB_SoftVersion)
		return true;
	else
		return false;
}

/****************************************end of file************************************************/
