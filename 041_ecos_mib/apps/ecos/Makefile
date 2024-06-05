TARGET = cfgserver

CC = cc
CFLAGS = -Wall
CFLAGS  += -I. \
	   -I$(PROJECT_DIR)/include -I$(PROJECT_DIR)/lib/common \
	   -I$(PROJECT_DIR)/apps/ecos/ecos_mib

LDFLAGS += -L$(PROJECT_DIR)/lib/common -lecos

VPATH   += ecos_mib

##################################################################################
SOURCE = $(ECOS_CORE) $(ECOS_APP)  $(ECOS_MIB)

ECOS_CORE = ecos_shash32.c	\
	ecos_config_file.c	\

ECOS_APP  = ecos_main.c		\
	ecos_mib_reg.c		\
	ecos_oem_mib_parser.c	\
	ecos_oem_root.c		\
	ecos_oem_database.c	\
	ecos_addr_rule.c	


ECOS_MIB = ecos_mib_system.c	\


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
