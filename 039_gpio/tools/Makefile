include $(ECOS_PROJECT_DIR)/config.mk
####################################################################################################

TARGET_BTN = btnconfig

####################################################################################################
CFLAGS  += -MD -Werror
CFLAGS  += -I$(EOCS_PROJECT_DIR)/include
CFLAGS  += -I$(ECOS_KERNEL_DIR)/include

LDFLAGS += 

####################################################################################################
SOURCE_BTN = btnconfig.c

####################################################################################################
.PHONY: clean install

####################################################################################################
OBJS_BTN = $(SOURCE_BTN:%.c=obj/%.o)
DEPS_BTN = $(SOURCE_BTN:%.c=obj/%.d)

default: $(TARGET_BTN)

$(TARGET_BTN): $(OBJS_BTN)
	$(CC) $(OBJS_BTN) $(LDFLAGS) -o $@


obj/%.o: %.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $<

####################################################################################################
clean:
	rm -rf $(TARGET_LED) $(TARGET_BTN) $(OBJS_LED) $(DEPS_LED) $(OBJS_BTN) $(DEPS_BTN) obj

install:
	cp -a $(TARGET_LED) $(TARGET_BTN) $(ECOS_INSTALL_DIR)/sbin

####################################################################################################
sinclude $(DEPS)
