AR=ar
CC=gcc
CFLAGS=-Wall -g -std=c99
ARFLAGS=crs

all: mainStatic mainDynamic mainShared

mainStatic: libstatic.a main.o
	$(CC) main.o -o mainStatic -L. -lstatic 

mainShared: libdynamic.so main.o
	$(CC) main.o -o mainShared -Wl,-rpath=. -L. -ldynamic
	
mainDynamic: libdynamic.so mainDynamic.o
	$(CC) -std=c99 mainDynamic.c -o mainDynamic -ldl -Wl,-rpath=.

mainDynamic.o: mainDynamic.c libaddrbook.h

main.o: main.c libaddrbook.h

libstatic.a: libstatic.o
	$(AR) $(ARFLAGS) libstatic.a libstatic.o

libstatic.o: libaddrbook.c libaddrbook.h
	$(CC) $(CFLAGS) -c -o libstatic.o libaddrbook.c

libdynamic.so: libdynamic.o
	$(CC) -shared -o libdynamic.so libdynamic.o

libdynamic.o: libaddrbook.c libaddrbook.h
	$(CC) $(CFLAGS) -c -fPIC -o libdynamic.o libaddrbook.c

run:
	echo "\n\nStatic" >> results.txt 
	./mainStatic >> results.txt
	echo "\n\nShared" >> results.txt
	./mainShared >>results.txt
	echo "\n\nDynamic" >>results.txt
	./mainDynamic >> results.txt

clean:
	rm *.o *.so *.a mainStatic mainDynamic mainShared
