/***************************************************************************************************
*FileName: ItemConst_Data
*Description: 各个测试品种的固定数据，不用写在二维码
*Author:xsx
*Data: 2017年2月21日11:19:58
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
		.itemCode = "NT-proBNP\0",
		.icoIndex = 0,
		.pointNum = 0,
		.lowstResult = 20,
		.highestResult = 30000,
		.normalResult = "<75岁,<125 pg/mL,>=75岁,<450 pg/mL\0",
		.itemMeasure = "pg/mL\0"
	},
	{
		.itemName = "CK-MB\0",
		.itemCode = "CK-MB\0",
		.icoIndex = 1,
		.pointNum = 2,
		.lowstResult = 0.5,
		.highestResult = 80,
		.normalResult = {"<=3.8 ng/mL\0"},
		.itemMeasure = {"ng/mL\0"}
	},
	{
		.itemName = "cTnI\0",
		.itemCode = "cTnI\0",
		.icoIndex = 2,
		.pointNum = 2,
		.lowstResult = 0.1,
		.highestResult = 30,
		.normalResult = {"<=0.3 ng/mL\0"},
		.itemMeasure = {"ng/mL\0"}
	},
	{
		.itemName = "Myo\0",
		.itemCode = "Myo\0",
		.icoIndex = 3,
		.pointNum = 2,
		.lowstResult = 2.5,
		.highestResult = 350,
		.normalResult = {"<=99.3 ng/mL\0"},
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

/****************************************end of file************************************************/
