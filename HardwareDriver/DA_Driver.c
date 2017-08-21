/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"DA_Driver.h"
/***************************************************************************************************/
/******************************************Static Variables***************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/******************************************Static Methods***************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName: DA_GPIOInit
*Description: 发光二极管，基线控制DA GPIO初始化
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年8月7日 10:08:39
***************************************************************************************************/
void DA_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_AHB1PeriphClockCmd(DA_Led_Rcc | DA_Line_Rcc, ENABLE);                         

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DA_Led_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DA_Led_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DA_Line_Pin;
	GPIO_Init(DA_Line_Group, &GPIO_InitStructure);
	
	DAC_InitType.DAC_Trigger = DAC_Trigger_None;
	DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DA_Led_Channel, &DAC_InitType);
	DAC_Init(DA_Line_Channel, &DAC_InitType);

	DAC_Cmd(DA_Led_Channel, ENABLE);
	DAC_Cmd(DA_Line_Channel, ENABLE);
  
	SetLedVol(0);
	SetLineVol(0);
}

/***************************************************************************************************
*FunctionName: SetLedVol
*Description: 控制发光二极管输出电压
*Input: volNum -- 输出电压值（mv）
*Output: 
*Return: 
*Author: xsx
*Date: 2017年8月7日 10:09:15
***************************************************************************************************/
void SetLedVol(unsigned short volNum)
{
	float temp = volNum;

	temp *= 4096.2f;
	temp /= 3300.1f;
	
	DAC_SetChannel1Data(DAC_Align_8b_R, temp);
}

/***************************************************************************************************
*FunctionName: SetLineVol
*Description: 控制基线输出电压（mv）
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年8月7日 10:09:37
***************************************************************************************************/
void SetLineVol(unsigned short volNum)
{
	float temp = volNum;

	temp *= 4096.2f;
	temp /= 3300.1f;
	
	DAC_SetChannel2Data(DAC_Align_12b_R, temp);
}
/****************************************end of file************************************************/
