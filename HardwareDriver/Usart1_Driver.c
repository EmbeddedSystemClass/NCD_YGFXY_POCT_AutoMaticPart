/******************************************File Start**********************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include 	"Usart1_Driver.h"
/***************************************************************************************************/
/******************************************Static Variables*****************************************/
/***************************************************************************************************/
static xQueueHandle xRxQueue;									//接收队列
static xQueueHandle xTxQueue;									//发送队列
/***************************************************************************************************/
/******************************************Static Methods*******************************************/
/***************************************************************************************************/
static void Usart1_Os_Init(void);
static void ConfigUsart1(void);
static portBASE_TYPE prvUsart1_ISR_NonNakedBehaviour( void );
/***************************************************************************************************/
/***************************************************************************************************/
/******************************************Main Body************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName：Usart1_Os_Init
*Description：串口1队列互斥量创建
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日13:37:35
***************************************************************************************************/
static void Usart1_Os_Init(void)
{
	xRxQueue = xQueueCreate( xRxQueue1_Len, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
	xTxQueue = xQueueCreate( xTxQueue1_Len, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
}

/***************************************************************************************************
*FunctionName：ConfigUsart1
*Description：串口1的端口初始化
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:58:48
***************************************************************************************************/
static void ConfigUsart1(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	USART_InitStructure.USART_BaudRate   = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************************************************
*FunctionName：Usart1_Init
*Description：串口1初始化，外部调用
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:59:09
***************************************************************************************************/
void Usart1_Init(void)
{
	Usart1_Os_Init();
	ConfigUsart1();
}

/***************************************************************************************************
*FunctionName：USART1_IRQHandler
*Description：串口1中断函数
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:59:24
***************************************************************************************************/
void USART1_IRQHandler(void)
{			
	prvUsart1_ISR_NonNakedBehaviour();
}

/***************************************************************************************************
*FunctionName：prvUsart1_ISR_NonNakedBehaviour
*Description：串口1中断服务函数
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:59:44
***************************************************************************************************/
__attribute__((__noinline__))
static portBASE_TYPE prvUsart1_ISR_NonNakedBehaviour( void )
{
		/* Now we can declare the local variables. */
	signed portCHAR     cChar;
	portBASE_TYPE     xHigherPriorityTaskWoken = pdFALSE;

	portBASE_TYPE retstatus;

	if(USART_GetITStatus(USART1 , USART_IT_TXE) == SET)
	{
		portENTER_CRITICAL();
			retstatus = xQueueReceiveFromISR( xTxQueue, &cChar, &xHigherPriorityTaskWoken );
		portEXIT_CRITICAL();

		if (retstatus == pdTRUE)
			USART_SendData(USART1, cChar);
		else
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	}

	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		cChar = USART_ReceiveData(USART1);

		portENTER_CRITICAL();
			xQueueSendFromISR(xRxQueue, &cChar, &xHigherPriorityTaskWoken);
		portEXIT_CRITICAL();
	}

	return ( xHigherPriorityTaskWoken );
}

/***************************************************************************************************
*FunctionName：EnableUsart1TXInterrupt
*Description：开启一次发送中断（发送队列中数据）
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:18:28
***************************************************************************************************/
void EnableUsart1TXInterrupt(void)
{
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/***************************************************************************************************
*FunctionName：GetUsart1RXQueue, GetUsart1TXQueue,GetUsart1Mutex
*Description：获取串口1的发送接收队列,和队列互斥量
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:22:06
***************************************************************************************************/
xQueueHandle GetUsart1RXQueue(void)
{
	return xRxQueue;
}

xQueueHandle GetUsart1TXQueue(void)
{
	return xTxQueue;
}

/****************************************end of file************************************************/
