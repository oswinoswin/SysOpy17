CFLAGS=-g -Wall -std=gnu99
CC=gcc

all: program test_prog

program: program.o
	$(CC) $(CFLAGS) program.c -o program

test_prog: test_prog.o
	$(CC) $(CFLAGS) test_prog.c -o test_prog

clean:
	rm program *.o  test_prog

run:
	./program input.txt 2 5
