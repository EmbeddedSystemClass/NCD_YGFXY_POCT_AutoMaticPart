#ifndef __USART5_H__
#define __USART5_H__

#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"stm32f4xx_usart.h"
#include	"misc.h"
#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#define xRxQueue5_Len				6000
#define xTxQueue5_Len				2048

void Usart5_Init(void);
void EnableUsart5TXInterrupt(void);
xQueueHandle GetUsart5RXQueue(void);
xQueueHandle GetUsart5TXQueue(void);

#endif

