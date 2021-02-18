/*************************************************************************
    > File Name: 01_xs3_led_dev.c
    > Author: Qinyujia 
    > Mail: 1309163979@qq.com 
    > Created Time: 2021å¹?1æœ?29æ—? 12æ—?54åˆ?09ç§?
*************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


/*main dev number and second dev number*/
int Xs3LedMajorNumber = 190;
int Xs3LedMinorNumber = 0;


static int __init Xs3LedInitModule(void)
{
	printk("XS3led module init successfully\n");

	return 0;
}


static void __exit Xs3LedCleanUpModule(void)
{
	printk("Xs3Led module is removed\n");
}

//entrance 
module_init(Xs3LedInitModule);
//exit 
module_exit(Xs3LedCleanUpModule);

//open source 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("qinyujia");
MODULE_DESCRIPTION("test mod");


