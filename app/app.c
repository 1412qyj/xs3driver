#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "xs3_led_dev.h"

rtu_request_t RequestMsg;
rtu_respond_t RespondMsg;

pthread_t pid_usart;
pthread_t pid_led;

int main(int argc, const char *argv[])
{
	//init
	memset(&RequestMsg, 0, sizeof(rtu_request_t));
	memset(&RespondMsg, 0, sizeof(rtu_respond_t));

	int *ret = NULL;

	//pthread1: ctl usart
	*ret = pthread_create(&pid_usart, NULL, pthread_ctl_usart, NULL);
	if(*ret != 0)
	{
		printf("cannot create pthread_usart\n");
		return -1;
	}

	//pthread2: ctl led
	*ret = pthread_create(&pid_led, NULL, pthread_ctl_led, NULL);
	if(*ret != 0)
	{
		printf("cannnot create pthread_led\n");
		return -1;
	}

	
	pthread_join(pid_usart, &ret);
	pthread_join(pid_led, &ret);

	return 0;
}
