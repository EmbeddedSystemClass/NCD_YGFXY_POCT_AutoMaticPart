/******************************************File Start**********************************************
*FileName: 
*Description: 
*Author:xsx
*Data: 
****************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include 	"Usart3_Driver.h"

#include 	"stdio.h"
/***************************************************************************************************/
/******************************************Static Variables*****************************************/
/***************************************************************************************************/
static xQueueHandle xRxQueue;									//接收队列
static xQueueHandle xTxQueue;									//发送队列
/***************************************************************************************************/
/******************************************Static Methods*******************************************/
/***************************************************************************************************/
static void Usart3_Os_Init(void);
static void ConfigUsart3(void);
static portBASE_TYPE prvUsart3_ISR_NonNakedBehaviour( void );
/***************************************************************************************************/
/***************************************************************************************************/
/******************************************Main Body************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName：Usart3_Os_Init
*Description：串口3的队列互斥量创建
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:50:51
***************************************************************************************************/
static void Usart3_Os_Init(void)
{
	xRxQueue = xQueueCreate( xRxQueue3_Len, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
	xTxQueue = xQueueCreate( xTxQueue3_Len, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
}

/***************************************************************************************************
*FunctionName：ConfigUsart3
*Description：串口3的端口初始化和配置
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:52:08
***************************************************************************************************/
static void ConfigUsart3(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	USART_InitStructure.USART_BaudRate   = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART3, &USART_InitStructure);

	USART_Cmd(USART3, ENABLE);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************************************************
*FunctionName：Usart3_Init
*Description：串口3外部调用初始化函数
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:28:56
***************************************************************************************************/
void Usart3_Init(void)
{
	Usart3_Os_Init();
	ConfigUsart3();
}

/***************************************************************************************************
*FunctionName：USART3_IRQHandler
*Description：串口3的中断函数
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:29:13
***************************************************************************************************/
void USART3_IRQHandler(void)
{			
	prvUsart3_ISR_NonNakedBehaviour();
}

/***************************************************************************************************
*FunctionName：prvUsart3_ISR_NonNakedBehaviour
*Description：串口3的中断服务函数
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:29:32
***************************************************************************************************/
__attribute__((__noinline__))
static portBASE_TYPE prvUsart3_ISR_NonNakedBehaviour( void )
{
	signed portCHAR     cChar;
	portBASE_TYPE     xHigherPriorityTaskWoken = pdFALSE;

	portBASE_TYPE retstatus;

	if(USART_GetITStatus(USART3 , USART_IT_TXE) == SET)
	{
		portENTER_CRITICAL();
			retstatus = xQueueReceiveFromISR( xTxQueue, &cChar, &xHigherPriorityTaskWoken );
		portEXIT_CRITICAL();

		if (retstatus == pdTRUE)
			USART_SendData(USART3, cChar);
		else
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	}

	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		cChar = USART_ReceiveData(USART3);

		portENTER_CRITICAL();
			xQueueSendFromISR(xRxQueue, &cChar, &xHigherPriorityTaskWoken);
		portEXIT_CRITICAL();
	}
	
	return ( xHigherPriorityTaskWoken );
}

/***************************************************************************************************
*FunctionName：EnableUsart6TXInterrupt
*Description：开启一次发送中断（发送队列中数据）
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:18:28
***************************************************************************************************/
void EnableUsart3TXInterrupt(void)
{
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

/***************************************************************************************************
*FunctionName：GetUsart3RXQueue, GetUsart3TXQueue,GetUsart3Mutex
*Description：获取串口3的发送接收队列,和队列互斥量
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:22:06
***************************************************************************************************/
xQueueHandle GetUsart3RXQueue(void)
{
	return xRxQueue;
}

xQueueHandle GetUsart3TXQueue(void)
{
	return xTxQueue;
}

int fputc(int ch, FILE *f)
{
	// Place your implementation of fputc here 
	// e.g. write a character to the USART 
	USART_SendData(USART3, (uint8_t) ch);

	// Loop until the end of transmission 
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
	{;}

	return ch;
}
/****************************************end of file************************************************/
