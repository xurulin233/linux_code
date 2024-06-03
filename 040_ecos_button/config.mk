###################################################################################################
# Common Export
CC      = $(CROSS_COMPILE)gcc
AR      = $(CROSS_COMPILE)ar
AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
CPP     = $(CC) -E
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB  = $(CROSS_COMPILE)ranlib
SIZE    = $(CROSS_COMPILE)size
PERL    = perl


