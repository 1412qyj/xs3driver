#include "led.h"


void led_on(int num, int fd)
{
	switch(num)
	{
		case 1:
			ioctl(fd, XS3_LED_ON, num);
			break;
		case 2:
			ioctl(fd, XS3_LED_ON, num);
			break;
		case 3:
			ioctl(fd, XS3_LED_ON, num);
			break;

	}

}

void led_off(int num, int fd)
{
	switch(num)
	{
		case 1:
			ioctl(fd, XS3_LED_OFF, num);
			break;
		case 2:
			ioctl(fd, XS3_LED_OFF, num);
			break;
		case 3:
			ioctl(fd, XS3_LED_OFF, num);
			break;
	}

}
