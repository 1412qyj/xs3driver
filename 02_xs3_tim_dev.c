#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/atomic.h>
#include <linux/timer.h>
#include <linux/slab.h>


int mymajor = 188;
int myminor = 0;
#define TIM_INC 1

struct secdev
{
	struct cdev cdev;

	atomic_t secnum;
	
	struct timer_list tm;

	struct class *pclass;
	struct device *pdevice;
};

struct secdev *pgsecdev = NULL;

void timer_handler(unsigned long data)
{
	struct secdev *pmydev = (struct secdev *)data;

	atomic_inc(&pmydev->secnum);

	mod_timer(&pmydev->tm,jiffies + TIM_INC * HZ);
}

int secdev_open(struct inode *pnode,struct file *pfile)
{
	struct secdev *pmydev = NULL;
	pfile->private_data = container_of(pnode->i_cdev,struct secdev,cdev);

	pmydev = (struct secdev *)pfile->private_data;

	pmydev->tm.expires = jiffies + TIM_INC * HZ;
	pmydev->tm.function = timer_handler;
	pmydev->tm.data = (unsigned long)pmydev;

	add_timer(&pmydev->tm);	
	
	return 0;
}

int secdev_close(struct inode *pnode,struct file *pfile)
{
	struct secdev *pmydev = (struct secdev *)pfile->private_data;

	del_timer(&pmydev->tm);

	atomic_set(&pmydev->secnum,0);
	return 0;
}

ssize_t secdev_write(struct file *pfile,const char __user *puser,size_t size,loff_t *poff)
{
	int seconds = 0;
	int ret = 0;
	struct secdev *pmydev = NULL;

	pmydev = (struct secdev *)pfile->private_data;

	ret = copy_from_user(&seconds, puser, size);
	if(ret)
	{
		printk(KERN_CRIT"copy_from_user falied\n");
		return -1;
	}
	atomic_set(&pmydev->secnum, seconds);

	return size;
}

ssize_t secdev_read(struct file *pfile,char __user *puser,size_t size,loff_t *poff)
{
	struct secdev *pmydev = (struct secdev *)pfile->private_data;
	int ret = 0;
	int seconds = 0;
	
	if(size < sizeof(int))
	{
		return -1;
	}

	if(size > sizeof(int))
	{
		size = sizeof(int);
	}

	seconds = atomic_read(&pmydev->secnum);


	//mydev->buf copy into puser
	ret = copy_to_user(puser,&seconds,size);
	if(ret)
	{
		printk(KERN_CRIT"copy_to_user failed\n");
		return -1;
	}

	return size;
}

struct file_operations myops = {
	.owner = THIS_MODULE,
    .open = secdev_open,
	.release = secdev_close,
	.read = secdev_read,
	.write = secdev_write,
};

static int __init secdev_init(void)
{
	dev_t devno = MKDEV(mymajor,myminor);
	int ret = 0;

	ret = register_chrdev_region(devno,1,"secdev");
	if(ret)
	{
		ret = alloc_chrdev_region(&devno,myminor,1,"secdev");
		if(ret)
		{
			printk(KERN_ALERT"alloc devno failed\n");
			return -1;
		}
		mymajor = MAJOR(devno);
	}

	pgsecdev = (struct secdev *)kmalloc(sizeof(struct secdev),GFP_KERNEL);
	if(NULL == pgsecdev)
	{
		unregister_chrdev_region(devno,1);
		return -1;
	}
	memset(pgsecdev,0,sizeof(struct secdev));
	

	atomic_set(&pgsecdev->secnum,0);
	init_timer(&pgsecdev->tm);

	cdev_init(&pgsecdev->cdev,&myops);
	pgsecdev->cdev.owner = THIS_MODULE;

	cdev_add(&pgsecdev->cdev,devno,1);


	//auto mknod
	pgsecdev->pclass = class_create(THIS_MODULE, "xs3timers");
	pgsecdev->pdevice = device_create(pgsecdev->pclass, NULL, devno, NULL, "xs3tim");

	printk("init\n");

    return 0;
}

static void __exit secdev_cleanup(void)
{
	dev_t devno = MKDEV(mymajor,myminor);
	unregister_chrdev_region(devno,1);

	cdev_del(&pgsecdev->cdev);

	device_destroy(pgsecdev->pclass,devno);
	class_destroy(pgsecdev->pclass);

	kfree(pgsecdev);
	pgsecdev = NULL;
}

module_init(secdev_init);
module_exit(secdev_cleanup);
MODULE_LICENSE("GPL");

