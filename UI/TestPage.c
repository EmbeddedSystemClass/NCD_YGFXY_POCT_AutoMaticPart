/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"TestPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"

#include	"MyMem.h"
#include	"WaittingCardPage.h"
#include	"MyTest_Data.h"
#include	"PaiDuiPage.h"
#include	"CardCheck_Driver.h"
#include	"StringDefine.h"
#include	"System_Data.h"
#include	"TestDataDao.h"
#include	"SystemSet_Dao.h"
#include	"Test_Task.h"
#include	"Printf_Fun.h"
#include	"Motor_Fun.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static TestPageBuffer * S_TestPageBuffer = NULL;
const unsigned int TestLineHigh = 76500;	//此数据与曲线显示区域高度有关，如果界面不改，此数不改
/******************************************************************************************/
/*****************************************局部函数声明*************************************/
static void InitCurve(void);
static void RefreshCurve(void);
static void AddDataToLine(unsigned short data);
static void InitPageText(void);
static void RefreshPageText(void);

static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyRes activityBufferMalloc(void);
static void activityBufferFree(void);
static void printfTestData(void);
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
MyRes createTestActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "TestActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	/*获取当前测试数据的地址*/
	S_TestPageBuffer->currenttestdata = GetCurrentTestItem();
	S_TestPageBuffer->canExit = false;
	readGbSystemSetData(&S_TestPageBuffer->systemSetData);
	InitCurve();
		
	//必须在获取当前测试数据地址后使用
	InitPageText();
		
	StartTest(S_TestPageBuffer->currenttestdata);
	
	SelectPage(96);
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
	S_TestPageBuffer->lcdinput[0] = pbuf[4];
	S_TestPageBuffer->lcdinput[0] = (S_TestPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
	/*退出*/
	if(0x1801 == S_TestPageBuffer->lcdinput[0])
	{
		if(S_TestPageBuffer->canExit)
		{							
			backToActivity(lunchActivityName);
					
			if(IsPaiDuiTestting())
				startActivity(createPaiDuiActivity, NULL, NULL);
		}
		//正在测试不允许退出
		else
			SendKeyCode(4);
	}
	/*打印数据*/
	else if(0x1800 == S_TestPageBuffer->lcdinput[0])
		printfTestData();
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
	if(S_TestPageBuffer->currenttestdata->testData.testResultDesc == NoResult)
		RefreshCurve();
	else if(isMotorMoveEnd(0 / portTICK_RATE_MS))
	{
        if(S_TestPageBuffer->currenttestdata->testData.testResultDesc == RE_TEST)
        {
            S_TestPageBuffer->currenttestdata->statues = status_timedown;
            S_TestPageBuffer->currenttestdata->testData.testResultDesc = NoResult;
        }
        else
            DeleteCurrentTest();
        
        S_TestPageBuffer->canExit = true;
		S_TestPageBuffer->currenttestdata = NULL;
	}
	
	if(S_TestPageBuffer->canExit && IsPaiDuiTestting())
	{
		backToActivity(lunchActivityName);

		startActivity(createPaiDuiActivity, NULL, NULL);
	}
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
	//清除当前页面的告警弹出框
	SendKeyCode(16);
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
	//清除当前页面的告警弹出框
	SendKeyCode(16);
	
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
	if(NULL == S_TestPageBuffer)
	{
		S_TestPageBuffer = MyMalloc(sizeof(TestPageBuffer));
		
		if(S_TestPageBuffer)
		{
			memset(S_TestPageBuffer, 0, sizeof(TestPageBuffer));
	
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
	MyFree(S_TestPageBuffer);
	S_TestPageBuffer = NULL;
}


static void InitCurve(void)
{
	ClearLine(0x56);
				
	//初始化测试曲线
	S_TestPageBuffer->line.MaxData = 0;
	S_TestPageBuffer->line.MUL_Y = 1;
	S_TestPageBuffer->line.Y_Scale = 100;
				
	SetChartSize(0x1870 , S_TestPageBuffer->line.MUL_Y);
				
	DspNum(0x180B , S_TestPageBuffer->line.Y_Scale, 2);
	DspNum(0x180A , S_TestPageBuffer->line.Y_Scale*2, 2);
	
	BasicUI(0x1880 ,0x1807 , 0, &(S_TestPageBuffer->myico[0]) , sizeof(Basic_ICO)*3);
}

static void InitPageText(void)
{
	sprintf(S_TestPageBuffer->buf, "%s", S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.itemName);
	DisText(0x1810, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
			
	memcpy(S_TestPageBuffer->buf, S_TestPageBuffer->currenttestdata->testData.sampleid, MaxSampleIDLen);
	DisText(0x1820, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
			
	sprintf(S_TestPageBuffer->buf, "%s", S_TestPageBuffer->currenttestdata->testData.operator.name);
	DisText(0x1830, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
			
	sprintf(S_TestPageBuffer->buf, "%s-%s", S_TestPageBuffer->currenttestdata->testData.qrCode.PiHao, 
		S_TestPageBuffer->currenttestdata->testData.qrCode.piNum);
	DisText(0x1840, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
			
	sprintf(S_TestPageBuffer->buf, "%s", S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.normalResult);
	DisText(0x1850, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);

	ClearText(0x1838);
}

/*更新数据*/
static void RefreshCurve(void)
{
	unsigned short temp;
	
	while(pdTRUE == TakeTestPointData(&temp))
	{
		if(0xffff == temp)
			InitCurve();
		else
			AddDataToLine(temp);
	}
	 
	if(My_Pass == TakeTestResult(&(S_TestPageBuffer->currenttestdata->testData.testResultDesc)))
	{
        if(S_TestPageBuffer->currenttestdata->testData.testResultDesc != ResultIsOK 
            && S_TestPageBuffer->currenttestdata->testData.testResultDesc != NoSample
            && S_TestPageBuffer->currenttestdata->testData.testcnt < MAX_TEST_CNT)
        {
            S_TestPageBuffer->currenttestdata->testData.testcnt++;
            S_TestPageBuffer->currenttestdata->testData.testResultDesc = RE_TEST;
            timer_SetAndStart(&S_TestPageBuffer->currenttestdata->timeDown_timer, 600);
            S_TestPageBuffer->currenttestdata->timeUp_timer.start = 0;

            FormatParmAndStartMotorAction(&S_TestPageBuffer->motorAction, PutDownCardInTestPlaceDef, S_TestPageBuffer->currenttestdata->testLocation, false);
            return;
        }
        
		S_TestPageBuffer->currenttestdata->statues = status_end;
		S_TestPageBuffer->motorAction.motorActionEnum = PutCardOutOfDeviceAfterTestDef;
		S_TestPageBuffer->motorAction.motorParm = S_TestPageBuffer->currenttestdata->testLocation;
		StartMotorAction(&S_TestPageBuffer->motorAction, false);
		
		memcpy(&(S_TestPageBuffer->currenttestdata->testData.testDateTime), &(getSystemRunTimeData()->systemDateTime), sizeof(DateTime));

		//保留一份数据给打印机打印
		memcpy(&(S_TestPageBuffer->testDataForPrintf), &(S_TestPageBuffer->currenttestdata->testData), sizeof(TestData));

		//保存数据,并且更新数据数目头信息
		writeTestDataToFile(&(S_TestPageBuffer->currenttestdata->testData));
		RefreshPageText();
		if(S_TestPageBuffer->currenttestdata->testData.testResultDesc == ResultIsOK)
		{
			if(S_TestPageBuffer->systemSetData.isAutoPrint == true)
				PrintfData(&(S_TestPageBuffer->testDataForPrintf));
		}
		else if(S_TestPageBuffer->currenttestdata->testData.testResultDesc == PeakError)
		{
			/*测试失败*/
			SendKeyCode(2);
		}
		else
		{
			SendKeyCode(1);		
		}
	}
}

static void RefreshPageText(void)
{
	
	
	if(S_TestPageBuffer->currenttestdata)
	{
		if(S_TestPageBuffer->currenttestdata->testData.testResultDesc != ResultIsOK)
			sprintf(S_TestPageBuffer->buf, "Error");
		else if(S_TestPageBuffer->currenttestdata->testData.testSeries.result <= S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.lowstResult)
			sprintf(S_TestPageBuffer->buf, "<%.*f %s", S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.pointNum, 
				S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.lowstResult, S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.itemMeasure);
        else if(S_TestPageBuffer->currenttestdata->testData.testSeries.result >= S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.highestResult)
			sprintf(S_TestPageBuffer->buf, ">%.*f %s", S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.pointNum, 
				S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.highestResult, S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.itemMeasure);
		else
			sprintf(S_TestPageBuffer->buf, "%.*f %s", S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.pointNum,
				S_TestPageBuffer->currenttestdata->testData.testSeries.result, S_TestPageBuffer->currenttestdata->testData.qrCode.itemConstData.itemMeasure);
		
		DisText(0x1838, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
	}
	
	if(S_TestPageBuffer->currenttestdata->testData.testResultDesc == ResultIsOK)
	{
		#if (DeviceUseType == Device_Final)
        double tempvalue = 0.0;
        //在曲线上标记出T,C,基线
		S_TestPageBuffer->myico[0].ICO_ID = 22;
		S_TestPageBuffer->myico[0].X = 574+S_TestPageBuffer->currenttestdata->testData.testSeries.T_Point.x-12;
		tempvalue = S_TestPageBuffer->currenttestdata->testData.testSeries.T_Point.y;
		tempvalue /= S_TestPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//曲线窗口宽度
		tempvalue += 139;										//曲线窗口起始y
		S_TestPageBuffer->myico[0].Y = (unsigned short)tempvalue - 11;
		
		S_TestPageBuffer->myico[1].ICO_ID = 22;
		S_TestPageBuffer->myico[1].X = 574+S_TestPageBuffer->currenttestdata->testData.testSeries.C_Point.x-12;
		tempvalue = S_TestPageBuffer->currenttestdata->testData.testSeries.C_Point.y;
		tempvalue /= S_TestPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//曲线窗口宽度
		tempvalue += 139;										//曲线窗口起始y
		S_TestPageBuffer->myico[1].Y = (unsigned short)tempvalue - 11;
		
		S_TestPageBuffer->myico[2].ICO_ID = 22;
		S_TestPageBuffer->myico[2].X = 574+S_TestPageBuffer->currenttestdata->testData.testSeries.B_Point.x-12;
		tempvalue = S_TestPageBuffer->currenttestdata->testData.testSeries.B_Point.y;
		tempvalue /= S_TestPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//曲线窗口宽度
		tempvalue += 139;										//曲线窗口起始y
		S_TestPageBuffer->myico[2].Y = (unsigned short)tempvalue - 11;
		
		BasicUI(0x1880 ,0x1807 , 3, &(S_TestPageBuffer->myico[0]) , sizeof(Basic_ICO)*3);
        #endif
	}
	else
	{
		BasicUI(0x1880 ,0x1807 , 0, &S_TestPageBuffer->myico[0] , sizeof(Basic_ICO)*3);
	}
}


static void AddDataToLine(unsigned short data)
{
	unsigned short tempdata = data;
	double TempMul_Y2;
	double TempY_Scale;
	unsigned short tempvalue;
	
	if(S_TestPageBuffer->line.MaxData <= tempdata)
	{
		S_TestPageBuffer->line.MaxData = tempdata;

		////////////////////////针对当前曲线最大值计算y轴放大倍数//////////////////////////////////////
		TempMul_Y2 = TestLineHigh;
		TempMul_Y2 /= S_TestPageBuffer->line.MaxData;
		TempMul_Y2 *= 0.8;			//*0.8是将最大值缩放到满刻度的0.8高度处

		
		tempvalue = (unsigned short)(TempMul_Y2*10);
		S_TestPageBuffer->line.MUL_Y = ((tempvalue%10) > 5)?(tempvalue/10 + 1):(tempvalue/10);
		
		if(S_TestPageBuffer->line.MUL_Y < 1)			//最小值为1
			S_TestPageBuffer->line.MUL_Y = 1;

		/////////////////////////针对当前放大倍数，计算y轴刻度递增基数/////////////////////////////////////
		TempY_Scale = TestLineHigh ;
		TempY_Scale /= S_TestPageBuffer->line.MUL_Y;
		TempY_Scale /= 2.0;																//目前显示2个y轴刻度
		S_TestPageBuffer->line.Y_Scale = (unsigned short)TempY_Scale;
		
		SetChartSize(0x1870 , S_TestPageBuffer->line.MUL_Y);

		DspNum(0x180B , S_TestPageBuffer->line.Y_Scale, 2);
		DspNum(0x180A , S_TestPageBuffer->line.Y_Scale*2, 2);
	}
	DisPlayLine(0 , &tempdata , 1);
}

/***************************************************************************************************
*FunctionName: printfTestData
*Description: 打印测试数据
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年1月19日16:39:51
***************************************************************************************************/
static void printfTestData(void)
{
	if(S_TestPageBuffer->testDataForPrintf.testResultDesc != NoResult)
		PrintfData(&(S_TestPageBuffer->testDataForPrintf));
	else
		SendKeyCode(4);
}

