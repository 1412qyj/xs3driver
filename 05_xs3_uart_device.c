#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <mach/platform.h>

#include "./inc/xs3_uart_dev.h"

struct UartData_t UartData = {
	.irqNum = UART_IRQ_NUM,
};


struct resource Xs3UartRes[] = {
	[0] = {
		.start = UART_MDR_ADDR,
		.end = UART_MDR_ADDR + 7,
		.name = "UART_MDR_ADDR",
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = UART_LDR_ADDR,
		.end = UART_LDR_ADDR + 7,
		.name = "UART_LDR_ADDR",
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = UART_EFR_ADDR,
		.end = UART_EFR_ADDR + 7,
		.name = "UART_EFR_ADDR",
		.flags = IORESOURCE_MEM,
	},
	
	
};

static void Xs3UartDeviceRelease(struct device *dev)
{
	printk("Uart Device is Released---\n");
}


//�����ʼ��platform_device
struct platform_device Xs3UartDevic = {
	.name = "Xs3UartDevice",
	.id = -1,
	.resource = Xs3UartRes,
	.num_resources = ARRAY_SIZE(Xs3UartRes),
	.dev = {
		//.platform_data = ,//��������
		.release = Xs3UartDeviceRelease,
	},
};


//���غ���
int PlatformDeviceInit(void)
{
	printk("Uart Device is going to be matched ---\n");
	return platform_device_register(&Xs3UartDevic);
}

//ж�غ���
void PlatformDeviceExit(void)
{
	printk("Uart Device is removed---\n");
	platform_device_unregister(&Xs3UartDevic);
}


//����Ϊģ�����ںͳ���
module_init(PlatformDeviceInit);
module_exit(PlatformDeviceExit);


//open source 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("qinyujia");
MODULE_DESCRIPTION("use for xinje Xs3Uartdevice");

