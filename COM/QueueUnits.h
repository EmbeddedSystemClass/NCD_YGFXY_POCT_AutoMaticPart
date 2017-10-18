#ifndef _QUEUE_U_H__
#define _QUEUE_U_H__

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"
#include	"Define.h"

MyRes ReceiveDataFromQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *receivedstr, unsigned short len, unsigned short * readSize, 
	unsigned short itemsize, portTickType queueBlockTime, portTickType mutexBlockTime);
MyRes SendDataToQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *sendstr, unsigned short len, unsigned short itemsize, 
	portTickType queueBlockTime, portTickType mutexBlockTime, void (*fun)(void));
#endif

