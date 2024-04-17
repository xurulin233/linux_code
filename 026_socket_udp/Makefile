all:sendto recvform

recvform:recvform.o common.o
	gcc recvform.o common.o -o recvform -Wall

recvform.o:recvform.c common.h
	gcc -c recvform.c -o recvform.o -Wall



sendto:sendto.o
	gcc sendto.o -o sendto -Wall

sendto.o:sendto.c
	gcc -c sendto.c -o sendto.o -Wall

common.o : common.c common.h
	gcc -c common.c -o common.o -Wall


clean:
	rm sendto recvform *.o

