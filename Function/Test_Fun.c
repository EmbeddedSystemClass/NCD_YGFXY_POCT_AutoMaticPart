/***************************************************************************************************
*FileName:TestFun
*Description:??
*Author:xsx
*Data:2016?5?14?17:09:53
***************************************************************************************************/

/***************************************************************************************************/
/******************************************???***************************************************/
/***************************************************************************************************/
#include	"Test_Fun.h"

#include	"DA_Driver.h"
#include	"Channel_Driver.h"
#include	"Ads8325_Driver.h"
#include	"MyTest_Data.h"
#include	"CRC16.h"
#include	"System_Data.h"
#include	"SystemSet_Data.h"
#include	"Motor1_Fun.h"
#include	"Motor2_Fun.h"
#include	"Motor4_Fun.h"
#include	"MyMem.h"
#include    "Timer.h"
#include	"MyTools.h"
#include	"QueueUnits.h"
#include	"Delay.h"

#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include	"stdlib.h"
#include	"math.h"
/***************************************************************************************************/
/**************************************??????*************************************************/
/***************************************************************************************************/
static xQueueHandle xTestCurveQueue = NULL;												//发送测试曲线
/***************************************************************************************************/
/**************************************??????*************************************************/
/***************************************************************************************************/
static MyRes SendTestPointData(void * data);
static void AnalysisTestData(TempCalData * S_TempCalData);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************??********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: InitTestFunData
*Description: 创建测试曲线的队列
*Input: None
*Output: None
*Author: xsx
*Date: 2016年11月29日09:14:43
***************************************************************************************************/
MyRes InitTestFunData(void)
{
	/*曲线队列*/
	if(xTestCurveQueue == NULL)
		xTestCurveQueue = xQueueCreate( 310, ( unsigned portBASE_TYPE ) sizeof( unsigned short ) );
	
	if(xTestCurveQueue == NULL)
		return My_Fail;
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: SendTestPointData
*Description: 发送测试曲线点到队列中，曲线点包含y坐标信息
*Input: data -- 数据地址
*Output: None
*Return: 发送成功返回my_pass
*			发送超时，或者队列满，返回my_fail
*Author: xsx
*Date: 2016年11月29日09:18:33
***************************************************************************************************/
static MyRes SendTestPointData(void * data)
{
	if(xTestCurveQueue == NULL)
		return My_Fail;
	
	if(pdPASS == xQueueSend( xTestCurveQueue, data, 0/portTICK_RATE_MS ))
		return My_Pass;
	else
		return My_Fail;	
}

/***************************************************************************************************
*FunctionName: TakeTestPointData
*Description: 从测试曲线队列中读取数据
*Input: 读取数据缓存地址
*Output: None
*Return: 	My_Pass -- 成功读取到数据
			My_Fail -- 读取失败
*Author: xsx
*Date: 
***************************************************************************************************/
MyRes TakeTestPointData(void * data)
{
	if(xTestCurveQueue == NULL)
		return My_Fail;
	
	if(pdPASS == xQueueReceive( xTestCurveQueue, data, 5/portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;	
}

/***************************************************************************************************
*FunctionName: 
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
ResultState TestFunction(PaiduiUnitData * parm)
{
	unsigned short i = 0, j;
	unsigned short index;
	TempCalData * S_TempCalData = NULL;															//测试过程中使用的变量
	ResultState S_ResultState = NoResult;
	
	if(parm == NULL)
		return MemError;
	
	/*初始化测试曲线队列*/
	if(InitTestFunData() == My_Fail)
		return MemError;

	xQueueReset(xTestCurveQueue);
	
	S_TempCalData = MyMalloc(sizeof(TempCalData));
	
	if(S_TempCalData)
	{
		memset(S_TempCalData, 0, sizeof(TempCalData));
		//测试数据指针指向传进来的真实数据空间
		S_TempCalData->paiduiUnitData = parm;
		S_TempCalData->motor = getMotor(Motor_2);
		//初始配置
		SetLedVol(S_TempCalData->paiduiUnitData->ledValue);
		vTaskDelay(10/portTICK_RATE_MS);
		
		SelectChannel(S_TempCalData->paiduiUnitData->testData.qrCode.ChannelNum);
		vTaskDelay(10/portTICK_RATE_MS);
		
		repeat:

			S_TempCalData->resultstatues = NoResult;

			motor2MoveTo(2, 3, Motor2_StartTestLocation, true);
			
			motor2MoveTo(14, 23, Motor2_EndTestLocation, false);
		
			S_TempCalData->tempvalue1 = 0;
			for(i=1; i<= TestStep; i++)
			{
				vTaskDelay(3 / portTICK_RATE_MS);				
				S_TempCalData->tempvalue1 += ADS8325();
				
				//平均值滤波
				if(i%AvregeNum == 0)
				{
					index = i/AvregeNum;
					
					S_TempCalData->tempvalue1 /= AvregeNum;
					
					S_TempCalData->tempSeries[index-1] = (unsigned short)(S_TempCalData->tempvalue1);

					if(index >= FilterNum)
					{
						S_TempCalData->tempvalue2 = 0;
						for(j=index-FilterNum; j<index; j++)
						{
							S_TempCalData->tempvalue2 += S_TempCalData->tempSeries[j];
						}
						S_TempCalData->tempvalue2 /= FilterNum;
						
						S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[index - FilterNum] = S_TempCalData->tempvalue2;
						SendTestPointData(&S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[index - FilterNum]);
					}
						
					S_TempCalData->tempvalue1 = 0;
				}
			}
	
			//分析曲线
			AnalysisTestData(S_TempCalData);
			
			if(S_TempCalData->resultstatues == NoResult)
			{
				//发送一个特定数据，清除曲线
				S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[0] = 0xffff;
				SendTestPointData(&(S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[0]));
				goto repeat;
			}

			S_ResultState = S_TempCalData->resultstatues;
			
			MyFree(S_TempCalData);
			
			SetLedVol(0);
			vTaskDelay(1000 / portTICK_RATE_MS);

			return S_ResultState;
	}
	else
		return MemError;	
}


static void AnalysisTestData(TempCalData * S_TempCalData)
{
	unsigned short i=0;
	
    //用于检验中心，根据样品编号产生随机结果
    #if (DeviceUseType == Device_TestCenter)
    
        int group_value = 0;
        int a=0;
        int b = 0;
        double tempresult = 0;
        
        srand(getClockValue());
        a = pow(-1, ((rand()%2)+1));
    
        while(b <= 0 || b > 100000l)
        {
            b = (rand()%100001l);
            i++;
            if(i>50)
            {
                b = 50000;
                break;
            }
        }
        
        tempresult = b;
        tempresult *= 0.000001;
        
        switch(S_TempCalData->paiduiUnitData->testData.sampleid[0])
        {
            case '1': group_value = 5000; break;
            case '2': group_value = 15000; break;
            case '3': group_value = 20000; break;
            case '4': group_value = 2000; break;
            default : group_value = 2000; break;
        }
        
        tempresult *= group_value;
        tempresult *= a;
        S_TempCalData->paiduiUnitData->testData.testSeries.result = tempresult + group_value;
        
        S_TempCalData->resultstatues = ResultIsOK;
		return;
    #else
    
	//计算最大值,平均值
		S_TempCalData->maxdata = S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[0];
		S_TempCalData->tempvalue1 = 0;
		for(i=0; i<MaxPointLen; i++)
		{
			if(S_TempCalData->maxdata < S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[i])
				S_TempCalData->maxdata = S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[i];
			
			S_TempCalData->tempvalue1 += S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[i];
		}
		
		/*判断测试值是否饱和*/
		if(S_TempCalData->maxdata >= 4000)
		{
			if(getChannelNum() > 0)
			{
				SelectChannel(getChannelNum() - 1);

				vTaskDelay(10/portTICK_RATE_MS);
				return;
			}
		}
		else if(S_TempCalData->maxdata < 50)
		{
			if(getChannelNum() < 7)
			{
				SelectChannel(getChannelNum() + 3);
				
				vTaskDelay(10/portTICK_RATE_MS);
				return;
			}
		}
		
		//step.1 计算整条曲线的cv值，用于判断是否加样
		S_TempCalData->tempCV = calculateDataCV(S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint, 300, S_TempCalData->tempvalue1);
		if(S_TempCalData->tempCV < 0.025)
		{
            S_TempCalData->resultstatues = NoSample;
            goto END1;
        }
		
		//step.2 find T
		findFeng(S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint, S_TempCalData->paiduiUnitData->testData.qrCode.ItemLocation-30, 
			S_TempCalData->paiduiUnitData->testData.qrCode.ItemLocation, S_TempCalData->paiduiUnitData->testData.qrCode.ItemLocation+30,
			&S_TempCalData->paiduiUnitData->testData.testSeries.T_Point);
		
		//can not find T line
		if(S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.y == 0)
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x = S_TempCalData->paiduiUnitData->testData.qrCode.ItemLocation;
		
			S_TempCalData->CV_T = calculateDataCV(&S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x-15], 31, 0);

			if(S_TempCalData->CV_T > 0.05)
			{
                S_TempCalData->resultstatues = T_CV_0_05;
				goto END1;
			}
		}
		else
		{
			if(S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x < S_TempCalData->paiduiUnitData->testData.qrCode.ItemLocation) 
			{
				S_TempCalData->tempvalue3 = S_TempCalData->paiduiUnitData->testData.qrCode.ItemLocation - S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x;
				
				if(S_TempCalData->tempvalue3 >= 10)
		        {
					S_TempCalData->CV_T = calculateDataCV(&S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x], S_TempCalData->tempvalue3, 0);
		        	
			        if(S_TempCalData->CV_T < 0.01)
			        {
			        	S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x = S_TempCalData->paiduiUnitData->testData.qrCode.ItemLocation;
			        }
		        }
			}
		}
		
		S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.y = S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x];

		
		//step.3 找C线
		findFeng(S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint, S_TempCalData->paiduiUnitData->testData.qrCode.CLineLocation-30, 
			S_TempCalData->paiduiUnitData->testData.qrCode.CLineLocation, S_TempCalData->paiduiUnitData->testData.qrCode.CLineLocation+30,
			&S_TempCalData->paiduiUnitData->testData.testSeries.C_Point);
		if(S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.y == 0)
			S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.x = S_TempCalData->paiduiUnitData->testData.qrCode.CLineLocation;

		S_TempCalData->tempvalue3 = S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.x-15;
		if((300 - S_TempCalData->tempvalue3) > 31)
			S_TempCalData->CV_C = calculateDataCV(&S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->tempvalue3], 31, 0);
		else
			S_TempCalData->CV_C = calculateDataCV(&S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->tempvalue3], (300 - S_TempCalData->tempvalue3), 0);
		
        S_TempCalData->paiduiUnitData->testData.testSeries.c_cv = S_TempCalData->CV_C;
         
		if((S_TempCalData->CV_C > 0.03) && (S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.y == 0))
		{
            S_TempCalData->resultstatues = C_CV_ERR_1;
			goto END1;
		}
		
		if(S_TempCalData->CV_C < 0.03)
		{
            S_TempCalData->resultstatues = C_CV_ERR_2;
			goto END1;
		}
		
		S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.y = S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.x];

		//step.4 c.x - t.x should between in 50-100
		if(S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.x <= S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x)
		{
            S_TempCalData->resultstatues = C_L_T_L_1;
            goto END1;
        }
		
		S_TempCalData->tempvalue3 = S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.x - S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x;
		if((S_TempCalData->tempvalue3 < 50) || (S_TempCalData->tempvalue3 > 100))
		{
            S_TempCalData->resultstatues = C_L_T_L_2;
            goto END1;
        }
		
		//step.5 c+t cv > 0.2
		S_TempCalData->CV_T = calculateDataCV(&S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x-15], 31, 0);
		S_TempCalData->paiduiUnitData->testData.testSeries.t_cv = S_TempCalData->CV_T;
		if((S_TempCalData->CV_C + S_TempCalData->CV_T) < 0.13)
		{
            S_TempCalData->resultstatues = C_CV_T_CV;
            goto END1;
        }
		
		//step.6 canliu
		S_TempCalData->tempvalue1 = 0;
		S_TempCalData->tempCV = 0;
		for(i=20; i<S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x-30; i++)
		{
			S_TempCalData->tempvalue1 += S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[i];
			if(i >= 35)
			{
				S_TempCalData->CV_0 = calculateDataCV(&S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[20], i-20+1, S_TempCalData->tempvalue1);
				if(S_TempCalData->tempCV < S_TempCalData->CV_0)
					S_TempCalData->tempCV = S_TempCalData->CV_0;
			}
		}
		if(S_TempCalData->tempCV > 0.15)
		{
            S_TempCalData->resultstatues = CANLIU;
            goto END1;
        }
		
		//step.7 find b
		S_TempCalData->paiduiUnitData->testData.testSeries.B_Point.y = 10000;
		S_TempCalData->paiduiUnitData->testData.testSeries.B_Point.x = 0;

		for(i=S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.x; i<S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.x; i++)
		{
			if(S_TempCalData->paiduiUnitData->testData.testSeries.B_Point.y > S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[i])
			{
				S_TempCalData->paiduiUnitData->testData.testSeries.B_Point.x = i;
				S_TempCalData->paiduiUnitData->testData.testSeries.B_Point.y = S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[i];
			}
		}
		
		//step.8 b line value : c > b, t >= b
		if((S_TempCalData->paiduiUnitData->testData.testSeries.B_Point.y >= S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.y)
			|| (S_TempCalData->paiduiUnitData->testData.testSeries.B_Point.y > S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.y))
		{
            S_TempCalData->resultstatues = B_V_ERR;
		     goto END1;
		}
		
		S_TempCalData->paiduiUnitData->testData.testSeries.t_cv = S_TempCalData->CV_T;
		S_TempCalData->paiduiUnitData->testData.testSeries.c_cv = S_TempCalData->CV_C;
		
		//step 9 判断微球是否过少，，补偿
		S_TempCalData->maxdata = S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.y;
		S_TempCalData->tempvalue1 = 0;
		if(S_TempCalData->CV_T < 0.02 && (S_TempCalData->CV_C + S_TempCalData->CV_T) < 0.35)
		{
			S_TempCalData->tempvalue1 = 1.1;

			for(i=0; i<24; i++)
			{
				S_TempCalData->tempvalue2 = S_TempCalData->tempvalue1;
				S_TempCalData->tempvalue2 *= S_TempCalData->maxdata;
				S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.y = (unsigned short)S_TempCalData->tempvalue2;
				S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.x] = S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.y;
				
				S_TempCalData->tempvalue3 = S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.x-15;
				if(S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.x < 284)
					S_TempCalData->CV_C = calculateDataCV(&S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->tempvalue3], 31, 0);
				else
					S_TempCalData->CV_C = calculateDataCV(&S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->tempvalue3], (300 - S_TempCalData->tempvalue3), 0);
				
				if((S_TempCalData->CV_C + S_TempCalData->CV_T) >= 0.35)
					break;
				
				S_TempCalData->tempvalue1 += 0.1;
			}
		}
		
		S_TempCalData->paiduiUnitData->testData.testSeries.CMdifyNum = (unsigned char)(S_TempCalData->tempvalue1*10);
		
		/*计算结果*/
		S_TempCalData->tempvalue1 = (S_TempCalData->paiduiUnitData->testData.testSeries.T_Point.y - S_TempCalData->paiduiUnitData->testData.testSeries.B_Point.y);
		S_TempCalData->tempvalue2 = (S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.y - S_TempCalData->paiduiUnitData->testData.testSeries.B_Point.y);
		S_TempCalData->paiduiUnitData->testData.testSeries.TestPoint[S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.x] = S_TempCalData->maxdata;
		S_TempCalData->paiduiUnitData->testData.testSeries.C_Point.y = S_TempCalData->maxdata;
		
		/*原始峰高比*/
		S_TempCalData->paiduiUnitData->testData.testSeries.t_c = S_TempCalData->tempvalue1 / S_TempCalData->tempvalue2;
		S_TempCalData->paiduiUnitData->testData.testSeries.t_tc = S_TempCalData->tempvalue1 / (S_TempCalData->tempvalue1 + S_TempCalData->tempvalue2);
		
		if(S_TempCalData->paiduiUnitData->testData.qrCode.calMode == 1)
			S_TempCalData->finalBili = S_TempCalData->paiduiUnitData->testData.testSeries.t_c;
		else
			S_TempCalData->finalBili = S_TempCalData->paiduiUnitData->testData.testSeries.t_tc;
		
		//根据批次修改曲线
		if(CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IM1711-01", 9))
		{
			S_TempCalData->finalBili = S_TempCalData->paiduiUnitData->testData.testSeries.t_tc;
			
			if(S_TempCalData->finalBili <= 0.2865f)
				S_TempCalData->paiduiUnitData->testData.testSeries.result = S_TempCalData->finalBili * S_TempCalData->finalBili * 136.98f 
					+ S_TempCalData->finalBili * 37.055f - 0.1029f;
			else
				S_TempCalData->paiduiUnitData->testData.testSeries.result = 616.26f*S_TempCalData->finalBili*S_TempCalData->finalBili - 
					272.15f * S_TempCalData->finalBili + 50.805;
		}
		else if(CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IK1711-02", 9))
		{
			S_TempCalData->finalBili = S_TempCalData->paiduiUnitData->testData.testSeries.t_tc;
			
			if(S_TempCalData->finalBili <= 0.1577f)
				S_TempCalData->paiduiUnitData->testData.testSeries.result = S_TempCalData->finalBili * S_TempCalData->finalBili * 1082.0f 
					- S_TempCalData->finalBili * 92.803f + 2.0259f;
			else
				S_TempCalData->paiduiUnitData->testData.testSeries.result = 1015.9f*S_TempCalData->finalBili*S_TempCalData->finalBili - 
					282.47f * S_TempCalData->finalBili + 36.177f;
		}
		else if(CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IB1803-01", 9))
		{
			S_TempCalData->finalBili = S_TempCalData->paiduiUnitData->testData.testSeries.t_tc;
			
			if(S_TempCalData->finalBili <= 0.6133)
				S_TempCalData->paiduiUnitData->testData.testSeries.result = S_TempCalData->finalBili * S_TempCalData->finalBili * 16864.0f 
					- S_TempCalData->finalBili * 1510.7f + 26.155f;
			else if(S_TempCalData->finalBili <= 0.8483)
				S_TempCalData->paiduiUnitData->testData.testSeries.result = -63205.0f*S_TempCalData->finalBili*S_TempCalData->finalBili + 
					139155.0f * S_TempCalData->finalBili - 56327.0f;
			else
				S_TempCalData->paiduiUnitData->testData.testSeries.result = 261236.0f*S_TempCalData->finalBili - 207211.0f;
		}
		else if(CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IB1801-02", 9))
		{
			S_TempCalData->finalBili = S_TempCalData->paiduiUnitData->testData.testSeries.t_tc;
			
			if(S_TempCalData->finalBili <= 0.673)
				S_TempCalData->paiduiUnitData->testData.testSeries.result = S_TempCalData->finalBili * S_TempCalData->finalBili * 23703.0f 
					- S_TempCalData->finalBili * 1455.5f + 33.66f;
			else 
				S_TempCalData->paiduiUnitData->testData.testSeries.result = 512669.0f*S_TempCalData->finalBili*S_TempCalData->finalBili - 
					671741.0f * S_TempCalData->finalBili + 229288.0f;
		}
        else if(CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IB1807-01", 9))
		{
			if(S_TempCalData->finalBili <= 0.3154)
				S_TempCalData->paiduiUnitData->testData.testSeries.result = -17681.0f*S_TempCalData->finalBili * S_TempCalData->finalBili 
					+ S_TempCalData->finalBili * 15841.0f - 208.34f;
			else 
				S_TempCalData->paiduiUnitData->testData.testSeries.result = 802.31f*exp(4.6264f*S_TempCalData->finalBili);
		}
        
		#if (DeviceBuildId == Device_NCD13021801105)
		
		else if(CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IT1803-01", 9))
		{
			S_TempCalData->finalBili = S_TempCalData->paiduiUnitData->testData.testSeries.t_tc;
			
			if(S_TempCalData->finalBili <= 0.2054)
				S_TempCalData->paiduiUnitData->testData.testSeries.result = S_TempCalData->finalBili * S_TempCalData->finalBili * 68.189f 
					- S_TempCalData->finalBili * 2.4836f + 0.0625f;
			else 
				S_TempCalData->paiduiUnitData->testData.testSeries.result = 1.0506*exp(4.3132*S_TempCalData->finalBili);
		}
		else if(CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IK1804-01", 9))
		{
			S_TempCalData->finalBili = S_TempCalData->paiduiUnitData->testData.testSeries.t_tc;
			
			if(S_TempCalData->finalBili <= 0.24)
				S_TempCalData->paiduiUnitData->testData.testSeries.result = S_TempCalData->finalBili * S_TempCalData->finalBili * 104.64f 
					+ S_TempCalData->finalBili * 29.721f - 1.1381f;
			else 
				S_TempCalData->paiduiUnitData->testData.testSeries.result = 926.16*S_TempCalData->finalBili*S_TempCalData->finalBili - 
					374.1f * S_TempCalData->finalBili + 51.193f;
		}
		
		#endif
		else
		{
			/*根据分段，计算原始结果*/
			S_TempCalData->isE = false;
			if((S_TempCalData->finalBili < S_TempCalData->paiduiUnitData->testData.qrCode.ItemFenDuan[0]) || (S_TempCalData->paiduiUnitData->testData.qrCode.ItemFenDuan[0] == 0))
			{
				if(S_TempCalData->paiduiUnitData->testData.qrCode.qu1Ise)
					S_TempCalData->isE = true;
				S_TempCalData->tempvalue3 = 0;
			}
			else if((S_TempCalData->finalBili < S_TempCalData->paiduiUnitData->testData.qrCode.ItemFenDuan[1]) || (S_TempCalData->paiduiUnitData->testData.qrCode.ItemFenDuan[1] == 0))
			{
				if(S_TempCalData->paiduiUnitData->testData.qrCode.qu2Ise)
					S_TempCalData->isE = true;
				S_TempCalData->tempvalue3 = 1;
			}
			else
			{
				if(S_TempCalData->paiduiUnitData->testData.qrCode.qu3Ise)
					S_TempCalData->isE = true;
				S_TempCalData->tempvalue3 = 2;
			}
				
			if(S_TempCalData->isE)
			{
				S_TempCalData->paiduiUnitData->testData.testSeries.result = S_TempCalData->paiduiUnitData->testData.qrCode.ItemBiaoQu[S_TempCalData->tempvalue3][0] 
					* exp(S_TempCalData->paiduiUnitData->testData.qrCode.ItemBiaoQu[S_TempCalData->tempvalue3][1] * S_TempCalData->finalBili + 
					S_TempCalData->paiduiUnitData->testData.qrCode.ItemBiaoQu[S_TempCalData->tempvalue3][2] ) + 
					S_TempCalData->paiduiUnitData->testData.qrCode.ItemBiaoQu[S_TempCalData->tempvalue3][3];
			}
			else
			{
				S_TempCalData->paiduiUnitData->testData.testSeries.result = S_TempCalData->finalBili * S_TempCalData->finalBili;
				S_TempCalData->paiduiUnitData->testData.testSeries.result *= S_TempCalData->paiduiUnitData->testData.qrCode.ItemBiaoQu[S_TempCalData->tempvalue3][0];
							
				S_TempCalData->paiduiUnitData->testData.testSeries.result += (S_TempCalData->finalBili * S_TempCalData->paiduiUnitData->testData.qrCode.ItemBiaoQu[S_TempCalData->tempvalue3][1]);
							
				S_TempCalData->paiduiUnitData->testData.testSeries.result += S_TempCalData->paiduiUnitData->testData.qrCode.ItemBiaoQu[S_TempCalData->tempvalue3][2];
			}
		}

		if(S_TempCalData->paiduiUnitData->testData.testSeries.result < 0)
			S_TempCalData->paiduiUnitData->testData.testSeries.result = 0;

		if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IT1705-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result *= 0.6666f;
		}
		else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IK1705-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result /= 2.3f;
		}
		else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IT1708-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result /= 1.75f;
		}
		else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IM1705-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result /= 5.0f;
		}
        else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IT1805-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result /= 1.3f;
		}
        else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IB1803-01", 9))
		{
			if(S_TempCalData->paiduiUnitData->testData.testSeries.result < 1000)
                S_TempCalData->paiduiUnitData->testData.testSeries.result *= 5.0f;
            else if(S_TempCalData->paiduiUnitData->testData.testSeries.result <6000)
                S_TempCalData->paiduiUnitData->testData.testSeries.result *= 4.0f;
            else if(S_TempCalData->paiduiUnitData->testData.testSeries.result < 10000)
                S_TempCalData->paiduiUnitData->testData.testSeries.result *= 3.0f;
            else
                S_TempCalData->paiduiUnitData->testData.testSeries.result *= 2.0f;
		}
        else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IB1804-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result *= 2.0f;
		}
        else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IB1805-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result *= 2.0f;
		}
        else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IB1807-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result *= 2.0f;
		}
        
		//2018年3月29日 16:39:47 只针对孝感中心医院检验科的两台设备发布程序，其他设备不做修改
		#if ((DeviceBuildId == Device_NCD13021806109) || (DeviceBuildId == Device_NCD13021801106))
		else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IM1801-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result *= 1.6f;
		}
		else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IM1803-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result *= 2.5f;
		}
		else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IM1804-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result *= 2.5f;
		}
		
		#elif (DeviceBuildId == Device_NCD13021801105)
		
		else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IM1803-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result *= 2.5f;
		}
		else if(true == CheckStrIsSame(S_TempCalData->paiduiUnitData->testData.qrCode.PiHao, "IM1804-01", 9))
		{
			S_TempCalData->paiduiUnitData->testData.testSeries.result *= 2.5f;
		}
		
		#endif
		
		S_TempCalData->resultstatues = ResultIsOK;
		return;
		
		END1:
			S_TempCalData->paiduiUnitData->testData.testSeries.result = 0;
			return;
        
    #endif
}

