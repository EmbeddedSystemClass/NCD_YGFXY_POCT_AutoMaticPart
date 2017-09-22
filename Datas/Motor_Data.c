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
//定义4个电机
static Motor GB_Motors[3] = 
{
	//电机1，转盘
	{
		.highTime = 6,										//高电平时间段，(1-3)*100us
		.lowTime = 8,										//低电平时间段，(4)*100us
		.periodCnt = 0,
		.isFront = true,									//是否前进
		.moveStepNum = 0,									//运动步数
		.motorLocation = 0,									//代表当前电机对准口的编号，0 -- 无效数据， 1-8表示插卡空对应编号
		.motorTargetLocation = 0,							//代表电机目标对准口的编号，0 -- 无效数据， 1-8表示插卡空对应编号
		.motorMaxLocation = 60000,
		.parm1 = 0,											//两次中断的间隔步数
		.parm2 = true,										//由于停止位置在边沿，需要继续走几步到中间，true，继续走，false停止
		.parm3 = 0											//到达位置后，为了补偿居中，继续走的步数
	},
	//电机2，爪子移动
	{
		.highTime = 1,										//高电平时间，*100us
		.lowTime = 2,										//低电平时间，*100us
		.periodCnt = 0,
		.isFront = true,									//是否前进
		.moveStepNum = 0,									//运动步数
		.motorLocation = 60000,									//电机当前位置
		.motorTargetLocation = 0,							//电机目标位置
		.motorMaxLocation = 65000,
	},
	//电机4，爪子捏合
	{
		.highTime = 1,										//高电平时间，*100us
		.lowTime = 2,										//低电平时间，*100us
		.periodCnt = 0,
		.isFront = true,									//是否前进
		.moveStepNum = 0,									//运动步数
		.motorLocation = 60000,								//电机当前位置
		.motorTargetLocation = 0,							//电机目标位置
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
	//如果是前进
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
