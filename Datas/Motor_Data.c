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
static Motor GB_Motors[3] = 
{
	//���1��ת��
	{
		.highTime = 6,										//�ߵ�ƽʱ��Σ�(1-3)*100us
		.lowTime = 8,										//�͵�ƽʱ��Σ�(4)*100us
		.periodCnt = 0,
		.isFront = true,									//�Ƿ�ǰ��
		.moveStepNum = 0,									//�˶�����
		.motorLocation = 0,									//����ǰ�����׼�ڵı�ţ�0 -- ��Ч���ݣ� 1-8��ʾ�忨�ն�Ӧ���
		.motorTargetLocation = 0,							//������Ŀ���׼�ڵı�ţ�0 -- ��Ч���ݣ� 1-8��ʾ�忨�ն�Ӧ���
		.motorMaxLocation = 60000,
		.parm1 = 0,											//�����жϵļ������
		.parm2 = true,										//����ֹͣλ���ڱ��أ���Ҫ�����߼������м䣬true�������ߣ�falseֹͣ
		.parm3 = 0											//����λ�ú�Ϊ�˲������У������ߵĲ���
	},
	//���2��צ���ƶ�
	{
		.highTime = 1,										//�ߵ�ƽʱ�䣬*100us
		.lowTime = 2,										//�͵�ƽʱ�䣬*100us
		.periodCnt = 0,
		.isFront = true,									//�Ƿ�ǰ��
		.moveStepNum = 0,									//�˶�����
		.motorLocation = 60000,									//�����ǰλ��
		.motorTargetLocation = 0,							//���Ŀ��λ��
		.motorMaxLocation = 65000,
	},
	//���4��צ�����
	{
		.highTime = 1,										//�ߵ�ƽʱ�䣬*100us
		.lowTime = 2,										//�͵�ƽʱ�䣬*100us
		.periodCnt = 0,
		.isFront = true,									//�Ƿ�ǰ��
		.moveStepNum = 0,									//�˶�����
		.motorLocation = 60000,								//�����ǰλ��
		.motorTargetLocation = 0,							//���Ŀ��λ��
		.motorMaxLocation = 65000
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
Motor * getMotor(Motorx_Def motor)
{
	return &GB_Motors[motor];
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


/****************************************end of file************************************************/
