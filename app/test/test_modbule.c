/*************************************************************************
  > File Name: test_modbule.c
  > Author: Qinyujia 
  > Mail: 1309163979@qq.com 
  > Created Time: 2021年02月18日 09时36分44秒
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
	int fd = open("/dev/xs3led", O_RDWR);
	if(fd < 0)
		printf("open xs3led failed\n");

	return 0;
}

