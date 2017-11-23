/***************************************************************************************************
*FileName: Paidui_Fun
*Description: �Ŷӹ���
*Author: xsx_kair
*Data: 2016��12��13��11:47:20
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Paidui_Fun.h"

#include	"PaiDuiPage.h"
#include	"Motor_Fun.h"
#include	"PaiduiUnitData.h"
#include	"UI_Data.h"
#include	"StringDefine.h"

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
*Description: �Ŷ����̿���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��13��11:58:58
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
			//�����Ŷ�ģʽ
			if(temp->statues == status_start)
			{
				temp->statues = status_timedown;
			}
			
			//������ڵ���ʱ
			if(temp->statues == status_timedown)
			{
				tempvalue = timer_surplus(&(temp->timeDown_timer));
				
				if(0 == tempvalue)
				{
					temp->statues = status_timeup;
					timer_restart(&(temp->timeUp_timer));				//������ʱ��ʱ��
				}
				else if(tempvalue <= StartTestTime)
				{
					if(NULL == GetCurrentTestItem() || temp == GetCurrentTestItem())
					{
						SetCurrentTestItem(temp);
						temp->statues = statusPrepareTest;
						
						if(false == CheckStrIsSame(paiduiActivityName, getCurrentActivityName(), strlen(paiduiActivityName)))
						{
							backToActivity(lunchActivityName);
							startActivity(createPaiDuiActivity, NULL, NULL);
						}
					}
				}
				//ʱ��>30��
				else
				{
					if(temp == GetCurrentTestItem())
						SetCurrentTestItem(NULL);
				}
			}
			//��ʱ״̬
			else if(temp->statues == status_timeup)
			{
				tempvalue = GetMinWaitTime();
				
				if((tempvalue > DisableCreateNewTestTime) && (GetCurrentTestItem() == NULL))
				{
					SetCurrentTestItem(temp);
					temp->statues = statusPrepareTest;
				}
			}
		}
	}
}

/****************************************end of file************************************************/
