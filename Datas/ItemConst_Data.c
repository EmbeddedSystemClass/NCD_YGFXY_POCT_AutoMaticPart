/***************************************************************************************************
*FileName: ItemConst_Data
*Description: ��������Ʒ�ֵĹ̶����ݣ�����д�ڶ�ά��
*Author:xsx
*Data: 2017��2��21��11:19:58
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"ItemConst_Data.h"

#include	"MyTools.h"

#include	<string.h>
#include	"stdio.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
const ItemConstData GB_ItemConstData[ItemConstDataNum]=
{
	{
		.itemName = "NT-proBNP\0",
		.itemCH = "N-����������ǰ��\0",
		.pointNum = 0,
		.lowstResult = 20,
		.highestResult = 30000,
		.normalResult = "<75��,<125 pg/mL,>=75��,<450 pg/mL\0",
		.itemMeasure = "pg/mL\0"
	},
	{
		.itemName = "CK-MB\0",
		.itemCH = "���ἤøͬ��ø\0",
		.pointNum = 2,
		.lowstResult = 0.5,
		.highestResult = 80,
		.normalResult = {"<=3.8 ng/mL\0"},
		.itemMeasure = {"ng/mL\0"}
	},
	{
		.itemName = "cTnI\0",
		.itemCH = "���Ƶ���I\0",
		.pointNum = 3,
		.lowstResult = 0.01,
		.highestResult = 30,
		.normalResult = {"<=0.3 ng/mL\0"},
		.itemMeasure = {"ng/mL\0"}
	},
	{
		.itemName = "Myo\0",
		.itemCH = "���쵰��\0",
		.pointNum = 2,
		.lowstResult = 2.5,
		.highestResult = 350,
		.normalResult = {"<=99.3 ng/mL\0"},
		.itemMeasure = {"ng/mL\0"}
	},
	{
		.itemName = "D-Dimer\0",
		.itemCH = "D-������\0",
		.pointNum = 1,
		.lowstResult = 0.2,
		.highestResult = 15,
		.normalResult = {"<=0.5 mg/L\0"},
		.itemMeasure = {"mg/L\0"}
	},
	{
		.itemName = "CRP\0",
		.itemCH = "ȫ��C-��Ӧ����\0",
		.pointNum = 2,
		.lowstResult = 0.44,
		.highestResult = 200,
		.normalResult = {"<=1.0 mg/L\0"},
		.itemMeasure = {"mg/L\0"}
	},
	{
		.itemName = "PCT\0",
		.itemCH = "������ԭ\0",
		.pointNum = 2,
		.lowstResult = 0.22,
		.highestResult = 100,
		.normalResult = {"<=0.5 ng/mL\0"},
		.itemMeasure = {"ng/mL\0"}
	},
	{
		.itemName = "CysC\0",
		.itemCH = "������C\0",
		.pointNum = 2,
		.lowstResult = 0.46,
		.highestResult = 8,
		.normalResult = {"0.5-1.3 mg/L\0"},
		.itemMeasure = {"mg/L\0"}
	},
	{
		.itemName = "��-HCG\0",
		.itemCH = "��-����ëĤ�����ټ���\0",
		.pointNum = 2,
		.lowstResult = 2,
		.highestResult = 25000,
		.normalResult = {"<=5 mIU/mL\0"},
		.itemMeasure = {"mIU/mL\0"}
	},
	{
		.itemName = "NGAL\0",
		.itemCH = "������ϸ������ø���֬�����ص���\0",
		.pointNum = 2,
		.lowstResult = 10,
		.highestResult = 1500,
		.normalResult = {"<=170 ng/mL\0"},
		.itemMeasure = {"ng/mL\0"}
	},
};
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

MyRes getItemConstData(ItemConstData * itemConstData, char * itemName)
{
	unsigned char i=0;
	
	if(itemConstData == NULL)
		return My_Fail;
	
	for(i=0; i<ItemConstDataNum; i++)
	{
		if(strcmp(GB_ItemConstData[i].itemName, itemName) == 0)
		{
			memcpy(itemConstData, &GB_ItemConstData[i], sizeof(ItemConstData));
			return My_Pass;
		}
	}
	
	return My_Fail;
}

MyRes getItemConstDataByIndex(ItemConstData * itemConstData, unsigned char index)
{	
	if(index >= ItemConstDataNum || itemConstData == NULL)
		return My_Fail;
	
	memcpy(itemConstData, &GB_ItemConstData[index], sizeof(ItemConstData));
	
	return My_Pass;
}

/****************************************end of file************************************************/
