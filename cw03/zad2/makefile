CFLAGS=-g -Wall -std=gnu99
CC=gcc

all: program test_prog

program: program.o
	$(CC) $(CFLAGS) program.c -o program

test_prog: test_prog.o
	$(CC) $(CFLAGS) test_prog.c -o test_prog

set_cpu: set_cpu.o
		$(CC) $(CFLAGS) set_cpu.c -o set_cpu

clean:
	rm program *.o  test_prog

run:
	./program input.txt 100 100
