#ifndef __SIM800C_F_H__
#define __SIM800C_F_H__

#include	"Define.h"
#include	"HttpBuf.h"

#define ATStr              "AT\r\0"
#define AT0             "ATE0\r\0"
#define AT1             "ATE1\r\0"
#define AT_CPIN         "AT+CPIN?\r\0"
#define AT_CSQ          "AT+CSQ\r\0"
#define AT_CNUM         "AT+CNUM\r\0"
#define AT_CREG         "AT+CREG?\r\0"
#define AT_COPS         "AT+COPS\r\0"
#define AT_CIPCLOSE1    "AT+CIPCLOSE=1\r\0"                                     //关闭连接
#define	AT_CIPSHUT      "AT+CIPSHUT\r\0"                                        //关闭移动场景 
#define	AT_CGCLASSB     "AT+CGCLASS=\"B\"\r\0"                                  //设置GPRS移动台类别为B,支持包交换和数据交换 
#define	AT_CGDCONT1     "AT+CGDCONT=1,\"IP\",\"CMNET\"\r\0"                     //设置PDP上下文,互联网接协议,接入点等信息
#define	AT_CGATT1       "AT+CGATT=1\r\0"                                        //附着GPRS业务
#define	AT_CGATT        "AT+CGATT?\r\0"                                         //查询附着GPRS业务
#define	AT_CIPCSGP1     "AT+CIPCSGP=1,\"CMNET\"\r\0"                            //设置为GPRS连接模式
#define	AT_CIPHEAD_1    "AT+CIPHEAD=1\r\0"                                      //设置接收数据显示IP头(方便判断数据来源)
#define	AT_CIPSEND    "AT+CIPSEND\r\0"                                          //发送数据

#define AT_OK           "OK\0"
#define AT_READY        "READY\0"
#define gprsData    "GET /iot/datacollection.do?iccid=89860114871025185511&battery=3.72 HTTP/1.1\n\
Host: iot.osapling.com\n\
User-Agent: xsx\n\
Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\n\
Connection: keep-alive\n\
\n\0"

#pragma pack(1)
typedef struct Sim800InitBuf_Tag
{
	char recvBuf[1024];
	char * tempP;
	unsigned char tempChar;
	unsigned short tempShort;
	unsigned int tempInt;
}Sim800InitBuf;
#pragma pack()

void createGSMMutex(void);
MyRes takeGSMMutex(portTickType xBlockTime);
void giveGSMxMutex(void);
MyRes ComWithSim800c(char * txBuf, const char *strcmp, char *rxBuf, unsigned short maxRxLen, portTickType delayTime, unsigned char maxErrorCnt, portTickType waitTime);
bool checkIsATMode(char * recvBuf);
MyRes ConnectServer(char * recvBuf);
void readGSMInd(Sim800InitBuf * sim800InitBuf, unsigned char * ind);
void readGSMIccid(Sim800InitBuf * sim800InitBuf, char * iccid);
void readGSMPhoneNum(Sim800InitBuf * sim800InitBuf, char * num);
MyRes CommunicateWithNcdServerInGPRS(HttpBuf * httpBuf);
#endif

