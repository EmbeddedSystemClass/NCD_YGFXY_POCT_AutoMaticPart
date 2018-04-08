/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"TCP_Fun.h"

#include	"QueueUnits.h"
#include	"StringDefine.h"

#include	"tcp.h"
#include 	"api.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
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
#if (UserProgramType == UserNormalProgram)
MyRes CommunicateWithServerByLineNet(HttpBuf * httpBuffer)
{
	err_t err;
	
	httpBuffer->status = My_Fail;
	
	IP4_ADDR(&httpBuffer->server_ipaddr, httpBuffer->serverSet.serverIP.ip_1, httpBuffer->serverSet.serverIP.ip_2, 
		httpBuffer->serverSet.serverIP.ip_3, httpBuffer->serverSet.serverIP.ip_4);

	//创建连接
	httpBuffer->clientconn = netconn_new(NETCONN_TCP);
	//创建失败
	if(httpBuffer->clientconn == NULL)
		return httpBuffer->status;

	//绑定本地ip
	err = netconn_bind(httpBuffer->clientconn, IP_ADDR_ANY, 0);
	//连接失败
	if(err != ERR_OK)
		goto END2;

	//尝试连接远程服务器
	err = netconn_connect(httpBuffer->clientconn, &httpBuffer->server_ipaddr, httpBuffer->serverSet.serverPort);
	//连接失败
	if(err != ERR_OK)
		goto END2;
		
	//设置接收数据超时时间100MS
	httpBuffer->clientconn->recv_timeout = 3000;
		
		//发送数据
	err = netconn_write(httpBuffer->clientconn, httpBuffer->sendBuf, httpBuffer->sendDataLen, NETCONN_COPY );
	//发送失败
	if(err != ERR_OK)
		goto END1;
		
	//接收数据
	httpBuffer->recvDataLen = 0;
	memset(httpBuffer->recvBuf, 0, HttpRecvBufSize);
	while(ERR_OK == netconn_recv(httpBuffer->clientconn, &httpBuffer->tcpRecvBuf))
	{
		httpBuffer->recvDataLen += netbuf_copy(httpBuffer->tcpRecvBuf, httpBuffer->recvBuf + httpBuffer->recvDataLen ,
			HttpRecvBufSize - httpBuffer->recvDataLen);
		
		netbuf_delete(httpBuffer->tcpRecvBuf);
	}
	
	httpBuffer->tempP = strstr(httpBuffer->recvBuf, SuccessStr);
	if(httpBuffer->tempP)
		httpBuffer->status = My_Pass;
			
	END1:
		netconn_close(httpBuffer->clientconn);
		netconn_delete(httpBuffer->clientconn);
		return httpBuffer->status;
		
	END2:
		netconn_delete(httpBuffer->clientconn);
		return httpBuffer->status;
}

#elif (UserProgramType == UserXGProgram)

MyRes CommunicateWithLisByLineNet(HttpBuf * httpBuffer)
{
	err_t err;
	
	httpBuffer->status = My_Fail;
	
	IP4_ADDR(&httpBuffer->server_ipaddr, httpBuffer->serverSet.serverIP.ip_1, httpBuffer->serverSet.serverIP.ip_2, 
		httpBuffer->serverSet.serverIP.ip_3, httpBuffer->serverSet.serverIP.ip_4);

	//创建连接
	httpBuffer->clientconn = netconn_new(NETCONN_TCP);
	//创建失败
	if(httpBuffer->clientconn == NULL)
		return httpBuffer->status;

	//绑定本地ip
	err = netconn_bind(httpBuffer->clientconn, IP_ADDR_ANY, 0);
	//连接失败
	if(err != ERR_OK)
		goto END2;

	//尝试连接远程服务器
	err = netconn_connect(httpBuffer->clientconn, &httpBuffer->server_ipaddr, httpBuffer->serverSet.serverPort);
	//连接失败
	if(err != ERR_OK)
		goto END2;
		
	//设置接收数据超时时间100MS
	httpBuffer->clientconn->recv_timeout = 3000;
		
	//step 1, ENQ
	httpBuffer->tempBuf[0] = 0X05;
	httpBuffer->tempBuf[1] = 0X00;
	httpBuffer->recvBuf[0] = 0;
	netconn_write(httpBuffer->clientconn, httpBuffer->tempBuf, 1, NETCONN_COPY );
	if(ERR_OK == netconn_recv(httpBuffer->clientconn, &httpBuffer->tcpRecvBuf))
	{
		netbuf_copy(httpBuffer->tcpRecvBuf, httpBuffer->recvBuf, 1);
		netbuf_delete(httpBuffer->tcpRecvBuf);
		
		if(httpBuffer->recvBuf[0] == 0x06)
			netconn_write(httpBuffer->clientconn, httpBuffer->recvBuf, 1, NETCONN_COPY );
		else
			goto END1;
	}
	else
		goto END1;
	
	//step2, send data
	httpBuffer->recvBuf[0] = 0;
	netconn_write(httpBuffer->clientconn, httpBuffer->sendBuf, httpBuffer->sendDataLen, NETCONN_COPY );
	if(ERR_OK == netconn_recv(httpBuffer->clientconn, &httpBuffer->tcpRecvBuf))
	{
		netbuf_copy(httpBuffer->tcpRecvBuf, httpBuffer->recvBuf, 1);
		netbuf_delete(httpBuffer->tcpRecvBuf);
		
		if(httpBuffer->recvBuf[0] == 0x06)
			netconn_write(httpBuffer->clientconn, httpBuffer->recvBuf, 1, NETCONN_COPY );
		else
			goto END1;
	}
	else
		goto END1;
	
	httpBuffer->status = My_Pass;
			
	END1:
		netconn_close(httpBuffer->clientconn);
		netconn_delete(httpBuffer->clientconn);
		return httpBuffer->status;
		
	END2:
		netconn_delete(httpBuffer->clientconn);
		return httpBuffer->status;
}
#endif
/****************************************end of file************************************************/
