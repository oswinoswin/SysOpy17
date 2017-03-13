AR=ar
CC=gcc
CFLAGS=-Wall -g -std=c99
ARFLAGS=crs
OPT=
#to run with custom optimalization do: make all OPT=[-O0 | -O1 | -O | -O3 | -Os | -Ofast] 
# -O0 compilation time
# -O1 or O code size and execution time
# -O2 and -O3 also code size and execution time (O2 is safer)
# -Os code size
#

all: mainStatic mainDynamic mainShared

mainStatic: libstatic.a main.o
	$(CC) $(OPT) $(CFLAGS) main.o -o mainStatic -L. -lstatic 

mainShared: libdynamic.so main.o
	$(CC) $(OPT) $(CFLAGS) main.o -o mainShared -Wl,-rpath=. -L. -ldynamic
	
mainDynamic: libdynamic.so mainDynamic.o
	$(CC) $(OPT) $(CFLAGS) mainDynamic.c -o mainDynamic -ldl -Wl,-rpath=.

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
	echo "\nStatic\n" >> results.txt 
	./mainStatic >> results.txt
	echo "\nShared\n" >> results.txt
	./mainShared >>results.txt
	echo "\nDynamic\n" >>results.txt
	./mainDynamic >> results.txt

clean:
	rm *.o *.so

action:
	echo OPT is $(OPT)