all:sendto recvform

recvform:recvform.o 
	gcc recvform.o -o recvform -Wall

recvform.o:recvform.c
	gcc -c recvform.c -o recvform.o -Wall



sendto:sendto.o
	gcc sendto.o -o sendto -Wall

sendto.o:sendto.c
	gcc -c sendto.c -o sendto.o -Wall


clean:
	rm sendto recvform *.o

