
BUILD_DIR := $(shell pwd)
CC = /home/s5pv210/test0108/arm-none-linux-gnueabi-4.8/bin/arm-none-linux-gnueabi-gcc
CFLAGS = -Wall -O -g  
ROOTFS:= /home/s5pv210/test0104/project_tiny210/build/out/rootfs

led_test : led_test.c
	$(CC)   led_test.c -o led_test
	$(CC) led_server.c -o led_server
	gcc led_client.c -o led_client

install : led_test
	cp led_test led_server $(ROOTFS)


clean:  
	rm -rf *.o led_test led_server led_client


