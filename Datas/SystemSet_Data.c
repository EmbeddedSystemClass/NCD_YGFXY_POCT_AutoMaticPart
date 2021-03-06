/***************************************************************************************************
*FileName:	SystemSet_Data
*Description:	系统设置参数
*Author: xsx_kair
*Data:	2016年12月16日16:20:43
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"SystemSet_Data.h"

#include	"StringDefine.h"
#include	"MyTools.h"
#include	"CRC16.h"

#include	<string.h>
#include	"stdio.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static SystemSetData GBSystemSetData;								//系统参数


static bool isShowRealValue = false;								//是否显示真实数据
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
*FunctionName: setDefaultSystemSetData
*Description: 初始化为默认配置
*Input: systemSetData -- 需要重置的系统设置地址
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月19日09:17:30
***************************************************************************************************/
void setDefaultSystemSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
	{
		memset(systemSetData, 0, SystemSetDataStructSize);

		systemSetData->isAutoPrint = true;
		systemSetData->ledLightIntensity = 100;
		systemSetData->ledSleepTime = 60;

		systemSetData->wireNetSet.isStaticIp = false;

		systemSetData->testLedLightIntensity = 200;
        
        memcpy(systemSetData->printInfo.header, CompanyNameStr, strlen(CompanyNameStr));
        systemSetData->printInfo.crc = CalModbusCRC16Fun(&systemSetData->printInfo, sizeof(PrintInfo)-2, NULL);
        
		memset(systemSetData->parm, 0, 480);
		
		systemSetData->crc = CalModbusCRC16Fun(systemSetData, SystemSetDataStructCrcSize, NULL);
	}
}

SystemSetData * getGBSystemSetData(void)
{
	return &GBSystemSetData;
}

void readGbSystemSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
		memcpy(systemSetData, &GBSystemSetData, SystemSetDataStructSize);
}


void upDateSystemSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
	{	
		memcpy(&GBSystemSetData, systemSetData, SystemSetDataStructSize);
	}
}

unsigned short getSystemTestLedLightIntensity(void)
{
	return GBSystemSetData.testLedLightIntensity;
}

void readSystemServerSerData(ServerSet * server)
{
	if(server)
		memcpy(server, &GBSystemSetData.serverSet, ServerSetStructSize);
}
/***************************************************************************************************
*FunctionName: setIsShowRealValue, IsShowRealValue
*Description: 是否显示真实值
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月27日14:04:02
***************************************************************************************************/
void setIsShowRealValue(bool isShow)
{
	isShowRealValue = isShow;
}

bool IsShowRealValue(void)
{
	return isShowRealValue;
}

void readPrintInfo(PrintInfo * printInfo)
{
    if(printInfo)
    {
        if(GBSystemSetData.printInfo.crc == CalModbusCRC16Fun(&GBSystemSetData.printInfo, sizeof(PrintInfo)-2, NULL))
        {
            memcpy(printInfo, &GBSystemSetData.printInfo, sizeof(PrintInfo));
            return;
        }
        
        memcpy(printInfo->header, CompanyNameStr, strlen(CompanyNameStr));
    } 
}

/****************************************end of file************************************************/
