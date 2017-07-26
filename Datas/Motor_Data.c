/******************************************File Start**********************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include	"Motor_Data.h"
/***************************************************************************************************/
/******************************************Static Variables*****************************************/
/***************************************************************************************************/
//����4�����
static Motor GB_Motors[4] = 
{
	//���1��ת��
	{
		.highTime = 3,										//�ߵ�ƽʱ��Σ�(1-3)*100us
		.lowTime = 4,										//�͵�ƽʱ��Σ�(4)*100us
		.periodCnt = 0,
		.isFront = true,									//�Ƿ�ǰ��
		.isSleep = false,									//�Ƿ�����
		.moveStepNum = 0,									//�˶�����
		.motorLocation = 0,									//�����ǰλ��
		.motorTargetLocation = 0,							//���Ŀ��λ��
		.motorMaxLocation = 60000,
	},
	//���2���˿�
	{
		.highTime = 2,										//�ߵ�ƽʱ�䣬*100us
		.lowTime = 3,										//�͵�ƽʱ�䣬*100us
		.periodCnt = 0,
		.isFront = true,									//�Ƿ�ǰ��
		.isSleep = false,									//�Ƿ�����
		.moveStepNum = 0,									//�˶�����
		.motorLocation = 0,									//�����ǰλ��
		.motorTargetLocation = 0,							//���Ŀ��λ��
		.motorMaxLocation = 60000,
	},
	//���3��צ���ƶ�
	{
		.highTime = 1,										//�ߵ�ƽʱ�䣬*100us
		.lowTime = 2,										//�͵�ƽʱ�䣬*100us
		.periodCnt = 0,
		.isFront = true,									//�Ƿ�ǰ��
		.isSleep = false,									//�Ƿ�����
		.moveStepNum = 0,									//�˶�����
		.motorLocation = 0,									//�����ǰλ��
		.motorTargetLocation = 0,							//���Ŀ��λ��
		.motorMaxLocation = 60000,
	},
	//���4��צ�����
	{
		.highTime = 1,										//�ߵ�ƽʱ�䣬*100us
		.lowTime = 2,										//�͵�ƽʱ�䣬*100us
		.periodCnt = 0,
		.isFront = true,									//�Ƿ�ǰ��
		.isSleep = false,									//�Ƿ�����
		.moveStepNum = 0,									//�˶�����
		.motorLocation = 0,									//�����ǰλ��
		.motorTargetLocation = 0,							//���Ŀ��λ��
		.motorMaxLocation = 60000,
		.parm1 = false,										//���4�źţ�true��ʾ��������falseΪ��Ч�ź�
	}
};
/***************************************************************************************************/
/******************************************Static Methods*******************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/******************************************Main Body************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName:  
*Description:   
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  
***************************************************************************************************/
void setMotorxDir(Motorx_Def motor, bool value)
{
	GB_Motors[motor].isFront = value;
}

bool getMotorxDir(Motorx_Def motor)
{
	return GB_Motors[motor].isFront;
}

void toggleMotorxDir(Motorx_Def motor)
{
	if(GB_Motors[motor].isFront)
		GB_Motors[motor].isFront = false;
	else
		GB_Motors[motor].isFront = true;
}

/***************************************************************************************************
*FunctionName:  
*Description:   
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  
***************************************************************************************************/
void setMotorxSleep(Motorx_Def motor, bool value)
{
	GB_Motors[motor].isSleep = value;
}

bool getMotorxSleep(Motorx_Def motor)
{
	return GB_Motors[motor].isSleep;
}

void toggleMotorxSleep(Motorx_Def motor)
{
	if(GB_Motors[motor].isSleep)
		GB_Motors[motor].isSleep = false;
	else
		GB_Motors[motor].isSleep = true;
}

/***************************************************************************************************
*FunctionName:  
*Description:   
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  
***************************************************************************************************/
void setMotorxMoveStepNum(Motorx_Def motor, unsigned short value)
{
	//�����ǰ��
	if(GB_Motors[motor].isFront)
	{
		if(GB_Motors[motor].motorMaxLocation < (GB_Motors[motor].motorLocation + value))
		{
			if(GB_Motors[motor].motorMaxLocation > GB_Motors[motor].motorLocation)
				value = GB_Motors[motor].motorMaxLocation - GB_Motors[motor].motorLocation;
			else
				value = 0;
		}
	}
	else
	{
		if(value > GB_Motors[motor].motorLocation)
			value = GB_Motors[motor].motorLocation;
	}
	
	GB_Motors[motor].moveStepNum = value;
}

unsigned short getMotorxMoveStepNum(Motorx_Def motor)
{
	return GB_Motors[motor].moveStepNum;
}

void minusMotorxMoveStepNum(Motorx_Def motor)
{
	if(GB_Motors[motor].moveStepNum > 0)
		GB_Motors[motor].moveStepNum--;
}

/***************************************************************************************************
*FunctionName:  
*Description:   
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  
***************************************************************************************************/
void setMotorxPeriodCnt(Motorx_Def motor, unsigned char value)
{
	GB_Motors[motor].periodCnt = value;
}

unsigned char getMotorxPeriodCnt(Motorx_Def motor)
{
	return GB_Motors[motor].periodCnt;
}

void plusMotorxPeriodCnt(Motorx_Def motor)
{
	if(GB_Motors[motor].periodCnt < 200)
		GB_Motors[motor].periodCnt++;
}
/***************************************************************************************************
*FunctionName:  
*Description:   
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  
***************************************************************************************************/
void setMotorxLocation(Motorx_Def motor, unsigned short value)
{
	GB_Motors[motor].motorLocation = value;
}

unsigned short getMotorxLocation(Motorx_Def motor)
{
	return GB_Motors[motor].motorLocation;
}

void plusMotorxLocation(Motorx_Def motor, unsigned short value)
{
	if(GB_Motors[motor].isFront)
	{
		if((GB_Motors[motor].motorLocation + value) > GB_Motors[motor].motorMaxLocation)
			GB_Motors[motor].motorLocation = GB_Motors[motor].motorMaxLocation;
		else
			GB_Motors[motor].motorLocation += value;
	}
	else
	{
		if(GB_Motors[motor].motorLocation > value)
			GB_Motors[motor].motorLocation -= value;
		else
			GB_Motors[motor].motorLocation = 0;
	}
}

/***************************************************************************************************
*FunctionName:  
*Description:   
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  
***************************************************************************************************/
void setMotorxTargetLocation(Motorx_Def motor, unsigned short value)
{
	GB_Motors[motor].motorTargetLocation = value;
}

unsigned short getMotorxTargetLocation(Motorx_Def motor)
{
	return GB_Motors[motor].motorTargetLocation;
}

/***************************************************************************************************
*FunctionName:  
*Description:   
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  
***************************************************************************************************/
unsigned short getMotorxHighTime(Motorx_Def motor)
{
	return GB_Motors[motor].highTime;
}

unsigned short getMotorxLowTime(Motorx_Def motor)
{
	return GB_Motors[motor].lowTime;
}

/***************************************************************************************************
*FunctionName:  
*Description:   
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  
***************************************************************************************************/
unsigned short getMotorxMaxLocation(Motorx_Def motor)
{
	return GB_Motors[motor].motorMaxLocation;
}

/***************************************************************************************************
*FunctionName:  
*Description:   
*Input:  
*Output:  
*Return:  
*Author:  xsx
*Date:  
***************************************************************************************************/
void setMotorxParm1(Motorx_Def motor, bool value)
{
	GB_Motors[motor].parm1 = value;
}

bool getMotorxParm1(Motorx_Def motor)
{
	return GB_Motors[motor].parm1;
}

/****************************************end of file************************************************/
