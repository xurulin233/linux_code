
################################################################################
# module name
MOD_NAME := gpio_mod
KMODSUF  := ko
obj-m    += $(MOD_NAME).o

################################################################################
# module objs(.c > .o)
$(MOD_NAME)-objs := ecos_gpio.o

################################################################################
EXTRA_CFLAGS += -MD -D__KERNEL__ -DMODULE -D_LINUX_ -Werror
EXTRA_CFLAGS += -I$(ECOS_PROJECT_DIR)/include

################################################################################
# module objs(.c > .o)
default:
	$(MAKE) -C $(ECOS_KERNEL_DIR) SUBDIRS=$(shell pwd) modules
	$(MAKE) -C tools

install:
	cp -a $(MOD_NAME).ko $(ECOS_INSTALL_DIR)/lib/modules
	$(MAKE) -C tools install

clean:
	rm -rf *~ *.o *.ko *.mod.c
	rm -rf .depend .version .*.o.flags .*.o.d .*.o.cmd .*.ko.cmd *.symvers
	rm -rf .tmp_versions modules.order
	$(MAKE) -C tools clean

################################################################################
