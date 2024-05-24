# 生成可执行文件:
world.out: hello.o main.o
	cc -o world.out hello.o main.o

# 自动创建 hello.o 文件的隐式规则:
# hello.o: hello.c
#     cc -c -o hello.o hello.c

clean:
	rm -f *.o world.out
