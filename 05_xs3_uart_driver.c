#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include <asm/gpio.h>
#include <mach/soc.h>
#include <mach/platform.h>
#include <linux/miscdevice.h>

#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/sched.h>


static void uart_flush_to_port(struct work_struct *work)
{
    struct virtual_uart_port *uart_port = container_of(work, struct virtual_uart_port, work);
    int count = 0;

    struct circ_buf *xmit = &uart_port->port.state->xmit;

    if (uart_port->port.x_char)
    {
        uart_port->port.icount.tx++;
        uart_port->port.x_char = 0;
        return;
    }

    if (uart_circ_empty(xmit) || uart_tx_stopped(&uart_port->port)) 
    {
        uart_port->tx_enable_flag = false;
        return;
    }
    
    count = uart_port->port.fifosize;
    do
    {
        printk("0x%hhx ", xmit->buf[xmit->tail]);
        
        xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
        uart_port->port.icount.tx++;
        if (uart_circ_empty(xmit))
            break;
    }while (--count > 0);
    printk("\n");

    if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
    {
        uart_write_wakeup(&uart_port->port);
    }

    printk("%s:%d\n", __FUNCTION__, __LINE__);

    if(uart_circ_empty(xmit))
        uart_port->tx_enable_flag = false;

    
    printk("%s:%d\n", __FUNCTION__, __LINE__);
}


static unsigned int uart_tx_empty(struct uart_port *port)
{
    struct virtual_uart_port *uart_port = container_of(port, struct virtual_uart_port, port);
    struct circ_buf *xmit = &uart_port->port.state->xmit;

    /*在正常的驱动开发中，此处应为串口控制器的fifo是否为空*/
    return uart_circ_empty(xmit);
}

static void uart_stop_tx(struct uart_port *port)
{
    
    struct virtual_uart_port *uart_port = container_of(port, struct virtual_uart_port, port);

    uart_port->tx_enable_flag = false;
}

static void uart_start_tx(struct uart_port *port)
{
    struct virtual_uart_port *uart_port = container_of(port, struct virtual_uart_port, port);
    unsigned long flags = 0;


    spin_lock_irqsave(&uart_port->write_lock, flags);

    uart_port->tx_enable_flag = true;
    spin_unlock_irqrestore(&uart_port->write_lock, flags);
    schedule_work(&uart_port->work);
    
    printk("%s:%d\n", __FUNCTION__, __LINE__);
}
static void uart_stop_rx(struct uart_port *port)
{
    struct virtual_uart_port *uart_port = container_of(port, struct virtual_uart_port, port);
    unsigned long flags = 0;

    spin_lock_irqsave(&uart_port->write_lock, flags);
    uart_port->rx_enable_flag = false;
    spin_unlock_irqrestore(&uart_port->write_lock, flags);
}
static void uart_throttle(struct uart_port *port)
{

}
static void uart_unthrottle(struct uart_port *port)
{


}
static void uart_enable_ms(struct uart_port *port)
{


}
static void uart_break_ctl(struct uart_port *port, int break_state)
{

}

static int uart_startup(struct uart_port *port)
{
    struct virtual_uart_port *uart_port = container_of(port, struct virtual_uart_port, port);
    unsigned long flags = 0;

    spin_lock_irqsave(&uart_port->write_lock, flags);
    uart_port->rx_enable_flag = true;
    uart_port->tx_enable_flag = true;
    spin_unlock_irqrestore(&uart_port->write_lock, flags);

    return 0;
}

static void uart_shutdown(struct uart_port *port)
{
    printk("%s:%d \n", __FUNCTION__, __LINE__);
}
static void uart_set_termios(struct uart_port *port, struct ktermios *termios,
            struct ktermios *old)
{
    unsigned char cval = 0;
    unsigned int baud = 0;

    switch (termios->c_cflag & CSIZE)
    {
        case CS5:
            cval = UART_LCR_WLEN5;
            break;
        case CS6:
            cval = UART_LCR_WLEN6;
            break;
        case CS7:
            cval = UART_LCR_WLEN7;
            break;
        default:
        case CS8:
            cval = UART_LCR_WLEN8;
            break;
    }

    if (termios->c_cflag & CSTOPB)
        cval |= UART_LCR_STOP;
    if (termios->c_cflag & PARENB)
        cval |= UART_LCR_PARITY;
    if (!(termios->c_cflag & PARODD))
        cval |= UART_LCR_EPAR;



    baud = uart_get_baud_rate(port, termios, old, VIRTUAL_UART_MIN_SPEED, VIRTUAL_UART_MAX_SPEED);
    printk("baud = %d cval = %d\n", baud, cval);
}
static const char * uart_type(struct uart_port *port)
{
    struct virtual_uart_port *uart_port = container_of(port, struct virtual_uart_port, port);

    return uart_port->uart_type;
}
static void uart_release_port(struct uart_port *port)
{
    printk("%s:%d\n", __FUNCTION__, __LINE__);
}
static void uart_set_mctrl(struct uart_port *uart_port, unsigned int mctrl)
{
    printk("%s:%d\n", __FUNCTION__, __LINE__);
}
static unsigned int uart_get_mctrl(struct uart_port *uart_port)
{

    printk("%s:%d\n", __FUNCTION__, __LINE__);

    return 0;
}


static struct uart_ops uart_ops = {
    .tx_empty	= uart_tx_empty,
    .stop_tx	= uart_stop_tx,
    .start_tx	= uart_start_tx,
    .throttle	= uart_throttle,
    .unthrottle	= uart_unthrottle,
    .stop_rx	= uart_stop_rx,
    .enable_ms	= uart_enable_ms,
    .break_ctl	= uart_break_ctl,
    .startup	= uart_startup,
    .shutdown	= uart_shutdown,
    .set_termios	= uart_set_termios,
    .type   = uart_type,
    .release_port = virtual_uart_release_port,
    .set_mctrl = virtual_uart_set_mctrl,
    .get_mctrl = virtual_uart_get_mctrl
};



int Xs3UartDriverProbe(struct platform_device *pPlatFormDevice)
{
	printk("Xs3UartDriverProbe\n");

	return 0;
}

int Xs3UartDriverRemove(struct platform_device *pPlatFormDevice)
{
	printk("Xs3UartDriverRemove\n");
	
	return 0;
}


static struct platform_driver Xs3PlatformDriver = {
    .driver = {
        .name = "Xs3UartDevice",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(uart_of_match),
    },
    .probe = Xs3UartDriverProbe,
    .remove = Xs3UartDriverRemove,
};


static struct uart_driver Xs3UartDriver = {
    .owner		= THIS_MODULE,
    .driver_name	= XS3_UART_NAME,
    .dev_name	= XS3_UART_DRIVER_NAME,
    .nr		= XS3_UART_DEVICE_SUPPORT_MAX,
    .cons		= NULL,//not support
};

static int __init Xs3UartInit(void)
{
	printk("Xs3UartInit\n");

	int ret = 0;

    ret = uart_register_driver(&Xs3UartDriver);
    if (ret != 0)
        return ret;


    ret = platform_driver_register(&Xs3PlatformDriver);
    if(ret != 0)
        uart_unregister_driver(&Xs3UartDriver);

   	return ret;	
}

static void __exit Xs3UartExit(void)
{
	printk(KERN_ALERT "Xs3UartExit\n");
	platform_driver_unregister(&Xs3PlatformDriver);
    uart_unregister_driver(&Xs3UartDriver); 
}

//entrance 
module_init(Xs3UartInit);
//exit 
module_exit(Xs3UartExit);


//open source 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("qinyujia");
MODULE_DESCRIPTION("use for xinje Xs3UartDriver");


