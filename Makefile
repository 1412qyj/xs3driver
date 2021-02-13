INC_DIR = ./inc/
#HeadFile:=$(wildcard $(INC_DIR)/*.h)
#HeadFile+=$(wildcard*.h)
CFILEs := $(wildcard *.c)
OBJs := $(patsubst %.c,%.o, $(CFILEs))

ifeq ($(KERNELRELEASE),)

#ifeq ($(ARCH),arm)
KERNELDIR ?= /home/yj/linux_kernel/pd-a8-plc-linux/
ROOTFS ?= /home/yj/source/rootfs 
#else
#KERNELDIR ?= /lib/modules/$(shell uname -r)/build
#endif
PWD := $(shell pwd)



modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules INSTALL_MOD_PATH=$(ROOTFS) modules_install

tranfer:
	cp *.ko /home/yj/source/rootfs/qyj/

clean:
	rm -rf *.o *.ko .*.cmd *.mod.* modules.order Module.symvers .tmp.versions *.mod 

else
obj-m += 01_xs3_led_dev.o
#obj-m += 02_xs3_uart_dev.o



endif
