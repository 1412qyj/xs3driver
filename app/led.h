#ifndef __LED_H
#define __LED_H

#include "data_global.h"

#define XS3_LED_MAGIC 'q'
#define XS3_LED_ON  _IOW(XS3_LED_MAGIC, 1, unsigned long)
#define XS3_LED_OFF _IOW(XS3_LED_MAGIC, 0, unsigned long)

int LedCtl(int fd, int opt, int ledNum);

#endif
