####################################################################################################
include $(ECOS_PROJECT_DIR)/config.mk

TARGET_NAME = ecosproduct
TARGET      = lib$(TARGET_NAME)

####################################################################################################
CFLAGS  += -MD -Werror

CFLAGS  += -I. \
		-I$(ECOS_PROJECT_DIR)/include \
		-I$(ECOS_PROJECT_DIR)/modules/ecos_gpio \
		-I$(ECOS_PROJECT_DIR)/apps/ecos

####################################################################################################
SOURCE=			\
	ecos_product_misc.c     


####################################################################################################
.PHONY: clean install

####################################################################################################
OBJS = $(SOURCE:%.c=obj.$(TARGET)/%.o)
DEPS = $(SOURCE:%.c=obj.$(TARGET)/%.d)

default:
	@rm -rf $(TARGET)
	make $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET).a $(OBJS)
	$(CC) $(LDFLAGS) -shared -o $(TARGET).so $(OBJS)

obj.$(TARGET)/%.o: %.c
	@mkdir -p obj.$(TARGET)
	$(CC) $(CFLAGS) -c -o $@ $<

####################################################################################################
clean:
	rm -rf $(TARGET).so $(TARGET).a $(OBJS) $(DEPS) obj.$(TARGET)

#install:
#	cp -a $(TARGET).a $(EBI_INSTALL_DIR)/lib

####################################################################################################
sinclude $(DEPS)
