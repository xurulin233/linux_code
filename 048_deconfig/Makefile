CC = gcc
SRC = $(wildcard *.c)
BIN = $(SRC:%.c=%)

-include .config

ifdef CONFIG_HELLO
CFLAGS += -DCONFIG_HELLO
endif


all : verify_config ${BIN}

${BIN}:%:%.c
	${CC} $^ -o $@ $(CFLAGS)

verify_config:
	@if [ ! -r .config ]; then \
                echo 'Building hostapd requires a configuration file'; \
                echo '(.config). See README for more instructions. You can'; \
                echo 'run "cp defconfig .config" to create an example'; \
                echo 'configuration.'; \
                exit 1; \
        fi

clean:
	rm -rf ${BIN}
 
.PHONY: all clean
