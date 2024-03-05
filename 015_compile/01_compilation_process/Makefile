all:
	gcc -o test1.c -E test.c
	gcc -o test.s -S test.c
	gcc -o test.o -c test.c
	gcc -o main test.c
clean:
	rm test1.c test.s test.o main
