AS = as
CC = gcc
SFLAGS = -no-pie -fno-pie
CFLAGS = -Wall -g

main: malloc.o teste.o
	$(CC) $(SFLAGS) malloc.o teste.o -o main $(CFLAGS)

teste.o: teste.c
	$(CC) -c teste.c -o teste.o

malloc.o: malloc.s
	$(AS) malloc.s -o malloc.o

clean:
	rm -rf *.o main