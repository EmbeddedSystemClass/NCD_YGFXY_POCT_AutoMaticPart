/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"UserMPage.h"
#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"SystemSetPage.h"
#include	"System_Data.h"
#include	"OperatorDao.h"
#include	"MyTools.h"
#include	"SleepPage.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static UserMPageBuffer * page = NULL;

/******************************************************************************************/
/*****************************************局部函数声明*************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyRes activityBufferMalloc(void);
static void activityBufferFree(void);

static void showCurrentPageList(void);
static void showCurrentOperatorInfo(void);
static void deleteCurrentOperator(void);
static MyRes addOrModifyOperator(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: 创建选择操作人界面
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:09
***************************************************************************************************/
MyRes createUserManagerActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "UserManagerActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: 显示主界面
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:32
***************************************************************************************************/
static void activityStart(void)
{
	updateSystemWorkStatus(SystemOperatorModifying, ON);
	
	ReadAllOperatorFromFile(page->operatorList, &page->allValidNum);
			
	page->pageIndex = 0;

	showCurrentPageList();
	
	SelectPage(106);
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: 界面输入
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	/*命令*/
	page->lcdinput[0] = pbuf[4];
	page->lcdinput[0] = (page->lcdinput[0]<<8) + pbuf[5];
		
	/*返回*/
	if(page->lcdinput[0] == 0x1d00)
	{
		backToFatherActivity();
	}
		
	/*上翻也*/
	else if(page->lcdinput[0] == 0x1d03)
	{
		if(page->pageIndex > 0)
		{
			page->pageIndex--;

			showCurrentPageList();
		}
	}
	/*下翻页*/
	else if(page->lcdinput[0] == 0x1d04)
	{
		page->tempV1 = (page->pageIndex + 1) * MaxPageShowOperatorSize;
		if(page->tempV1 < page->allValidNum)
		{
			page->pageIndex++;

			showCurrentPageList();
		}
	}
	/*选择操作人*/
	else if((page->lcdinput[0] >= 0x1d07)&&(page->lcdinput[0] <= 0x1d0B))
	{
		page->tempV1 = page->pageIndex * MaxPageShowOperatorSize + page->lcdinput[0] - 0x1d07;
		page->tempOperator = &page->operatorList[page->tempV1];
		
		if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
		{
			page->selectIndex = page->lcdinput[0] - 0x1d07;
			BasicPic(0x1d40, 1, 137, 11, 10, 303, 79, 157, 135+page->selectIndex*72);
			
			memcpy(&page->operatorDetail, page->tempOperator, OneOperatorStructSize);
			
			showCurrentOperatorInfo();
		}	
	}
	/*删除*/
	else if(page->lcdinput[0] == 0x1d01)
	{
		deleteCurrentOperator();
	}
	//修改或者添加
	else if(page->lcdinput[0] == 0x1d02)
	{
		if(strlen(page->operatorDetail.name) > 0)
		{
			if(My_Pass == addOrModifyOperator())
				SendKeyCode(1);
			else
				SendKeyCode(2);
		}
		else
			SendKeyCode(2);
	}
	//姓名
	else if(page->lcdinput[0] == 0x1d50)
	{
		getLcdInputData(page->operatorDetail.name, &pbuf[7]);
	}
	//年龄
	else if(page->lcdinput[0] == 0x1d60)
	{
		getLcdInputData(page->operatorDetail.age, &pbuf[7]);
	}
	//性别
	else if(page->lcdinput[0] == 0x1d70)
	{
		getLcdInputData(page->operatorDetail.sex, &pbuf[7]);
	}
	//联系方式
	else if(page->lcdinput[0] == 0x1d80)
	{
		getLcdInputData(page->operatorDetail.phone, &pbuf[7]);
	}
	//职位
	else if(page->lcdinput[0] == 0x1d90)
	{
		getLcdInputData(page->operatorDetail.job, &pbuf[7]);
	}
	//备注
	else if(page->lcdinput[0] == 0x1da0)
	{
		getLcdInputData(page->operatorDetail.department, &pbuf[7]);
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: 界面刷新
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:01:16
***************************************************************************************************/
static void activityFresh(void)
{

}

/***************************************************************************************************
*FunctionName: activityHide
*Description: 隐藏界面时要做的事
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:01:40
***************************************************************************************************/
static void activityHide(void)
{

}

/***************************************************************************************************
*FunctionName: activityResume
*Description: 界面恢复显示时要做的事
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:01:58
***************************************************************************************************/
static void activityResume(void)
{
	SelectPage(106);
}

/***************************************************************************************************
*FunctionName: activityDestroy
*Description: 界面销毁
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:02:15
***************************************************************************************************/
static void activityDestroy(void)
{
	updateSystemWorkStatus(SystemOperatorModifying, OFF);
	activityBufferFree();
}

/***************************************************************************************************
*FunctionName: activityBufferMalloc
*Description: 界面数据内存申请
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static MyRes activityBufferMalloc(void)
{
	if(NULL == page)
	{
		page = MyMalloc(sizeof(UserMPageBuffer));
		
		if(page)
		{
			memset(page, 0, sizeof(UserMPageBuffer));
	
			return My_Pass;
		}
		else
			return My_Fail;
	}
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: activityBufferFree
*Description: 界面内存释放
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:03:10
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(page);
	page = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName:  showCurrentPageList
*Description:  显示当前页操作人列表
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017年7月6日 14:16:50
***************************************************************************************************/
static void showCurrentPageList(void)
{
	page->tempV1 = page->pageIndex*MaxPageShowOperatorSize;
	
	page->tempOperator = &page->operatorList[page->tempV1];
	
	/*显示列表数据*/
	page->pageValidNum = 0;
	for(page->tempV1=0; page->tempV1<MaxPageShowOperatorSize; page->tempV1++)
	{
		if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
		{
			snprintf(page->buf, OperatorNameLen, "%s", page->tempOperator->name);
			page->pageValidNum++;
		}
		else
			memset(page->buf, 0, 10);
		
		DisText(0x1d10+8*page->tempV1, page->buf, strlen(page->buf)+1);
		
		page->tempOperator++;
	}
	
	page->selectIndex = -1;
	BasicPic(0x1d40, 0, 137, 11, 10, 303, 79, 157, 135);
	
	memset(&page->operatorDetail, 0, OneOperatorStructSize);
	
	showCurrentOperatorInfo();
}

/***************************************************************************************************
*FunctionName:  showCurrentOperatorInfo
*Description:  显示当前操作人信息
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017年7月6日 14:19:13
***************************************************************************************************/
static void showCurrentOperatorInfo(void)
{
	if(page->operatorDetail.crc == CalModbusCRC16Fun(&page->operatorDetail, OneOperatorStructCrcSize, NULL))
	{
		sprintf(page->buf, "%s", page->operatorDetail.name);
		DisText(0x1d50, page->buf, strlen(page->buf)+1);
			
		sprintf(page->buf, "%s", page->operatorDetail.age);
		DisText(0x1d60, page->buf, strlen(page->buf)+1);
			
		sprintf(page->buf, "%s", page->operatorDetail.sex);
		DisText(0x1d70, page->buf, strlen(page->buf)+1);
			
		sprintf(page->buf, "%s", page->operatorDetail.phone);
		DisText(0x1d80, page->buf, strlen(page->buf)+1);
			
		sprintf(page->buf, "%s", page->operatorDetail.job);
		DisText(0x1d90, page->buf, strlen(page->buf)+1);
			
		sprintf(page->buf, "%s", page->operatorDetail.department);
		DisText(0x1da0, page->buf, strlen(page->buf)+1);
	}
	else
	{
		ClearText(0x1d50);
			
		ClearText(0x1d60);
				
		ClearText(0x1d70);
				
		ClearText(0x1d80);
				
		ClearText(0x1d90);
				
		ClearText(0x1da0);
	}
}

/***************************************************************************************************
*FunctionName:  deleteCurrentOperator
*Description:  删除当前操作人
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017年7月6日 15:25:44
***************************************************************************************************/
static void deleteCurrentOperator(void)
{
	if(page->selectIndex >= 0 && page->selectIndex < MaxPageShowOperatorSize)
	{
		page->tempV1 = page->pageIndex * MaxPageShowOperatorSize + page->selectIndex;
		page->tempV2 = MaxOperatorSize - page->tempV1 - 1;
		page->tempV2 *= OneOperatorStructSize;
		memcpy(&page->operatorList[page->tempV1], &page->operatorList[page->tempV1+1], page->tempV2);
		page->operatorList[MaxOperatorSize-1].crc = 0;
		
		SaveAllOperatorToFile(page->operatorList);
		ReadAllOperatorFromFile(page->operatorList, &page->allValidNum);
		
		page->pageIndex = 0;

		showCurrentPageList();
	}
}

/***************************************************************************************************
*FunctionName:  addOrModifyOperator
*Description:  添加或者修改操作人
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date: 2017年7月7日 16:07:17
***************************************************************************************************/
static MyRes addOrModifyOperator(void)
{
	page->operatorDetail.crc = CalModbusCRC16Fun(&page->operatorDetail, OneOperatorStructCrcSize, NULL);
	
	//查找是否存在名字一样的人
	page->tempV2 = MaxOperatorSize;
	page->tempOperator = &page->operatorList[0];
	for(page->i=0; page->i<MaxOperatorSize; page->i++)
	{
		if(page->tempOperator->crc == CalModbusCRC16Fun(page->tempOperator, OneOperatorStructCrcSize, NULL))
		{
			if(CheckStrIsSame(page->operatorDetail.name, page->tempOperator->name, OperatorNameLen))
				break;
		}
		else
			break;
		
		page->tempOperator++;
	}
	
	//如果找不到一样的名字，且没有空位
	if(page->i >= MaxOperatorSize)
		return My_Fail;
	else
		memcpy(&page->operatorList[page->i], &page->operatorDetail, OneOperatorStructSize);
	
	if(My_Pass == SaveAllOperatorToFile(page->operatorList))
	{
		ReadAllOperatorFromFile(page->operatorList, &page->allValidNum);
		page->pageIndex = 0;
		showCurrentPageList();
		return My_Pass;
	}
	else
	{
		ReadAllOperatorFromFile(page->operatorList, &page->allValidNum);
		page->pageIndex = 0;
		showCurrentPageList();
		return My_Fail;
	}
		
	
}

