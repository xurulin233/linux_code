obj-m += timer_task.o

KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build


CONFIG_MODULE_SIG=n
all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean

