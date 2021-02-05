#ifndef __XS3_LED_DEV_H
#define __XS3_LED_DEV_H

#define XS3_UART_BUFFER_SIZE 512
#define XS3_UART_NR_MAX_SIZE 3
#define XS3_UART_DRIVER_NAME "Xs3UartDriver"
#define DEV_NAME            "xs3Uart"     /* 设备名 */
/* 这里将串口的主设备号设为0,则串口设备编号由内核动态分配;你也可指定串口的设备编号 */
#define GPRS_UART_MAJOR        200            /* 主设备号 */
#define GPRS_UART_MINOR        0            /* 次设备号 */
#define GPRS_UART_FIFO_SIZE    10           /* 串口FIFO的大小 */
#define RXSTAT_DUMMY_READ    (0x10000000)
#define MAP_SIZE             (0x100)        /* 要映射的串口IO内存区大小 */
#define IRQ_NUM					72
#endif
