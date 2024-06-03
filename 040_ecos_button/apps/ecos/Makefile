
include $(ECOS_PROJECT_DIR)/config.mk
TARGET = configserver

CFLAGS = -Wall
CFLAGS  += -I. \
	   -I$(ECOS_PROJECT_DIR)/include -I$(ECOS_PROJECT_DIR)/lib/common \
	   -I$(ECOS_PROJECT_DIR)/apps/ecos/product

LDFLAGS += -L$(ECOS_PROJECT_DIR)/lib/common -lecos

LDFLAGS += $(ECOS_PROJECT_DIR)/apps/ecos/product/libecosproduct.a

##################################################################################
SOURCE = $(APP_SRC) 

APP_SRC = ecos_main.c \
	ecos_dev_api.c


####################################################################################################
.PHONY: clean install

####################################################################################################

# *.c => *.o
OBJS = $(SOURCE:%.c=obj.$(TARGET)/%.o)

# *.c => *.d
DEPS = $(SOURCE:%.c=obj.$(TARGET)/%.d)

default:
	make -C product		
	make $(TARGET)

$(TARGET): $(OBJS)
	echo $(CC)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

obj.$(TARGET)/%.o: %.c
	@mkdir -p obj.$(TARGET)
	$(CC) $(CFLAGS) -c -o $@ $<

###################################################################################################
clean:
	make -C product clean
	rm -rf $(TARGET) $(OBJS) $(DEPS) obj.$(TARGET)

install:
	cp -a $(TARGET) $(INSTALL_DIR)/sbin

####################################################################################################

# 引入所有 .d 文件:
sinclude $(DEPS)
