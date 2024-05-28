TARGET = test

CC = cc
CFLAGS = -Wall
CFLAGS  += -I. \
	   -I-I$(PROJECT_DIR)/include -I$(PROJECT_DIR)/lib/common

LDFLAGS += -L$(PROJECT_DIR)/lib/common -lhello

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
