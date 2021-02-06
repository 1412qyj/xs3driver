#include "data_global.h"

pthread_t PidUart;
pthread_t PidLed;
pthread_t PidTim;

int FdLed;
int FdTim;
int FdUart;

pthread_mutex_t MutexTim;
pthread_cond_t CondTim;
pthread_mutex_t MutexLed;

rtu_request_t GlobalRequestMsg;
rtu_respond_t GlobalRespondMsg;

Tim_t TimData;
