#Makefile
CROSS_COMPILE:= /home/s5pv210/test0108/arm-none-linux-gnueabi-4.8/bin/arm-none-linux-gnueabi-
ARCH:= arm
CC:= $(CROSS_COMPILE)gcc
LD:= $(CROSS_COMPILE)ld
ROOTFS:= /home/s5pv210/test0104/project_tiny210/build/out/rootfs
obj-m += led_s5pv210_dev.o
obj-m += led_s5pv210_drv.o

KDIR=/home/s5pv210/test0108/linux-3.0.8-kernel-master


all:
	make -C $(KDIR) M=$(shell pwd) modules

clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -rf modules.order
install:
	cp led_s5pv210_dev.ko led_s5pv210_drv.ko $(ROOTFS)
