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
		
		snprintf(systemSetData->deviceId, DeviceIdLen+1, "NCD-Device");
		
		systemSetData->isAutoPrint = true;
		systemSetData->isMute = false;
		systemSetData->ledLightIntensity = 100;
		systemSetData->ledSleepTime = 60;

		systemSetData->wireNetSet.ipMode = Dynamic_IP;
		
		
		systemSetData->testLedLightIntensity = 200;
		
		systemSetData->crc = CalModbusCRC16Fun(systemSetData, SystemSetDataStructCrcSize, NULL);
	}
}

const SystemSetData * getGBSystemSetData(void)
{
	return &GBSystemSetData;
}


void upDateSystemSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
	{
		vTaskSuspendAll();
		
		memcpy(&GBSystemSetData, systemSetData, SystemSetDataStructSize);
		
		xTaskResumeAll();
	}
}

/***************************************************************************************************
*FunctionName: addAdjPram
*Description: ����һ��У׼�������Ȳ����Ƿ��Ѵ��ڣ����򸲸ǣ��������
*Input: adjData -- У׼����
*Output: 
*Return: MyRes -- fail��ʾ�޿���λ�ñ���
*Author: xsx
*Date: 2017��2��8��14:47:14
***************************************************************************************************/
MyRes addAdjPram(SystemSetData * systemSetData, AdjustData * adjData)
{
	unsigned char i = 0;
	
	if(adjData == NULL)
		return My_Fail;
	
	if(systemSetData->crc != CalModbusCRC16Fun(systemSetData, SystemSetDataStructCrcSize, NULL))
		setDefaultSystemSetData(systemSetData);
	
	//�����Ƿ����
	for(i=0; i<MaxAdjustItemNum; i++)
	{
		//���ڣ��滻
		if(CheckStrIsSame(systemSetData->adjustData[i].ItemName, adjData->ItemName, ItemNameLen) == true)
		{
			systemSetData->adjustData[i].parm = adjData->parm;
			break;
		}
	}
	
	//������
	if(i >= MaxAdjustItemNum)
	{
		//�ҿ���λ�ã����
		for(i=0; i<MaxAdjustItemNum; i++)
		{
			//���ֵĵ�һ���ַ���\0������
			if(systemSetData->adjustData[i].ItemName[0] == 0)
			{
				memcpy(&(systemSetData->adjustData[i]), adjData, sizeof(AdjustData));
				break;
			}
		}
	}
	
	//����У׼��Ŀ����
	if(i >= MaxAdjustItemNum)
		return My_Fail;
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: getAdjPram
*Description: ��ȡһ����Ŀ��У׼����
*Input: systemSetData -- ��ȡԴ�� adjData -- ��ȡ����Ŀ���ַ
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��8��15:01:17
***************************************************************************************************/
void getAdjPram(const SystemSetData * systemSetData, AdjustData * adjData)
{
	unsigned char i = 0;
	
	if(adjData == NULL)
		return;
	
	if(systemSetData == NULL)
		return;
	
	adjData->parm = 0;
	
	if(systemSetData->crc != CalModbusCRC16Fun(systemSetData, SystemSetDataStructCrcSize, NULL))
	{
		adjData->parm = 1;
		return;
	}
	
	//�����Ƿ����
	for(i=0; i<MaxAdjustItemNum; i++)
	{
		if(CheckStrIsSame(systemSetData->adjustData[i].ItemName, adjData->ItemName, ItemNameLen) == true)
		{
			adjData->parm = systemSetData->adjustData[i].parm;
			break;
		}
	}
	
	if((i >= MaxAdjustItemNum) || (adjData->parm <= 0))
		adjData->parm = 1;
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
