#ifndef __XS3_LED_DEV_H
#define __XS3_LED_DEV_H

#define XS3_UART_BUFFER_SIZE 512
#define XS3_UART_NR_MAX_SIZE 3
#define XS3_UART_DRIVER_NAME "Xs3UartDriver"
#define DEV_NAME            "xs3Uart"     /* �豸�� */
/* ���ｫ���ڵ����豸����Ϊ0,�򴮿��豸������ں˶�̬����;��Ҳ��ָ�����ڵ��豸��� */
#define GPRS_UART_MAJOR        200            /* ���豸�� */
#define GPRS_UART_MINOR        0            /* ���豸�� */
#define GPRS_UART_FIFO_SIZE    10           /* ����FIFO�Ĵ�С */
#define RXSTAT_DUMMY_READ    (0x10000000)
#define MAP_SIZE             (0x100)        /* Ҫӳ��Ĵ���IO�ڴ�����С */
#define IRQ_NUM					72
#endif
