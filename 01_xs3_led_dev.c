/*************************************************************************
    > File Name: 01_xs3_led_dev.c
    > Author: Qinyujia 
    > Mail: 1309163979@qq.com 
    > Created Time: 2021å¹?1æœ?29æ—? 12æ—?54åˆ?09ç§?
*************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_gpio.h>

#include "./inc/xs3_led_dev.h"

/*main dev number and second dev number*/
int Xs3LedMajorNumber = 190;
int Xs3LedMinorNumber = 0;

/*led dev structor*/
struct Xs3LedObject{
	struct cdev cdev;

	volatile unsigned long *pLED1_GPIODAT;
	volatile unsigned long *pLED2_GPIODAT;
	volatile unsigned long *pLED3_GPIODAT;

	volatile unsigned long *pLED1_GPIOCON;
	volatile unsigned long *pLED2_GPIOCON;
	volatile unsigned long *pLED3_GPIOCON;
};

/*definity a led example*/
struct Xs3LedObject *pGlobalXs3Led = NULL;


void Xs3LedConfig(struct Xs3LedObject *pXs3Led)
{
	writel(readl(pXs3Led->pLED1_GPIOCON) | (0x1 << 20), pXs3Led->pLED1_GPIOCON);
	writel(readl(pXs3Led->pLED2_GPIOCON) | (0x1 << 9), pXs3Led->pLED2_GPIOCON);
	writel(readl(pXs3Led->pLED3_GPIOCON) | (0x1 << 8), pXs3Led->pLED3_GPIOCON);

	writel(readl(pXs3Led->pLED1_GPIODAT) & ~(0x1 << 10), pXs3Led->pLED1_GPIODAT);
	writel(readl(pXs3Led->pLED2_GPIODAT) & ~(0x1 << 9), pXs3Led->pLED2_GPIODAT);
	writel(readl(pXs3Led->pLED3_GPIODAT) & ~(0x1 << 8), pXs3Led->pLED3_GPIODAT);
}

//the arguments from the system_call
int Xs3LedOpen(struct inode *pnode, struct file *pfile)
{
	//important !!!
	pfile->private_data = container_of(pnode->i_cdev, struct Xs3LedObject, cdev);	

	//print Debug msg in system msgList
	printk(KERN_DEBUG"Xs3LedOpenFunc is called\n");

	return 0;
}


/*
fun:	open led function 
arg:
		pXs3Led-> led on xs3
		num-> which led on
ret:	void
*/
void Xs3LedOn(struct Xs3LedObject *pXs3Led, unsigned long num)
{
	switch(num)
	{
		case 1:
		writel((readl(pXs3Led->pLED1_GPIODAT) | (0x1 << 20)),pXs3Led->pLED1_GPIODAT);	
		break;
		case 2:
		writel((readl(pXs3Led->pLED2_GPIODAT) | (0x1 << 9)),pXs3Led->pLED2_GPIODAT);
		break;
		case 3:
		writel((readl(pXs3Led->pLED3_GPIODAT) | (0x1 << 8)),pXs3Led->pLED3_GPIODAT);
		break;
	}	
}

void Xs3LedOff(struct Xs3LedObject *pXs3Led, unsigned long num)
{
	switch(num)
	{
		case 1:
		writel((readl(pXs3Led->pLED1_GPIODAT) & ~(0x1 << 20)),pXs3Led->pLED1_GPIODAT);	
		break;
		case 2:
		writel((readl(pXs3Led->pLED2_GPIODAT) & ~(0x1 << 9)),pXs3Led->pLED2_GPIODAT);
		break;
		case 3:
		writel((readl(pXs3Led->pLED3_GPIODAT) & ~(0x1 << 8)),pXs3Led->pLED3_GPIODAT);
		break;
	}
}


long Xs3LedIoCtl(struct file *pfile, unsigned int cmd, unsigned long arg)
{
	struct Xs3LedObject *pXs3Led = (struct Xs3LedObject *)pfile->private_data;

	if(arg < 1 || arg > 3)
	{
		return -1;
	}

	switch(cmd)
	{
		case XS3_LED_ON:
		Xs3LedOn(pXs3Led, arg);
		break;
		case XS3_LED_OFF:
		Xs3LedOff(pXs3Led, arg);
		break;
		default:
		break;
	}

	return 0;
}


/*
fun:	release led dev func, where you use systemcall(close), this func will be called
arg:
		pnode->	
		pfile->

ret:	0, always succeed
*/
int Xs3LedRelease(struct inode *pnode, struct file *pfile)
{
	printk("Xs3LedRelease is called\n");

	return 0;
}

void Xs3LedAddrRemap(struct Xs3LedObject *pXs3Led)
{
	pXs3Led->pLED1_GPIOCON = ioremap(XS3_LED_1_GPCON,4);	
	pXs3Led->pLED1_GPIODAT = ioremap(XS3_LED_1_GPDAT,4);	
	pXs3Led->pLED2_GPIOCON = ioremap(XS3_LED_2_GPCON,4);	
	pXs3Led->pLED2_GPIODAT = ioremap(XS3_LED_2_GPDAT,4);	
	pXs3Led->pLED3_GPIOCON = ioremap(XS3_LED_3_GPCON,4);	
	pXs3Led->pLED3_GPIODAT = ioremap(XS3_LED_3_GPDAT,4);
}

void Xs3LedAddrUnremap(struct Xs3LedObject *pXs3Led)
{
	iounmap(pXs3Led->pLED1_GPIOCON);
	iounmap(pXs3Led->pLED1_GPIODAT);
	iounmap(pXs3Led->pLED2_GPIOCON);
	iounmap(pXs3Led->pLED2_GPIODAT);
	iounmap(pXs3Led->pLED3_GPIOCON);
	iounmap(pXs3Led->pLED3_GPIODAT);
}


struct file_operations Xs3LedOpts ={
	.owner = THIS_MODULE,
	.open = Xs3LedOpen,
	.release = Xs3LedRelease,
	.unlocked_ioctl = Xs3LedIoCtl,
	//....
};


static int __init Xs3LedInitModule(void)
{
	int ret = 0;	

	/*make a dev num*/
	dev_t dev_num = MKDEV(Xs3LedMajorNumber, Xs3LedMinorNumber);
	ret = register_chrdev_region(dev_num, 1, "Xs3Led");
	if(ret)
	{
		ret = alloc_chrdev_region(&dev_num, Xs3LedMinorNumber, 1, "Xs3Led");

		if(ret)
		{
			printk(KERN_ALERT"alloc Xs3LedDevNumber failed\n");
			return -1;
		}

		Xs3LedMajorNumber = MAJOR(dev_num);
	}
	dev_num = MKDEV(Xs3LedMajorNumber, Xs3LedMinorNumber);

	/*malloc a led dev msg*/
	pGlobalXs3Led = (struct Xs3LedObject *)kmalloc(sizeof(struct Xs3LedObject), GFP_KERNEL);
	if(NULL == pGlobalXs3Led)
	{
		return -1;
	}
	memset(pGlobalXs3Led, 0, sizeof(struct Xs3LedObject));


	/*config led gpio*/
	Xs3LedAddrRemap(pGlobalXs3Led);
	Xs3LedConfig(pGlobalXs3Led);

	//dev init
	cdev_init(&pGlobalXs3Led->cdev, &Xs3LedOpts);

	//always config this
	pGlobalXs3Led->cdev.owner = THIS_MODULE;

	cdev_add(&pGlobalXs3Led->cdev, dev_num, 1);

	printk("XS3led module init successfully\n");

	return 0;
}


static void __exit Xs3LedCleanUpModule(void)
{
	dev_t dev_num = MKDEV(Xs3LedMajorNumber, Xs3LedMinorNumber);

	unregister_chrdev_region(dev_num, 1);

	cdev_del(&pGlobalXs3Led->cdev);
#
	//get back gpio to kernel
	Xs3LedAddrUnremap(pGlobalXs3Led);

	kfree(pGlobalXs3Led);
	pGlobalXs3Led = NULL;

	printk("Xs3Led module is removed\n");
}

//entrance 
module_init(Xs3LedInitModule);
//exit 
module_exit(Xs3LedCleanUpModule);

//open source 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("qinyujia");
MODULE_DESCRIPTION("use for xinje Xs3Led");


