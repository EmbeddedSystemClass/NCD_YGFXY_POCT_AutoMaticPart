/***************************************************************************************************
*FileName:	SystemSet_Data
*Description:	ϵͳ���ò���
*Author: xsx_kair
*Data:	2016��12��16��16:20:43
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
static SystemSetData GBSystemSetData;								//ϵͳ����


static bool isShowRealValue = false;								//�Ƿ���ʾ��ʵ����
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
*Description: ��ʼ��ΪĬ������
*Input: systemSetData -- ��Ҫ���õ�ϵͳ���õ�ַ
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��19��09:17:30
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

		systemSetData->testLedLightIntensity = 1200;
		
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
/***************************************************************************************************
*FunctionName: setIsShowRealValue, IsShowRealValue
*Description: �Ƿ���ʾ��ʵֵ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��27��14:04:02
***************************************************************************************************/
void setIsShowRealValue(bool isShow)
{
	isShowRealValue = isShow;
}

bool IsShowRealValue(void)
{
	return isShowRealValue;
}

/****************************************end of file************************************************/
