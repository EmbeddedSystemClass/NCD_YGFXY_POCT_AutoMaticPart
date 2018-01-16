/***************************************************************************************************
*FileName:ReTestPage
*Description:老化测试界面
*Author: xsx_kair
*Data:2016年8月15日17:36:56
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"ReTestPage.h"
#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"SystemSetPage.h"
#include	"CodeScan_Task.h"
#include	"Test_Task.h"
#include	"Timer.h"
#include	"Ads8325_Driver.h"
#include	"LEDCheck_Driver.h"
#include	"Motor_Fun.h"
#include	"System_Data.h"
#include	"SystemSet_Data.h"
#include	"CardCheck_Driver.h"
#include	"ReTestDataDao.h"
#include	"MyTools.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static ReTestPageBuffer * S_ReTestPageBuffer = NULL;
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyRes activityBufferMalloc(void);
static void activityBufferFree(void);

static void dspReTestStatus(char * str, unsigned char * parm1);
static void stopReTest(char * str);
static void OneReTestEnd(char * str);
static void dspReTestLogs(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: 创建选择操作人界面
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:09
***************************************************************************************************/
MyRes createReTestActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "ReTestActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	dspReTestStatus("Stopped", NULL);
	S_ReTestPageBuffer->isTestting = false;
	timer_SetAndStart(&(S_ReTestPageBuffer->testTimer), 10);
	
	SelectPage(143);
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
	S_ReTestPageBuffer->lcdinput[0] = pbuf[4];
	S_ReTestPageBuffer->lcdinput[0] = (S_ReTestPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
	/*退出*/
	if(S_ReTestPageBuffer->lcdinput[0] == 0x2802)
	{
		if(!S_ReTestPageBuffer->isTestting)
			backToFatherActivity();
		else
			SendKeyCode(1);
	}
	/*开始老化*/
	else if(S_ReTestPageBuffer->lcdinput[0] == 0x2800)
	{
		if(!S_ReTestPageBuffer->isTestting)
		{
			S_ReTestPageBuffer->isTestting = true;
			S_ReTestPageBuffer->waitCardIndex = 0;
			S_ReTestPageBuffer->testCardIndex = 0;
			S_ReTestPageBuffer->testCnt = 0;
			timer_restart(&S_ReTestPageBuffer->testTimer);
		}
	}
	/*停止老化*/
	else if(S_ReTestPageBuffer->lcdinput[0] == 0x2801)
	{
		S_ReTestPageBuffer->testCnt = S_ReTestPageBuffer->testTotalCnt;
	}
	/*获取老化次数*/
	else if(S_ReTestPageBuffer->lcdinput[0] == 0x2820)
	{
		if(!S_ReTestPageBuffer->isTestting)
		{
			memset(S_ReTestPageBuffer->buf, 0, 100);
			memcpy(S_ReTestPageBuffer->buf, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_ReTestPageBuffer->testTotalCnt = strtol(S_ReTestPageBuffer->buf, NULL, 10);
			if(S_ReTestPageBuffer->testTotalCnt > 60000)
				S_ReTestPageBuffer->testTotalCnt = 60000;
		}
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
/*	if(S_ReTestPageBuffer->isTestting)
	{
		//插卡
		if(S_ReTestPageBuffer->waitCardIndex <= PaiDuiWeiNum)
		{
			for(S_ReTestPageBuffer->i=0; S_ReTestPageBuffer->i<PaiDuiWeiNum ; S_ReTestPageBuffer->i++)
			{
				if(S_ReTestPageBuffer->waitCardIndex > 0)
					break;
				
				if(S_ReTestPageBuffer->qrCode[S_ReTestPageBuffer->i].CRC16 == 0)
				{
					S_ReTestPageBuffer->waitCardIndex = S_ReTestPageBuffer->i + 1;
					dspReTestStatus("ready", NULL);
					
					S_ReTestPageBuffer->motorStep = 1;
					MotorMoveToWaitCardPutIn(S_ReTestPageBuffer->waitCardIndex);
					break;
				}
			}
			
			if(S_ReTestPageBuffer->i >= PaiDuiWeiNum)
			{
				S_ReTestPageBuffer->waitCardIndex = PaiDuiWeiNum+1;
			}
			else
			{
				if(S_ReTestPageBuffer->motorStep == 1 && isMotorActionOver(S_ReTestPageBuffer->waitCardIndex, Motor2_WaitCardLocation, Motor4_OpenLocation))
				{
					S_ReTestPageBuffer->motorStep = 2;
					dspReTestStatus("put card", &S_ReTestPageBuffer->waitCardIndex);
				}
					
				if(S_ReTestPageBuffer->motorStep == 2 && readCaedCheckStatus() == ON)
				{
					S_ReTestPageBuffer->motorStep = 3;
					StartScanQRCode(&S_ReTestPageBuffer->qrCode[S_ReTestPageBuffer->waitCardIndex-1]);
				}
				
				if(S_ReTestPageBuffer->motorStep == 3 && My_Pass == TakeScanQRCodeResult(&S_ReTestPageBuffer->scancode))
				{
					if(S_ReTestPageBuffer->scancode == CardCodeScanOK)
					{
						S_ReTestPageBuffer->motorStep = 0;
						S_ReTestPageBuffer->waitCardIndex = 0;
					}
					else
					{
						dspReTestStatus("out card", &S_ReTestPageBuffer->waitCardIndex);
						S_ReTestPageBuffer->motorStep = 7;
					}
				}
				
				if((S_ReTestPageBuffer->motorStep == 7) && (Motor2_WaitCardLocation == getMotorxLocation(Motor_2)) && readCaedCheckStatus() == OFF)
				{
					S_ReTestPageBuffer->motorStep = 2;
					dspReTestStatus("re put card", &S_ReTestPageBuffer->waitCardIndex);
				}
			}
		}
		else
		{
			if(S_ReTestPageBuffer->testCnt < S_ReTestPageBuffer->testTotalCnt)
			{
				for(S_ReTestPageBuffer->i=0; S_ReTestPageBuffer->i<PaiDuiWeiNum; S_ReTestPageBuffer->i++)
				{
					if(S_ReTestPageBuffer->testCardIndex > 0)
						break;
					
					if(S_ReTestPageBuffer->result[S_ReTestPageBuffer->i] == 0)
					{
						S_ReTestPageBuffer->testCardIndex = S_ReTestPageBuffer->i + 1;
						dspReTestStatus("test card", &S_ReTestPageBuffer->testCardIndex);
						
						S_ReTestPageBuffer->testStep = 1;
						MotorMoveToWaitCardPutIn(S_ReTestPageBuffer->testCardIndex);
						break;
					}
				}
				
				if(S_ReTestPageBuffer->i >= PaiDuiWeiNum)
				{
					sprintf(S_ReTestPageBuffer->resultDesc, "Success\0");
					OneReTestEnd(S_ReTestPageBuffer->resultDesc);
				}
				
				if(S_ReTestPageBuffer->testStep == 1 && isMotorActionOver(S_ReTestPageBuffer->testCardIndex, Motor2_WaitCardLocation, Motor4_OpenLocation))
				{
					//check card is exist
					if(OFF == readCaedCheckStatus())
					{
						sprintf(S_ReTestPageBuffer->resultDesc, "%d not exist\0", S_ReTestPageBuffer->testCardIndex);
						OneReTestEnd(S_ReTestPageBuffer->resultDesc);

						MotorMoveToOriginLocation(S_ReTestPageBuffer->testCardIndex);
						S_ReTestPageBuffer->testStep = 21;
					}
					else
					{
						S_ReTestPageBuffer->testStep = 3;
						StartScanQRCode(&S_ReTestPageBuffer->testData.qrCode);
					}
				}
				
				if(S_ReTestPageBuffer->testStep == 3 && My_Pass == TakeScanQRCodeResult(&S_ReTestPageBuffer->scancode))
				{
					if(S_ReTestPageBuffer->scancode == CardCodeScanOK)
					{
						if(!CheckStrIsSame(&S_ReTestPageBuffer->testData.qrCode, &S_ReTestPageBuffer->qrCode[S_ReTestPageBuffer->testCardIndex-1], QRCodeStructSize))
						{
							sprintf(S_ReTestPageBuffer->resultDesc, "%d QR changed\0", S_ReTestPageBuffer->testCardIndex);
							OneReTestEnd(S_ReTestPageBuffer->resultDesc);
							
							S_ReTestPageBuffer->testStep = 21;
							MotorMoveToOriginLocation(S_ReTestPageBuffer->testCardIndex);
						}
						else
						{
							S_ReTestPageBuffer->testStep = 4;
							
							S_ReTestPageBuffer->testCardIndex2 = S_ReTestPageBuffer->testCardIndex;
							S_ReTestPageBuffer->testCardIndex2 += PaiDuiWeiNum;
							if(S_ReTestPageBuffer->testCardIndex2 > PaiDuiWeiNum*2)
								S_ReTestPageBuffer->testCardIndex2 -= PaiDuiWeiNum*2;
							
							MotorMoveToStartTestLocation(S_ReTestPageBuffer->testCardIndex2);
						}
					}
					else
					{
						sprintf(S_ReTestPageBuffer->resultDesc, "%d QR Fail\0", S_ReTestPageBuffer->testCardIndex);
						OneReTestEnd(S_ReTestPageBuffer->resultDesc);

						S_ReTestPageBuffer->testStep = 21;
						MotorMoveToOriginLocation(S_ReTestPageBuffer->testCardIndex);
					}
				}
				
				if((S_ReTestPageBuffer->testStep == 4) && isMotorActionOver(S_ReTestPageBuffer->testCardIndex2, Motor2_StartTestLocation, Motor4_CardLocation))
				{
					S_ReTestPageBuffer->testStep = 5;
					StartTest(&S_ReTestPageBuffer->testData);
				}

				if(S_ReTestPageBuffer->testStep == 5 && My_Pass == TakeTestResult(&S_ReTestPageBuffer->testData.testResultDesc))
				{
					S_ReTestPageBuffer->result[S_ReTestPageBuffer->testCardIndex-1] = S_ReTestPageBuffer->testCardIndex;//S_ReTestPageBuffer->testData.testSeries.result;

					StartMotorAction(Motor_2, Motor2_PutDownCardLocation2, false);
					S_ReTestPageBuffer->testStep = 6;
				}
				
				if(S_ReTestPageBuffer->testStep == 6 && Motor2_PutDownCardLocation2 == getMotorxLocation(Motor_2))
				{
					StartMotorAction(Motor_4, Motor4_OpenLocation, false);
					StartMotorAction(Motor_2, Motor2_MidLocation, false);
					S_ReTestPageBuffer->testStep = 7;
				}
				
				if(S_ReTestPageBuffer->testStep == 7 && Motor2_MidLocation == getMotorxLocation(Motor_2))
				{
					S_ReTestPageBuffer->testCardIndex = 0;
				}

				if(S_ReTestPageBuffer->testStep == 21 && isMotorActionOver(S_ReTestPageBuffer->testCardIndex, Motor2_MidLocation, Motor4_OpenLocation))
				{
					stopReTest(S_ReTestPageBuffer->resultDesc);
				}
			}
			else
				S_ReTestPageBuffer->isTestting = false;
		}
		
		if(S_ReTestPageBuffer->count % 10 == 0)
			dspReTestLogs();
		
		S_ReTestPageBuffer->count++;
	}*/
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
	if(NULL == S_ReTestPageBuffer)
	{
		S_ReTestPageBuffer = MyMalloc(sizeof(ReTestPageBuffer));
		
		if(S_ReTestPageBuffer)
		{
			memset(S_ReTestPageBuffer, 0, sizeof(ReTestPageBuffer));
	
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
	MyFree(S_ReTestPageBuffer);
	S_ReTestPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

static void stopReTest(char * str)
{
	S_ReTestPageBuffer->testCnt = S_ReTestPageBuffer->testTotalCnt;
	
	dspReTestStatus(str, NULL);
}

static void OneReTestEnd(char * str)
{
	//读取时间
	getSystemTime(&S_ReTestPageBuffer->dateTime);
	
	sprintf(S_ReTestPageBuffer->resultDesc, "%s", str);
	
	if(My_Pass == writeReTestDataToFile(S_ReTestPageBuffer))
		S_ReTestPageBuffer->testCnt++;
	else
		stopReTest("数据保存错误\0");
	
	memset(S_ReTestPageBuffer->result, 0, 8*sizeof(float));
}


static void dspReTestStatus(char * str, unsigned char * parm1)
{
	if(parm1)
		snprintf(S_ReTestPageBuffer->buf, 20, "%s-%d", str, *parm1);
	else
		snprintf(S_ReTestPageBuffer->buf, 20, "%s", str);
	DisText(0x2828, S_ReTestPageBuffer->buf, strlen(S_ReTestPageBuffer->buf)+1);
}

static void dspReTestLogs(void)
{
	//已测
	DspNum(0x2810 , S_ReTestPageBuffer->testCnt, 4);
	//已测时间
	DspNum(0x281a , timer_Count(&S_ReTestPageBuffer->testTimer), 4);
}
/****************************************end of file************************************************/
