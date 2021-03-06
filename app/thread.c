#include "thread.h"

extern int FdLed;
extern int FdTim;
extern int FdUart;

extern pthread_mutex_t MutexTim;
extern pthread_mutex_t MutexLed;
extern pthread_cond_t CondLed;

extern rtu_request_t GlobalRequestMsg;
extern rtu_respond_t GlobalRespondMsg;
extern Tim_t TimData;

void *PthreadLedCtl(void *arg)
{
	int state = LED_ON;
	FdLed = open(XS3_DEV_LED_PATH, O_RDWR);
	
	if(FdLed < 0)
	{
		printf("failed to open led dev\n");
		pthread_exit(NULL);
	}
	else
	{
		printf("open led successfully\n");
	}
	
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
	{
		printf("failed to open tim dev\n");
		pthread_exit(NULL);
	}
	else
	{
		printf("open tim successfully\n");
	}

	read(FdTim, &LastTimData, sizeof(int));

	while(1)
	{
		read(FdTim, &CurrentData, sizeof(int));
		
		pthread_mutex_lock(&MutexTim);
		if((CurrentData - LastTimData) >= TimData)
		{
			pthread_mutex_unlock(&MutexTim);
			pthread_cond_signal(&CondLed);
	
			LastTimData = CurrentData;
		}
		pthread_mutex_unlock(&MutexTim);
	}
	
}

void *PthreadUartCtl(void *arg)
{
	printf("uart dev name is %s\n", XS3_DEV_UART_PATH);
	int count = 0;
	int ret = 0;
	int i = 0;
	char recvBuf[20] = {'\0'};
	FdUart = open(XS3_DEV_UART_PATH, O_RDWR);
	if(FdUart < 0)
	{
		printf("failed to open uart dev\n");
		pthread_exit(NULL);
	}
	else
		printf("open uart successfully\n");

	while(1)
	{
		count = read(FdUart, &recvBuf, sizeof(recvBuf));
		
		if(count > 0)
		{
			printf("count = %d\n", count);
			if(!strncmp(recvBuf, "1", count))
				printf("�յ� 1\n");
		}
	}

#if 0	
	while(1)
	{
		memset(&GlobalRequestMsg, 0, sizeof(rtu_request_t));
		memset(&GlobalRespondMsg, 0, sizeof(rtu_respond_t));

		count = read(FdUart, &GlobalRequestMsg, sizeof(rtu_request_t));

		if(count > 0)
		{

		

			for(i = 0; i < count; i++)
			{
				//printf("recv > %#x ", GlobalRequestMsg.request.data[i]);
				printf("recv>%s\n", GlobalRequestMsg.request.data);
			}
			//printf("\n");

			ret = ModbusDataCheck(&GlobalRequestMsg, count);
		
			ret = ModbusRespondHandle(&GlobalRequestMsg, &GlobalRespondMsg, ret);

			if(ret == Error_NeedTranfer)
			{
				write(FdUart, &GlobalRespondMsg, 5);
			
				for(i = 0; i < 5; i++)
					printf("send > %#x ", GlobalRequestMsg.request.data[i]);
			
				printf("\n");
			}


		}

		
	}
	#endif
}
