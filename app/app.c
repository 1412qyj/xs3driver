#include "data_global.h"
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

extern pthread_t PidUart;
extern pthread_t PidLed;
extern pthread_t PidTim;

static void Init(void);

int main(int argc, const char *argv[])
{
	Init();

	pthread_create(&PidUart, NULL, PthreadUartCtl, NULL);
	pthread_create(&PidLed, NULL, PthreadLedCtl, NULL);
	pthread_create(&PidTim, NULL, PthreadTimCtl, NULL);

	pthread_join(PidUart, NULL);
	pthread_join(PidTim, NULL);
	pthread_join(PidLed, NULL);
	
	return 0;
}

static void Init(void)
{
	TimData = 1;

	memset(&GlobalRequestMsg, 0, sizeof(rtu_request_t));
	memset(&GlobalRespondMsg, 0, sizeof(rtu_respond_t));

	pthread_mutex_init(&MutexLed, NULL);
	pthread_mutex_init(&MutexTim, NULL);
	pthread_cond_init(&CondLed, NULL);
}
