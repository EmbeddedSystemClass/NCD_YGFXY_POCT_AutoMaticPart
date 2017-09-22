/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"ShowResultPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"Test_Fun.h"
#include	"Intent.h"

#include	"MyTest_Data.h"
#include	"System_Data.h"
#include	"Define.h"
#include	"Printf_Fun.h"
#include	"RecordPage.h"
#include	"SleepPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static ShowPageBuffer * S_ShowPageBuffer = NULL;

static unsigned int TestLineHigh = 76500;	//��������������ʾ����߶��йأ�������治�ģ���������
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void RefreshText(void);
static void DspLine(void);
static void dspIco(void);

static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyRes activityBufferMalloc(void);
static void activityBufferFree(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: ����ѡ������˽���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyRes createShowResultActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "ShowResultActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		//������������
		if(pram)
		{
			readIntent(pram, &S_ShowPageBuffer->testdata);
		}
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: ��ʾ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:32
***************************************************************************************************/
static void activityStart(void)
{
	RefreshText();
			
	DspLine();
			
	dspIco();
	
	SelectPage(147);
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	if(S_ShowPageBuffer)
	{
		/*����*/
		S_ShowPageBuffer->lcdinput[0] = pbuf[4];
		S_ShowPageBuffer->lcdinput[0] = (S_ShowPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*�˳�*/
		if(0x2301 == S_ShowPageBuffer->lcdinput[0])
		{
			backToFatherActivity();
		}
		/*��ӡ*/
		else if(0x2300 == S_ShowPageBuffer->lcdinput[0])
		{
			SendKeyCode(1);
			PrintfData(&(S_ShowPageBuffer->testdata));
			SendKeyCode(16);
		}
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: ����ˢ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:16
***************************************************************************************************/
static void activityFresh(void)
{

}

/***************************************************************************************************
*FunctionName: activityHide
*Description: ���ؽ���ʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:40
***************************************************************************************************/
static void activityHide(void)
{

}

/***************************************************************************************************
*FunctionName: activityResume
*Description: ����ָ���ʾʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:58
***************************************************************************************************/
static void activityResume(void)
{
	SelectPage(147);
}

/***************************************************************************************************
*FunctionName: activityDestroy
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:02:15
***************************************************************************************************/
static void activityDestroy(void)
{
	activityBufferFree();
}

/***************************************************************************************************
*FunctionName: activityBufferMalloc
*Description: ���������ڴ�����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static MyRes activityBufferMalloc(void)
{
	if(NULL == S_ShowPageBuffer)
	{
		S_ShowPageBuffer = MyMalloc(sizeof(ShowPageBuffer));
		
		if(S_ShowPageBuffer)
		{
			memset(S_ShowPageBuffer, 0, sizeof(ShowPageBuffer));
	
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
*Description: �����ڴ��ͷ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:03:10
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(S_ShowPageBuffer);
	S_ShowPageBuffer = NULL;
}

static void RefreshText(void)
{
	sprintf(S_ShowPageBuffer->tempbuf, "%s\0", S_ShowPageBuffer->testdata.qrCode.ItemName);
	DisText(0x2310, S_ShowPageBuffer->tempbuf, strlen(S_ShowPageBuffer->tempbuf)+1);

	sprintf(S_ShowPageBuffer->tempbuf, "%s\0", S_ShowPageBuffer->testdata.sampleid);
	DisText(0x2320, S_ShowPageBuffer->tempbuf, strlen(S_ShowPageBuffer->tempbuf)+1);
				
	sprintf(S_ShowPageBuffer->tempbuf, "%s\0", S_ShowPageBuffer->testdata.operator.name);
	DisText(0x2330, S_ShowPageBuffer->tempbuf, strlen(S_ShowPageBuffer->tempbuf)+1);
		
	sprintf(S_ShowPageBuffer->tempbuf, "%s-%s\0", S_ShowPageBuffer->testdata.qrCode.PiHao,
		S_ShowPageBuffer->testdata.qrCode.piNum);
	DisText(0x2340, S_ShowPageBuffer->tempbuf, strlen(S_ShowPageBuffer->tempbuf)+1);
		
	if(S_ShowPageBuffer->testdata.testResultDesc != ResultIsOK)
		sprintf(S_ShowPageBuffer->tempbuf, "Error\0");
	else if(S_ShowPageBuffer->testdata.testSeries.result <= S_ShowPageBuffer->testdata.qrCode.itemConstData.lowstResult)
		snprintf(S_ShowPageBuffer->tempbuf, 20, "<%.*f %s", S_ShowPageBuffer->testdata.qrCode.itemConstData.pointNum, 
			S_ShowPageBuffer->testdata.qrCode.itemConstData.lowstResult, S_ShowPageBuffer->testdata.qrCode.itemConstData.itemMeasure);
	else
		sprintf(S_ShowPageBuffer->tempbuf, "%.*f %s\0", S_ShowPageBuffer->testdata.qrCode.itemConstData.pointNum,
			S_ShowPageBuffer->testdata.testSeries.result, S_ShowPageBuffer->testdata.qrCode.itemConstData.itemMeasure);
	DisText(0x2338, S_ShowPageBuffer->tempbuf, strlen(S_ShowPageBuffer->tempbuf)+1);
		
	sprintf(S_ShowPageBuffer->tempbuf, "%s\0", S_ShowPageBuffer->testdata.qrCode.itemConstData.normalResult);

	DisText(0x2350, S_ShowPageBuffer->tempbuf, strlen(S_ShowPageBuffer->tempbuf)+1);
}

static void DspLine(void)
{
	unsigned short i=0;
	double TempMul_Y2;
	double TempY_Scale;
	unsigned short tempvalue;
	
	unsigned short *p = NULL;

	ClearLine(0x57);
	vTaskDelay(200 / portTICK_RATE_MS);
	for(i=0; i<MaxPointLen;i++)
	{
		if(i%50 == 0)
		{
			p = &(S_ShowPageBuffer->testdata.testSeries.TestPoint[i]);
			DisPlayLine(1 , p , 50);
			vTaskDelay(2 / portTICK_RATE_MS);
		}
		
		if(S_ShowPageBuffer->lineinfo.MaxData <= S_ShowPageBuffer->testdata.testSeries.TestPoint[i])
			S_ShowPageBuffer->lineinfo.MaxData = S_ShowPageBuffer->testdata.testSeries.TestPoint[i];
	}
	
	////////////////////////��Ե�ǰ�������ֵ����y��Ŵ���//////////////////////////////////////
		TempMul_Y2 = TestLineHigh;
		TempMul_Y2 /= S_ShowPageBuffer->lineinfo.MaxData;
		TempMul_Y2 *= 0.8;			//*0.8�ǽ����ֵ���ŵ����̶ȵ�0.8�߶ȴ�

		
		tempvalue = (unsigned short)(TempMul_Y2*10);
		S_ShowPageBuffer->lineinfo.MUL_Y = ((tempvalue%10) > 5)?(tempvalue/10 + 1):(tempvalue/10);
		
		if(S_ShowPageBuffer->lineinfo.MUL_Y < 1)			//��СֵΪ1
			S_ShowPageBuffer->lineinfo.MUL_Y = 1;

		/////////////////////////��Ե�ǰ�Ŵ���������y��̶ȵ�������/////////////////////////////////////
		TempY_Scale = TestLineHigh ;
		TempY_Scale /= S_ShowPageBuffer->lineinfo.MUL_Y;
		TempY_Scale /= 2.0;																//Ŀǰ��ʾ2��y��̶�
		S_ShowPageBuffer->lineinfo.Y_Scale = (unsigned short)TempY_Scale;
		
		SetChartSize(0x2370 , S_ShowPageBuffer->lineinfo.MUL_Y);

		DspNum(0x230b , S_ShowPageBuffer->lineinfo.Y_Scale, 2);
		DspNum(0x230a , S_ShowPageBuffer->lineinfo.Y_Scale*2, 2);

}

static void dspIco(void)
{
	double tempvalue = 0.0;
	if(S_ShowPageBuffer)
	{
		//�������ϱ�ǳ�T,C,����
		S_ShowPageBuffer->myico[0].ICO_ID = 22;
		S_ShowPageBuffer->myico[0].X = 574+S_ShowPageBuffer->testdata.testSeries.T_Point.x-12;
		tempvalue = S_ShowPageBuffer->testdata.testSeries.T_Point.y;
		tempvalue /= S_ShowPageBuffer->lineinfo.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_ShowPageBuffer->myico[0].Y = (unsigned short)tempvalue - 11;
		
		S_ShowPageBuffer->myico[1].ICO_ID = 22;
		S_ShowPageBuffer->myico[1].X = 574+S_ShowPageBuffer->testdata.testSeries.C_Point.x-12;
		tempvalue = S_ShowPageBuffer->testdata.testSeries.C_Point.y;
		tempvalue /= S_ShowPageBuffer->lineinfo.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_ShowPageBuffer->myico[1].Y = (unsigned short)tempvalue - 11;
		
		S_ShowPageBuffer->myico[2].ICO_ID = 22;
		S_ShowPageBuffer->myico[2].X = 574+S_ShowPageBuffer->testdata.testSeries.B_Point.x-12;
		tempvalue = S_ShowPageBuffer->testdata.testSeries.B_Point.y;
		tempvalue /= S_ShowPageBuffer->lineinfo.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_ShowPageBuffer->myico[2].Y = (unsigned short)tempvalue - 11;
		
		BasicUI(0x2380 ,0x1807 , 3, &(S_ShowPageBuffer->myico[0]) , sizeof(Basic_ICO)*3);
	}
}

