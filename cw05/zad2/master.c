#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <values.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <memory.h>

#define MAX_BUF 20
char *fifoName;
int fifoHandle = -1;
void cleanFifo(void);

int main(int argc, char **argv){

    if(argc != 3){
        fprintf(stderr, "Usage [filename][R]\n");
        EXIT_FAILURE;
    }

    char *myfifo = argv[1];
    int R = atoi(argv[2]);
    //int R = 800;
    int T[R][R];
    char *data = "data";


    FILE *fileDesc;
    FILE *gnuDesc;


    int i,j, iter;

    for(i = 0; i<R; i++) {
        for (j = 0; j < R; j++) {
            T[i][j] = 0;//0;
        }
    }


    int fd;
    char buf[MAX_BUF];
    char *a, *b, *c;
    char *pch;

    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);


    sleep(10);

    /* open, read, and display the message from the FIFO */
    fd = open(myfifo, O_RDONLY);

    while(read(fd, buf, MAX_BUF)>0){
        //printf("Received: %s\n", buf);
        pch = strtok(buf, " ");
        a = pch;
        pch = strtok(NULL, " ");
        b = pch;
        pch = strtok(NULL, " ");
        c = pch;
        i =atoi(a);
        j = atoi(b);
        iter = atoi(c);
        T[j][i] = iter;
        //printf("writes: %d %d %d pch = %s\n", i, j, iter, pch);
    }




    close(fd);

    /* remove the FIFO */
    unlink(myfifo);


    //write to file
    fileDesc = fopen(data,"w");
    for(i = 0; i<R; i++){
        for(j = 0; j<R; j++){
            fprintf(fileDesc,"%d %d %d\n",j, i,T[i][j]);
            //printf("%d %d %d\n", j, i, T[i][j]);
        }
        fprintf(fileDesc,"\n");
    }


    fclose(fileDesc);

    gnuDesc = popen("gnuplot","w");
    fprintf(gnuDesc, "set view map\n");
    fprintf(gnuDesc, "set xrange[0:%d]\n", R);
    fprintf(gnuDesc, "set yrange[0:%d]\n", R);
    fprintf(gnuDesc, "plot '%s' with image\n", data);

    fflush(gnuDesc);
    getc(stdin);
    fclose(gnuDesc);


    return 0;
}
