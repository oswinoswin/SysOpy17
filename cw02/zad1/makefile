CFLAGS=-g -Wall -std=c99
CC=gcc

all: program

program: program.o
	$(CC) $(CFLAGS) program.c -o program

clean:
	rm program *.o  datafile*

test:
	./program lib generate datafile1 5000 4
	./program lib sort datafile1 5000 4 >> wyniki.txt
	./program lib shuffle datafile1 5000 4 >> wyniki.txt
	./program sys sort datafile1 5000 4 >> wyniki.txt
	./program sys shuffle datafile1 5000 4 >> wyniki.txt


	./program lib generate datafile2 5000 512
	./program lib sort datafile2 5000 512 >> wyniki.txt
	./program lib shuffle datafile2 5000 512 >> wyniki.txt
	./program sys sort datafile2 5000 512 >> wyniki.txt
	./program sys shuffle datafile2 5000 512 >> wyniki.txt


	./program lib generate datafile3 5000 4096
	./program lib sort datafile3 5000 4096 >> wyniki.txt
	./program lib shuffle datafile3 5000 4096 >> wyniki.txt
	./program sys sort datafile3 5000 4096 >> wyniki.txt
	./program sys shuffle datafile3 5000 4096 >> wyniki.txt

	./program lib generate datafile4 5000 8192
	./program lib sort datafile4 5000 8192 >> wyniki.txt
	./program lib shuffle datafile4 5000 8192 >> wyniki.txt
	./program sys sort datafile4 5000 8192 >> wyniki.txt
	./program sys shuffle datafile4 5000 8192 >> wyniki.txt
