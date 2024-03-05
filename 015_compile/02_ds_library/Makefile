main:
	gcc -c test.c
	ar cr lib_test.a test.o
	gcc -fpic -shared -o lib_test.so test.c
	gcc -o main main.c ./lib_test.so
	gcc -o main2 main.c ./lib_test.a

clean:
	rm main main2 lib_test.a test.o lib_test.so

