#ifndef __USART1_D_H__
#define __USART1_D_H__

#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"stm32f4xx_usart.h"
#include	"misc.h"

#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#define xRxQueue1_Len				50
#define xTxQueue1_Len				50

void Usart1_Init(void);
void EnableUsart1TXInterrupt(void);
xQueueHandle GetUsart1RXQueue(void);
xQueueHandle GetUsart1TXQueue(void);

#endif

