include $(ECOS_PROJECT_DIR)/config.mk
TARGET = test

CFLAGS = -Wall
CFLAGS  += -I. \
	   -I$(ECOS_PROJECT_DIR)/include -I$(ECOS_PROJECT_DIR)/lib/common

LDFLAGS += -L$(ECOS_PROJECT_DIR)/lib/common -lecos

##################################################################################
SOURCE = $(APP_SRC) 

APP_SRC = main.c


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
	cp -a $(TARGET) $(INSTALL_DIR)/sbin

####################################################################################################

# 引入所有 .d 文件:
sinclude $(DEPS)
