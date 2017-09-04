/******************************************File Start**********************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include 	"Usart6_Driver.h"
#include	"Led_Driver.h"
#include	"Delay.h"
/***************************************************************************************************/
/******************************************Static Variables*****************************************/
/***************************************************************************************************/
static xQueueHandle xRxQueue = NULL;									//receive queue
static xQueueHandle xTxQueue = NULL;									//send queue
static xSemaphoreHandle xRxMutex;									//互斥量
static xSemaphoreHandle xTxMutex;									//互斥量
/***************************************************************************************************/
/******************************************Static Methods*******************************************/
/***************************************************************************************************/
static void prvUsart6_ISR_NonNakedBehaviour( void );
/***************************************************************************************************/
/***************************************************************************************************/
/******************************************Main Body************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName：ConfigUsart6
*Description：串口6的端口初始化和配置
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:28:25
***************************************************************************************************/
void Usart6_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

	USART_InitStructure.USART_BaudRate   = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART6, &USART_InitStructure);

	USART_Cmd(USART6, ENABLE);

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	xRxQueue = xQueueCreate( xRxQueue6_Len, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
	xTxQueue = xQueueCreate( xTxQueue6_Len, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
	vSemaphoreCreateBinary(xRxMutex);
	vSemaphoreCreateBinary(xTxMutex);
}

/***************************************************************************************************
*FunctionName：USART6_IRQHandler
*Description：串口6的中断函数
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:29:13
***************************************************************************************************/
void USART6_IRQHandler(void)
{			
	prvUsart6_ISR_NonNakedBehaviour();
}

/***************************************************************************************************
*FunctionName：prvUsart6_ISR_NonNakedBehaviour
*Description：串口6的中断服务函数
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:29:32
***************************************************************************************************/
__attribute__((__noinline__))
static void prvUsart6_ISR_NonNakedBehaviour( void )
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	char cChar;

	if( USART_GetITStatus( USART6, USART_IT_TXE ) == SET )
	{
		/* The interrupt was caused by the THR becoming empty.  Are there any
		more characters to transmit? */
		if( xQueueReceiveFromISR( xTxQueue, &cChar, &xHigherPriorityTaskWoken ) == pdTRUE )
		{
			/* A character was retrieved from the queue so can be sent to the
			THR now. */
			USART_SendData( USART6, cChar );
		}
		else
		{
			USART_ITConfig( USART6, USART_IT_TXE, DISABLE );		
		}		
	}
	
	if( USART_GetITStatus( USART6, USART_IT_RXNE ) == SET )
	{
		cChar = USART_ReceiveData( USART6 );
		xQueueSendFromISR( xRxQueue, &cChar, &xHigherPriorityTaskWoken );
	}	
	
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}

/***************************************************************************************************
*FunctionName：EnableUsart6TXInterrupt
*Description：开启一次发送中断（发送队列中数据）
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:18:28
***************************************************************************************************/
void EnableUsart6TXInterrupt(void)
{
	USART_ITConfig(USART6, USART_IT_TXE, ENABLE);
}

void Usart6SendData(unsigned char data)
{
	USART_SendData(USART6, data);
}

/***************************************************************************************************
*FunctionName：GetUsart6RXQueue, GetUsart6TXQueue,GetUsart6Mutex
*Description：获取串口6的发送接收队列,和队列互斥量
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:22:06
***************************************************************************************************/
xQueueHandle GetUsart6RXQueue(void)
{
	return xRxQueue;
}

xQueueHandle GetUsart6TXQueue(void)
{
	return xTxQueue;
}
xSemaphoreHandle GetUsart6RxMutex(void)
{
	return xRxMutex;
}
xSemaphoreHandle GetUsart6TxMutex(void)
{
	return xTxMutex;
}
/****************************************end of file************************************************/
