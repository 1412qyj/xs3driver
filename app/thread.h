#ifndef __PTHREAD_H
#define __PTHREAD_H

#include "data_global.h"
#include "modbus.h"
#include "led.h"

#include <pthread.h>

void *PthreadLedCtl(void *arg);
void *PthreadTimCtl(void *arg);
void *PthreadUartCtl(void *arg);


#endif
