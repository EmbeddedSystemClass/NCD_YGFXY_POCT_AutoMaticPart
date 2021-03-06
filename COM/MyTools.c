/***************************************************************************************************
*FileName:
*Description:
*Author:xsx
*Data:
***************************************************************************************************/


/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/

#include	"MyTools.h"

#include	"math.h"
#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/





/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/







/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/************************************************************************
** 函数名:CheckStrIsSame
** 功  能:比较字符串是否相同
** 输  入:无
** 输  出:无
** 返  回：无
** 备  注：无
** 时  间:  
** 作  者：xsx                                                 
************************************************************************/
bool CheckStrIsSame(const void *str1 , const void * str2 , unsigned short len)
{
	unsigned char *p = (unsigned char *)str1;
	unsigned char *q = (unsigned char *)str2;
	unsigned short i=0;
	
	if((NULL == p) || (NULL == q))
		return false;
	
	for(i=0; i<len; i++)
	{
		if(*q++ != *p++)
			return false;
	}
	
	return true;
}

/***************************************************************************************************
*FunctionName：clearStringEndWord
*Description：去掉字符串末尾的回车，主要用于条码枪的数据
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月21日14:54:56
***************************************************************************************************/
void clearStringEndWord(char * str, unsigned short len)
{
	for(unsigned short i=0; i<len; i++)
	{
		if(*str == 0x0d || *str == 0x0a)
			*str = 0;
		
		str++;
	}
}

unsigned short getMaxMinValue(unsigned short * datas, unsigned short len, Point * max, Point * min)
{
	unsigned short i=0;
	unsigned int avg = 0;
	
	max->x = 0;
	max->y = 0;
	min->x = 0;
	min->y = 0xffff;
	
	for(i=0; i<len; i++)
	{
		if(max->y < datas[i])
		{
			max->y = datas[i];
			max->x = i;
		}
			
		if(min->y > datas[i])
		{
			min->y = datas[i];
			min->x = i;
		}
		
		avg += datas[i];
	}
	
	avg /= len;
	
	return avg;
}

/***************************************************************************************************
*FunctionName: calculateDataCV
*Description: 计算cv值
*Input: datas -- 数据
*		len -- 数据长度
*		sum -- 数据累加和，如果为0，则需要计算，否则直接使用
*Output: 
*Return: 
*Author: xsx
*Date: 2017年8月2日 14:06:33
***************************************************************************************************/
float calculateDataCV(unsigned short * datas, unsigned short len, double sum)
{
	double average = 0;
	double tempV1 = 0;
	double tempV2 = 0;
	unsigned short i=0;
	unsigned short *p = datas;
	
	if(sum == 0)
	{
		for(i=0; i<len; i++)
			average += *p++;
	}
	else
		average = sum;
	average /= len;
	
	p = datas;
	for(i=0; i<len; i++)
	{
		tempV1 = *p++;
		tempV1 -= average;
		tempV1 *= tempV1;
		tempV2 += tempV1;
	}
	
	tempV2 /= len;
	tempV2 = sqrt(tempV2);

	return tempV2 / average;
}

void findFeng(unsigned short * datas, unsigned short startIndex, unsigned short midIndex, unsigned short endIndex, Point * myPoint)
{
	unsigned short i=0, j=0;
	double tempv1 = 0;
	Point tempPoint;
		
	myPoint->x = 0;
	myPoint->y = 0;
    
    if(endIndex > 300)
        endIndex = 300;
    
	for(i=startIndex; i<endIndex-10; i++)
	{
		tempPoint.x = 0;
		tempPoint.y = 0;
		for(j=i; j<10+i; j++) 
		{
			tempv1 = datas[j];
			if(tempPoint.y < tempv1) 
			{
				tempPoint.y = tempv1;
				tempPoint.x = j;
			}
		}

		for(j=0; j<10; j++) 
		{
			if(datas[tempPoint.x-j] < datas[tempPoint.x-j-1])
				break;

			if(tempPoint.x+j+1 < MaxPointLen)
			{
				if(datas[tempPoint.x+j] < datas[tempPoint.x+j+1])
					break;
			}		
		}

		if(j < 10)
		{
			continue;
		}
		else 
		{
			if(tempPoint.y > myPoint->y)
			{
				myPoint->x = tempPoint.x;
				myPoint->y = tempPoint.y;
			}
			i = (tempPoint.x + 10);
		}
	}
}

MyRes parseIpString(IP * ip, char * ipStr)
{
	char * tempP = NULL;
	
	if(ip == NULL || ipStr == NULL)
		return My_Fail;
	
	tempP = strtok(ipStr, ".");
	if(tempP)
	{
		ip->ip_1 = strtol(tempP, NULL, 10);
		if(ip->ip_1 > 255)
			return My_Fail;
	}
	else
		return My_Fail;
	
	tempP = strtok(NULL, ".");
	if(tempP)
	{
		ip->ip_2 = strtol(tempP, NULL, 10);
		if(ip->ip_2 > 255)
			return My_Fail;
	}
	else
		return My_Fail;
	
	tempP = strtok(NULL, ".");
	if(tempP)
	{
		ip->ip_3 = strtol(tempP, NULL, 10);
		if(ip->ip_3 > 255)
			return My_Fail;
	}
	else
		return My_Fail;
	
	tempP = strtok(NULL, ".");
	if(tempP)
	{
		ip->ip_4 = strtol(tempP, NULL, 10);
		if(ip->ip_4 > 255)
			return My_Fail;
	}
	else
		return My_Fail;
	
	return My_Pass;
}

MyRes parsePortString(unsigned short * port, char * portStr)
{
	if(port == NULL || portStr == NULL)
		return My_Fail;
	
	*port = strtol(portStr, NULL, 10);
	if(*port > 65535)
		return My_Fail;
	
	return My_Pass;
}

/***************************************************************************************************
*FunctionName: 
*Description: 小到大
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
void bubbleSort(unsigned short a[], unsigned char n)
{
	 unsigned short tmp = 0;
    for(unsigned char i =0 ; i< n-1; ++i) 
	{  
        for(unsigned char j = 0; j < n-i-1; ++j) 
		{  
            if(a[j] > a[j+1])  
            {  
                tmp = a[j] ; 
				a[j] = a[j+1] ;  
				a[j+1] = tmp;  
            }  
        }  
    }  
}


