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
	ioctl(fd, XS3_LED_ON, ledNum);
	printf("led %d on\n", ledNum);
	return 0;
}

static int LedOff(int fd, int ledNum)
{
	ioctl(fd, XS3_LED_OFF, ledNum);
	printf("led %d off\n", ledNum);
	return 0;
}
