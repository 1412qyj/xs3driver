#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

#define UART_BASE 0x44e09000
#define UART_RX_PIN	UART_BASE
#define UART_TX_PIN	UART_BASE


struct resource Xs3UartRes[] = {
	[0] = {
		.start = UART_RX_PIN,
		.end = UART_RX_PIN + 7,
		.flag = 
	},
};


