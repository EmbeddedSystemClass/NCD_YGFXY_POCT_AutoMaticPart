/***************************************************************************************************
*FileName: Paidui_Fun
*Description: 排队功能
*Author: xsx_kair
*Data: 2016年12月13日11:47:20
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Paidui_Fun.h"

#include	"PaiDuiPage.h"
#include	"TimeDownNorPage.h"

#include	"PaiduiUnitData.h"
#include	"UI_Data.h"

#include	"MyTools.h"

#include	<string.h>
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

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
*FunctionName: PaiDuiHandler
*Description: 排队流程控制
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月13日11:58:58
***************************************************************************************************/
void PaiDuiHandler(void)
{
	unsigned char index = 0;
	PaiduiUnitData * temp = NULL;
	unsigned short tempvalue = 0;
	
	for(index=0; index<PaiDuiWeiNum; index++)
	{
		temp = GetTestItemByIndex(index);
		
		if(temp)
		{
			//进入排队模式
			if(temp->statues == status_start)
			{
				temp->statues = status_timedown;
			}
			
			//如果正在倒计时
			if((temp->statues == status_timedown) || (temp->statues == status_waitTest))
			{
				tempvalue = timer_surplus(&(temp->timeDown_timer));
				
				if(0 == tempvalue)
				{
					temp->statues = status_timeup;
					timer_restart(&(temp->timeUp_timer));				//启动超时计时器
				}
				else if(tempvalue <= 5)
				{
					if(temp == GetCurrentTestItem())
					{
						temp->statues = status_testting;
						startActivity(createTimeDownActivity, NULL, NULL);
					}
				}
				else if(tempvalue <= 10)
				{
					if(NULL == GetCurrentTestItem())
					{
						SetCurrentTestItem(temp);
						temp->statues = status_waitTest;
					}
				}
				//时间>30秒
				else
				{
					if(temp == GetCurrentTestItem())
						SetCurrentTestItem(NULL);
				}
			}
			//超时状态
			else if(temp->statues == status_timeup)
			{
				tempvalue = GetMinWaitTime();
				
				if((tempvalue > 30) && (GetCurrentTestItem() == NULL))
				{
					SetCurrentTestItem(temp);
					temp->statues = status_testting;
					startActivity(createTimeDownActivity, NULL, NULL);
				}
			}
		}
	}
}

/****************************************end of file************************************************/
