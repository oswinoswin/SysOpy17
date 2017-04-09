#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

    /*if(argc != 3){
        fprintf(stderr, "Usage [filename][R]\n");
        EXIT_FAILURE;
    }

    char *filename = argv[1];
    int R = atoi(argv[2]);*/
    int R = 10;
    int T[R][R];
    char *output = "data";
    FILE *fileDesc;
    char *buffer;

    for(int i = 0; i<R; i++) {
        for (int j = 0; j < R; j++) {
            T[i][j] = 256;//0;
        }
    }

    //open file to write
    fileDesc = fopen(output, "w");
    if(fileDesc == NULL) {
        fprintf(stderr, "fopen error\n");
        exit(EXIT_FAILURE);
    }

    int recordSize = 7;

    buffer = malloc(sizeof(char) * recordSize);


    //start writting
    if (fseek(fileDesc, 0, SEEK_SET) == -1) {
        fprintf(stderr, "fseek error\n");
        exit(EXIT_FAILURE);
    }

    buffer[1] = ' ';
    buffer[4] = ' ';
    buffer[6] = '\n';
    printf("buffer= %s\n", buffer);

    if (fwrite(buffer, sizeof(char), recordSize, fileDesc) != recordSize) {
        fprintf(stderr, "fwrite error");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i<R; i++){
        for(int j = 0; j<R; j++){
            buffer[0] = i + '0';
            buffer[3] = j + '0';
            int tmp = i + j  + '0';
            printf("T[%d][%d]= %c\t",i,j, (char)tmp + '0');
            buffer[5] = tmp + '0';

            if (fwrite(buffer, sizeof(char), recordSize, fileDesc) != recordSize) {
                fprintf(stderr, "fwrite error");
                exit(EXIT_FAILURE);
            }


        }

        //buffer[0] = '\n';

        if (fwrite(buffer, sizeof(char), 1, fileDesc) != 1) { //write endl
            fprintf(stderr, "fwrite error");
            exit(EXIT_FAILURE);
        }


    }

    //popen("gnuplot",)

    //getc(stdin);



    fclose(fileDesc);


    return 0;
}

