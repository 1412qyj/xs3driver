#include "led.h"

static int LedOn(int fd, int ledNum);
static int LedOff(int fd, int ledNum);

int LedCtl(int fd, int opt, int ledNum)
{
	switch(opt)
	{
		case LED_ON:
			LedOn(fd, ledNum);
			break;
		case LED_OFF:
			LedOff(fd, ledNum);
			break;
	}
	
	return 0;
}

static int LedOn(int fd, int ledNum)
{
	if(ioctl(fd, XS3_LED_ON, ledNum) == 0)
		printf("led %d on\n", ledNum);
	else
		printf("led %d no reaction\n", ledNum);

	return 0;
}

static int LedOff(int fd, int ledNum)
{
	if(ioctl(fd, XS3_LED_OFF, ledNum) == 0)
		printf("led %d off\n", ledNum);
	else
		printf("led %d no reaction\n", ledNum);
	
	return 0;
}
