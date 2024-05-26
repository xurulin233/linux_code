SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)

.PHONY: all clean

all: main

#注释:'-'号的作用：加载错误时，会继续执行 make，
#主要是考虑到首次 make 时，目录中若不存在 '*.d' 文件时，
#加载便会产生错误而停止 make 的执行
-include $(DEPS)	

%.o:%.c
	gcc -c -g -Wall $< -o $@ -MD -MF $*.d -MP 

main: $(OBJS)
	gcc $^ -o $@   #注释:$^:表示所有的依赖文件 $@:表示目标文件

clean: 
	rm -f  *.d *.o main

