#include "pthread.h"

extern int FdLed;
extern int FdTim;
extern int FdUart;

extern pthread_mutex_t MutexTim;
extern pthread_cond_t CondTim;
extern pthread_mutex_t MutexLed;
extern pthread_cond_t CondLed;

extern rtu_request_t GlobalRequestMsg;
extern rtu_respond_t GlobalRespondMsg;


void *PthreadLedCtl(void *arg)
{
	int state = LED_ON;
	FdLed = open(XS3_DEV_LED_PATH, O_RDONLY);
	if(FdLed < 0)
		pthread_exit((void *)FdLed);
	else
		printf("open led successfully\n");
	
	while(1)
	{
		pthread_mutex_lock(&MutexLed);
		pthread_cond_wait(&CondLed, &MutexLed);
		pthread_mutex_unlock(&MutexLed);

		LedCtl(FdLed, state, LED_1);
		state = !state;
	}
}

void *PthreadTimCtl(void *arg)
{
	int LastTimData = 0;
	int CurrentData = 0;
	FdTim = open(XS3_DEV_TIM_PATH, O_RDWR);
	if(FdTim < 0)
		pthread_exit((void *)FdTim);
	else
		printf("open tim successfully\n");

	read(FdTim, &LastTimData, sizeof(int);
	while(1)
	{
		read(FdTim, &CurrentData, sizeof(int));

		pthread_mutex_lock(&MutexTim);
		if(CurrentData - LastTimData >= TimData)
		{
			pthread_mutex_unlock(&MutexTim);
			pthread_cond_signal(&CondTim);

			LastTimData = CurrentData;
		}
	}
	
}

void *PthreadUartCtl(void *arg)
{
	int count = 0;
	int ret = 0;
	FdUart = open(XS3_DEV_UART_PATH, O_RDWR);
	if(FdUart < 0)
		pthread_exit((void *)FdUart);
	else
		printf("open uart successfully\n");

	while(1)
	{
		memset(&GlobalRequestMsg, 0, sizeof(rtu_request_t));
		memset(&GlobalRespondMsg, 0, sizeof(rtu_respond_t));

		count = read(FdUart, &GlobalRequestMsg, sizeof(rtu_request_t);

		ret = ModbusDataCheck(&GlobalRequestMsg, count);
		
		ret = ModbusRespondHandle(&GlobalRequestMsg, &GlobalRespondMsg, ret);

		if(ret == Error_NeedTranfer)
			write(FdUart, &GlobalRespondMsg, 5);
	}
}
