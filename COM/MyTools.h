#ifndef MYTOOLS_H_H
#define MYTOOLS_H_H

#include	"Define.h"
#include	"NetWork.h"
#include	"TestData.h"

bool CheckStrIsSame(const void *str1 , const void * str2 , unsigned short len);
void clearStringEndWord(char * str, unsigned short len);
float calculateDataCV(unsigned short * datas, unsigned short len, double sum);
void findFeng(unsigned short * datas, unsigned short startIndex, unsigned short midIndex, unsigned short endIndex, Point * myPoint);
unsigned short getMaxMinValue(unsigned short * datas, unsigned short len, Point * max, Point * min);
MyRes parseIpString(IP * ip, char * ipStr);
MyRes parsePortString(unsigned short * port, char * portStr);
#endif

