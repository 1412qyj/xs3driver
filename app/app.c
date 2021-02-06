#include "data_global.h"
#include "pthread.h"

extern int FdLed;
extern int FdTim;
extern int FdUart;

extern pthread_mutex_t MutexTim;
extern pthread_mutex_t MutexLed;
extern pthread_cond_t CondLed;

extern rtu_request_t GlobalRequestMsg;
extern rtu_respond_t GlobalRespondMsg;

extern Tim_t TimData;

extern pthread_t PidUart;
extern pthread_t PidLed;
extern pthread_t PidTim;


int main(int argc, const char *argv[])
{
	int pidUartReval = 0;
	int pidLedReval = 0;
	int pidTimReval = 0;

	Init();

	pthread_create(&PidUart, NULL, PthreadUartCtl, NULL);
	pthread_create(&PidLed, NULL, PthreadLedCtl, NULL);
	pthread_create(&PidTim, NULL, PthreadTimCtl, NULL);

	pthread_join(PidUart, &pidUartReval);
	pthread_join(PidTim, &pidLedReval);
	pthread_join(PidLed, &pidTimReval);
	
	return 0;
}

static void Init(void)
{
	TimData = 1;

	memset(GlobalRequestMsg, 0, sizeof(rtu_request_t));
	memset(GlobalRespondMsg, 0, sizeof(rtu_respond_t));

	pthread_mutex_init(&MutexLed);
	pthread_mutex_init(&MutexTim);
	pthread_cond_init(&CondLed);
}
