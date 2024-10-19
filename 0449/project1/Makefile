CC = gcc
CFLAGS = -Wall -g

all: bmp_steganography

bmp_steganography: main.o bmp.o
	$(CC) $(CFLAGS) -o bmp_steganography main.o bmp.o

main.o: main.c bmp.h
	$(CC) $(CFLAGS) -c main.c

bmp.o: bmp.c bmp.h
	$(CC) $(CFLAGS) -c bmp.c

clean:
	rm -f *.o bmp_steganography
