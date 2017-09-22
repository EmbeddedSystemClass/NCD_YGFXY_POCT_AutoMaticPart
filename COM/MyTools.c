/***************************************************************************************************
*FileName:
*Description:
*Author:xsx
*Data:
***************************************************************************************************/


/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"MyTools.h"

#include	"math.h"
#include	<string.h>
#include	"stdio.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/





/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/







/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/************************************************************************
** ������:CheckStrIsSame
** ��  ��:�Ƚ��ַ����Ƿ���ͬ
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
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
*Description: ����cvֵ
*Input: datas -- ����
*		len -- ���ݳ���
*		sum -- �����ۼӺͣ����Ϊ0������Ҫ���㣬����ֱ��ʹ��
*Output: 
*Return: 
*Author: xsx
*Date: 2017��8��2�� 14:06:33
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
	for(i=startIndex; i<endIndex; i++)
	{
		tempPoint.x = 0;
		tempPoint.y = 0;
		for(j=i-15; j<15+i; j++) 
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
			i += 10;
			continue;
		}
		else 
		{
			if(tempPoint.y > myPoint->y)
			{
				myPoint->x = tempPoint.x;
				myPoint->y = tempPoint.y;
			}
			i = (tempPoint.x + 15);
		}
	}
}
