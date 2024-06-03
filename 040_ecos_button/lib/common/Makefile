include $(ECOS_PROJECT_DIR)/config.mk

TARGET_NAME = ecos
TARGET      = lib$(TARGET_NAME).so

####################################################################################################
CFLAGS  += -MD -Werror
CFLAGS  += -I$(ECOS_PROJECT_DIR)/include
CFLAGS += -fPIC

####################################################################################################

SOURCE = \
	ecos_mib_comm.c \
	ecos_sockv4.c

####################################################################################################
.PHONY: clean install

####################################################################################################
OBJS = $(SOURCE:%.c=obj.$(TARGET)/%.o)
DEPS = $(SOURCE:%.c=obj.$(TARGET)/%.d)

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $(OBJS)

obj.$(TARGET)/%.o: %.c
	@mkdir -p obj.$(TARGET)
	$(CC) $(CFLAGS) -c -o $@ $<

####################################################################################################
clean:
	rm -rf $(TARGET) $(OBJS) $(DEPS) obj.$(TARGET)

install:
	cp -a $(TARGET) $(INSTALL_DIR)/lib

####################################################################################################
sinclude $(DEPS)
