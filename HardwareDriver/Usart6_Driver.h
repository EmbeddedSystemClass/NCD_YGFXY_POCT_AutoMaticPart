#ifndef __USART6_H__
#define __USART6_H__

#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"
#include	"stm32f4xx_usart.h"
#include	"misc.h"
#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#define xRxQueue6_Len				200
#define xTxQueue6_Len				300

void Usart6_Init(void);
void EnableUsart6TXInterrupt(void);
xQueueHandle GetUsart6RXQueue(void);
xQueueHandle GetUsart6TXQueue(void);
xSemaphoreHandle GetUsart6RXMutex(void);
xSemaphoreHandle GetUsart6TXMutex(void);

#endif

