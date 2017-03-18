#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int swapInFileLib(unsigned char *buffer1, unsigned char *buffer2, int recordSize, FILE *fileDesc);
void bubbleSortLib(FILE *fileHandle, unsigned char *buffer1, unsigned char *buffer2, int recordSize);
int swapInFileSys(unsigned char *buffer1, unsigned char *buffer2, int recordSize, int fileDesc);
void bubbleSortSys(int fileHandle, unsigned char *buffer1, unsigned char *buffer2, int recordSize);
void shuffleLib(FILE *fileHandle, unsigned char *buffer1, unsigned char *buffer2, int recordSize);

void shuffleSys(int fileDesc, unsigned char *buffer1, unsigned char *buffer2, int recordSize);

int main (int argc, char *args[]) {
    char *mode;
    char *op;
    const char *filename;
    int ra; //record amount
    int rl; //record length
    int wr; //writed elements
    char tmp;
    FILE *fileDesc;
    FILE *devRandom;
    char *buffer;
    unsigned char *buffer1, *buffer2;
    int intHandle;

    if(argc != 6) {
        fprintf(stderr, "usage: [lib | sys]  [operation: shuffle | generate | sort ]"
                "[filename] [ number of records] [length of a record ]\n");
        exit(EXIT_FAILURE);
    }

    mode = args[1];
    op = args[2];
    filename = args[3];
    ra = atoi(args[4]);
    rl = atoi(args[5]);


    buffer = malloc(sizeof(char) * rl);
    buffer1 = malloc(sizeof(char) * rl);
    buffer2 = malloc(sizeof(char) * rl);
    if(buffer == NULL || buffer1 == NULL || buffer2 == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(EXIT_FAILURE);
    }

    //-----------GENERATE-----------------------------------
    if(strcmp(op, "generate") == 0) {

        fileDesc = fopen(filename, "w");
        if(fileDesc == NULL) {
            fprintf(stderr, "fopen error\n");
            exit(EXIT_FAILURE);
        }

        devRandom = fopen("/dev/urandom", "r");
        if(devRandom == NULL) {
            fprintf(stderr, "fopen error\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < ra; i++) {
            for (int j = 0; j < rl; j++) {
                tmp = (char) getc(devRandom);
                buffer[j] = tmp;
            }
            wr = fwrite(buffer, sizeof(char), rl, fileDesc);
            if (wr != rl) {
                fprintf(stderr, "fwrite error\n");
            }
        }

        fclose(devRandom);
        fclose(fileDesc);
    }
//-------sort------------------------------------------------------
    else if( strcmp(op, "sort") == 0  && strcmp(mode, "lib") == 0 ){

        fileDesc = fopen(filename, "r+w");
        if(fileDesc == NULL) {
            fprintf(stderr, "fopen error\n");
            exit(EXIT_FAILURE);
        }

        if(fseek(fileDesc, 0, SEEK_SET) != 0){
            fprintf(stderr, "fseek error");
            exit(EXIT_FAILURE);
        }

        bubbleSortLib(fileDesc, buffer1, buffer2, rl);
        fclose(fileDesc);
    }

    else if( strcmp(op, "sort") == 0  && strcmp(mode, "sys") == 0 ){


        if ((intHandle = open(filename, O_RDWR)) == -1) {
            fprintf(stderr, "open error\n");
            exit(EXIT_FAILURE);
        }

        bubbleSortSys(intHandle, buffer1, buffer2, rl);

        if (close(intHandle) == -1) {
            fprintf(stderr, "close error\n");
            exit(EXIT_FAILURE);
        }

    }

    //-----shuffle----------------------------------------------------
    else if( strcmp(op, "shuffle") == 0  && strcmp(mode, "lib") == 0 ){

        fileDesc = fopen(filename, "r+w");
        if(fileDesc == NULL) {
            fprintf(stderr, "fopen error\n");
            exit(EXIT_FAILURE);
        }

        if(fseek(fileDesc, 0, SEEK_SET) != 0){
            fprintf(stderr, "fseek error");
            exit(EXIT_FAILURE);
        }

        shuffleLib(fileDesc, buffer1, buffer2, rl);
        fclose(fileDesc);

    }

    else if( strcmp(op, "shuffle") == 0  && strcmp(mode, "sys") == 0 ){


        if ((intHandle = open(filename, O_RDWR)) == -1) {
            fprintf(stderr, "open error\n");
            exit(EXIT_FAILURE);
        }

        shuffleSys(intHandle,buffer1, buffer2, rl);

        if (close(intHandle) == -1) {
            fprintf(stderr, "close error\n");
            exit(EXIT_FAILURE);
        }

    }

    return 0;
}


/*
 * Puts buffer1 as first (fileDesc must be in position between b1 and b2
 */

int swapInFileLib(unsigned char *buffer1, unsigned char *buffer2, int recordSize, FILE *fileDesc) {
    if (fseek(fileDesc, (-1) * recordSize, SEEK_CUR) != 0) {
        fprintf(stderr, "fseek error");
        exit(EXIT_FAILURE);
    }

    if (fwrite(buffer1, sizeof(char), recordSize, fileDesc) != recordSize) {
        fprintf(stderr, "fwrite error");
        exit(EXIT_FAILURE);
    }

    if (fwrite(buffer2, sizeof(char), recordSize, fileDesc) != recordSize) {
        fprintf(stderr, "fwrite error");
        exit(EXIT_FAILURE);
    }

    if (fseek(fileDesc, (-1) * recordSize, SEEK_CUR) != 0) {
        fprintf(stderr, "fseek error");
        exit(EXIT_FAILURE);
    }

    return 0;
}

/*
 *
 * Bubble sort based on records comparison (in 2 buffers) using std functions
*/

void bubbleSortLib(FILE *fileHandle, unsigned char *buffer1, unsigned char *buffer2, int recordSize) {

    int swapped;


    do {

        swapped = 0;
        //printf("swapped = 0\n");
        if (fseek(fileHandle, 0, SEEK_SET) == -1) {
            fprintf(stderr, "fseek error\n");
            exit(EXIT_FAILURE);
        }

        while((fread(buffer1, sizeof(char), recordSize, fileHandle) == recordSize) && (fread(buffer2, sizeof(char), recordSize, fileHandle) == recordSize)) {

            //printf("b1: %s, b2: %s\n", buffer1, buffer2);
            if (buffer1[0] > buffer2[0]) {
                //printf("Zamiana!\n");
                fseek(fileHandle, (-1)*recordSize, SEEK_CUR);
                swapInFileLib(buffer2, buffer1, recordSize, fileHandle);
                //printf("Po zamianie b1: %s, b2: %s\n", buffer1, buffer2);
                swapped = 1;
            }

            if (fseek(fileHandle, (-1) * recordSize, SEEK_CUR) == -1) {
                fprintf(stderr, "fseek error\n");
                exit(EXIT_FAILURE);
            }
        }

    }while(swapped == 1);

}

//System bubble sort

void bubbleSortSys(int fileHandle, unsigned char *buffer1, unsigned char *buffer2, int recordSize) {
    int swapped;
    do {

        swapped = 0;
        if (lseek(fileHandle, 0, SEEK_SET) == -1) {
            fprintf(stderr, "lseek error\n");
            exit(EXIT_FAILURE);
        }

        while((read(fileHandle, buffer1, recordSize) == recordSize) && (read(fileHandle, buffer2, recordSize) == recordSize)) {


            if (buffer1[0] > buffer2[0]) {
                lseek(fileHandle, (-1)*recordSize, SEEK_CUR);
                swapInFileSys(buffer2, buffer1, recordSize, fileHandle);
                swapped = 1;
            }

            if (lseek(fileHandle, (-1) * recordSize, SEEK_CUR) == -1) {
                fprintf(stderr, "fseek error\n");
                exit(EXIT_FAILURE);
            }
        }


    } while (swapped == 1);
}

int swapInFileSys(unsigned char *buffer1, unsigned char *buffer2, int recordSize, int fileDesc) {
    if (lseek(fileDesc, (-1) * recordSize, SEEK_CUR) == -1) {
        fprintf(stderr, "lseek error");
        exit(EXIT_FAILURE);
    }

    if (write(fileDesc, buffer1, recordSize) != recordSize) {
        fprintf(stderr, "write error");
        exit(EXIT_FAILURE);
    }

    if (write(fileDesc, buffer2, recordSize) != recordSize) {
        fprintf(stderr, "write error");
        exit(EXIT_FAILURE);
    }

    if (lseek(fileDesc, (-1) * recordSize, SEEK_CUR) == -1) {
        fprintf(stderr, "lseek error");
        exit(EXIT_FAILURE);
    }

    return 0;
}

void shuffleSys(int fileDesc, unsigned char *buffer1, unsigned char *buffer2, int recordSize) {

    long n, j;

    off_t offsetEnd;
    long nmax;

    srand(time(NULL));
    if ((offsetEnd = lseek(fileDesc, (-1) * recordSize, SEEK_END)) == -1) {
        fprintf(stderr, "lseek error\n");
        exit(EXIT_FAILURE);
    }

    nmax = (long) (offsetEnd / recordSize);

    for(n = nmax; n>1; n--){
        j = rand()%n;

        //find nth record
        if (lseek(fileDesc, n * recordSize, SEEK_SET) == -1) {
            fprintf(stderr, "lseek error");
            exit(EXIT_FAILURE);
        }
        if (read(fileDesc, buffer1, recordSize) != recordSize) {
            fprintf(stderr, "read error");
            exit(EXIT_FAILURE);
        }

        //find j-th record
        if (lseek(fileDesc, j * recordSize, SEEK_SET) == -1) {
            fprintf(stderr, "lseek error");
            exit(EXIT_FAILURE);
        }
        if (read(fileDesc, buffer2, recordSize) != recordSize) {
            fprintf(stderr, "read error");
            exit(EXIT_FAILURE);
        }

        //swap j-th and n-th record
        if (lseek(fileDesc, (-1) * recordSize, SEEK_CUR) == -1) {
            fprintf(stderr, "lseek error");
            exit(EXIT_FAILURE);
        }

        //write content of buffer1 into j-th position
        if (write(fileDesc, buffer1, recordSize) != recordSize) {
            fprintf(stderr, "write error");
            exit(EXIT_FAILURE);
        }

        //go to the n-th position
        if (lseek(fileDesc, n * recordSize, SEEK_SET) == -1) {
            fprintf(stderr, "lseek error");
            exit(EXIT_FAILURE);
        }

        //write content of buffer2 into n-th position
        if (write(fileDesc, buffer2, recordSize) != recordSize) {
            fprintf(stderr, "write error");
            exit(EXIT_FAILURE);
        }


    }
}

void shuffleLib(FILE *fileHandle, unsigned char *buffer1, unsigned char *buffer2, int recordSize) {
    size_t res;

    long offsetEnd;
    long nmax, n, j;

    if (fseek(fileHandle, (-1) * recordSize, SEEK_END) == -1) {
        fprintf(stderr, "fseek error\n");
        exit(EXIT_FAILURE);
    }

    if ((offsetEnd = ftell(fileHandle)) == -1L) {
        fprintf(stderr, "ftell error\n");
        exit(EXIT_FAILURE);
    }

    nmax = (long) (offsetEnd / recordSize);
    srand(time(NULL));

    for(n = nmax; n>0; n--){

        j = rand()%n;

        //find nth record
        if (fseek(fileHandle, n * recordSize, SEEK_SET) == -1) {
            fprintf(stderr, "fseek error\n");
            exit(EXIT_FAILURE);
        }

        if (fread(buffer1, sizeof(char), recordSize, fileHandle) != recordSize){
            fprintf(stderr, "fread error\n");
            exit(EXIT_FAILURE);
        }


        //find j-th record
        if (fseek(fileHandle, j * recordSize, SEEK_SET) == -1) {
            fprintf(stderr, "fseek error\n");
            exit(EXIT_FAILURE);
        }

        if (fread(buffer2, sizeof(char), recordSize, fileHandle) != recordSize){
            fprintf(stderr, "fread error\n");
            exit(EXIT_FAILURE);
        }

        //swap j-th and n-th record

        //write content of buffer1 into j-th position
        if (fseek(fileHandle, (-1) * recordSize, SEEK_CUR) == -1) {
            fprintf(stderr, "fseek error\n");
            exit(EXIT_FAILURE);
        }
        if (fwrite(buffer1, sizeof(char), recordSize, fileHandle) != recordSize) {
            fprintf(stderr, "fwrite error");
            exit(EXIT_FAILURE);
        }

        //go to the n-th position
        if (fseek(fileHandle, n * recordSize, SEEK_SET) == -1) {
            fprintf(stderr, "fseek error\n");
            exit(EXIT_FAILURE);
        }

        //write content of buffer2 into n-th position
        if (fwrite(buffer2, sizeof(char), recordSize, fileHandle) != recordSize) {
            fprintf(stderr, "fwrite error");
            exit(EXIT_FAILURE);
        }



    }


}







