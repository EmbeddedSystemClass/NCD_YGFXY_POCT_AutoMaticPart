#ifndef __USART4_H__
#define __USART4_H__

#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"stm32f4xx_usart.h"
#include	"misc.h"
#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#define xRxQueue4_Len				100
#define xTxQueue4_Len				100

void Usart4_Init(void);
void EnableUsart4TXInterrupt(void);
xQueueHandle GetUsart4RXQueue(void);
xQueueHandle GetUsart4TXQueue(void);

#endif

