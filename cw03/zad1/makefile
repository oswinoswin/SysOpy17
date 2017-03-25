CFLAGS=-g -Wall -std=c99
CC=gcc

all: program

program: program.o
	$(CC) $(CFLAGS) program.c -o program

clean:
	rm program *.o  datafile*

run:
	./program input.txt
