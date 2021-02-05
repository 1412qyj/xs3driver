/*global vals used by the project*/

rtu_request_t RequestMsg;
rtu_respond_t RespondMsg;

int fd_led;
int fd_tim;
int fd_usart;

pthread_t pid_usart;
pthread_t pid_led;
pthread_t pid_tim;

pthread_mutex_t MutexTim;
pthread_cond_t CondTim;
pthread_mutex_t MutexLed;
pthread_cond_t CondLed;

typedef unsigned char u8;
u8 TimData;

