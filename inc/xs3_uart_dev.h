#ifndef __XS3_UART_DEV_H
#define __XS3_UART_DEV_H


#define GPIO0_BASE_ADDR							(0x44E07000)
#define GPIO0_SETDATOUT_OFFSET					(0x0194)


/*uart 0*/
#define UART0_GPIO_RX
#define UART0_GPIO_TX


/*uart 1*/
#define UART1_GPIO_RX 							GPIO0_BASE_ADDR + GPIO0_SETDATOUT_OFFSET //D16 gpio0_14
#define UART1_GPIO_TX 							GPIO0_BASE_ADDR + GPIO0_SETDATOUT_OFFSET //D15 gpio0_15
#define UART1_IRQ_NUM							(73)
#define UART1_BASE_START						(0x48022000)
#define UART1_BASE_END							(0x48022FFF)

#define XS3_UART_DEVICE_SUPPORT_MAX				2
#define XS3_UART_NAME							"Xs3Uart"
#define XS3_UART_DRIVER_NAME					"Xs3UartDriver"
#define XS3_UART_BAUD							115200
#define XS3_UART_BUFFER_SIZE 					512
#define DEV_NAME            					"xs3Uart"     /* 设备名 */
#define UART_MAJOR        						200            /* 主设备号 */
#define UART_MINOR       						0           /* 次设备号 */
#define UART_FIFO_SIZE    						10           /* 串口FIFO的大小 */
#define RXSTAT_DUMMY_READ    					(0x10000000)
#define MAP_SIZE             					(0x100)       /* 要映射的串口IO内存区大小 */
#define UART_ENABLE_IT_OFFSET					(0x4) //0->RX IT || 1->TX IT
#define UART_RX_HOLDING_OFFSET					(0)
#define UART_TX_HOLDING_OFFSET					(0)
#define UART_DATA_LENGTH_OFFSET					(0x48)
#define UART_IER_OFFSET							(0x4) //1->THR enable
#define UART_MDR_OFFSET							(0x20)//[0:2]
#define UART_LDR_OFFSET							(0xc) //0xBF
#define UART_EFR_OFFSET							(0x8) //0x10
#define UART_TXFLL_OFFSET						(0x28) //
#define UART_TXFLH_OFFSET						(0x2c)
#define UART_RXFLL_OFFSET						(0x30) //
#define UART_RXFLH_OFFSET						(0x34)
 
#define UART_IRQ_NUM							UART1_IRQ_NUM
#define UART_GPIO_RX_ADDR						UART1_GPIO_RX
#define UART_GPIO_TX_ADDR						UART1_GPIO_TX
#define UART_RX_HOLDING_ADDR					UART1_BASE_START + UART_RX_HOLDING_OFFSET
#define UART_TX_HOLDING_ADDR					UART1_BASE_START + UART_TX_HOLDING_OFFSET
#define UART_ENABLE_IT_ADDR						UART1_BASE_START + UART_ENABLE_IT_OFFSET
#define UART_DATA_LENGTH_ADDR					UART1_BASE_START + UART_DATA_LENGTH_OFFSET
#define UART_IER_ADDR							UART1_BASE_START + UART_IER_OFFSET
#define UART_MDR_ADDR							UART1_BASE_START + UART_MDR_OFFSET
#define UART_LDR_ADDR							UART1_BASE_START + UART_LDR_OFFSET
#define UART_EFR_ADDR							UART1_BASE_START + UART_EFR_OFFSET

struct UartData_t{
	unsigned int irqNum;
};

#endif