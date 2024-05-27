TARGET = configserver

CC = cc
CFLAGS = -Wall
CFLAGS  += -I. \
	   -Imib

VPATH   += mib
##################################################################################
SOURCE = $(APP_SRC) $(MIB_SRC)

APP_SRC = main.c \
		mib_reg.c		\
		shash32.c	

MIB_SRC = mib_sys.c

####################################################################################################
.PHONY: clean install

####################################################################################################

# *.c => *.o
OBJS = $(SOURCE:%.c=obj.$(TARGET)/%.o)

# *.c => *.d
DEPS = $(SOURCE:%.c=obj.$(TARGET)/%.d)

default:
	make $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

obj.$(TARGET)/%.o: %.c
	@mkdir -p obj.$(TARGET)
	$(CC) $(CFLAGS) -c -o $@ $<

###################################################################################################
clean:
	rm -rf $(TARGET) $(OBJS) $(DEPS) obj.$(TARGET)

install:
	cp -a $(TARGET) $(EBI_INSTALL_DIR)/sbin

####################################################################################################

# 引入所有 .d 文件:
sinclude $(DEPS)
