#Makefile
CROSS_COMPILE:= /home/s5pv210/test0108/arm-none-linux-gnueabi-4.8/bin/arm-none-linux-gnueabi-
ARCH:= arm
CC:= $(CROSS_COMPILE)gcc
LD:= $(CROSS_COMPILE)ld
ROOTFS:= /home/s5pv210/test0104/project_tiny210/build/out/rootfs
obj-m += helloworld.o

KDIR=/home/s5pv210/test0108/linux-3.0.8-kernel-master


all:
	make -C $(KDIR) M=$(shell pwd) modules

clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -rf modules.order
install:
	cp helloworld.ko $(ROOTFS)
