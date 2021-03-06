#ifndef __USART2_H__
#define __USART2_H__

#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"stm32f4xx_usart.h"
#include	"misc.h"
#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#define xRxQueue2_Len				50
#define xTxQueue2_Len				50

void Usart2_Init(void);
void EnableUsart2TXInterrupt(void);
xQueueHandle GetUsart2RXQueue(void);
xQueueHandle GetUsart2TXQueue(void);

#endif

