#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

pid_t pid;
int status;

void handle_programs(const char *buffer);

/*

struct timeval uStart, uEnd, sStart, sEnd, rEnd, rStart;
struct rusage ruStart, ruEnd;
long double uSum, sSum, rSum;

void startTime();
void stopTime();
void stopTimeAndPrint();*/


int main (int argc, char *args[]) {

    const char *filename;
    int buf_size = 512; //record length
    //char tmp;
    FILE *fileDesc;
    char *buffer;


    if(argc != 2) {
        fprintf(stderr, "usage: [filename]\n");
        exit(EXIT_FAILURE);
    }
    filename = args[1];

    buffer = malloc(sizeof(char) * buf_size);

    fileDesc = fopen(filename, "r");
    if(fileDesc == NULL) {
        fprintf(stderr, "ojoj fopen error\n");
        exit(EXIT_FAILURE);
    }

    if(fseek(fileDesc, 0, SEEK_SET) != 0){
        fprintf(stderr, "fseek error");
        exit(EXIT_FAILURE);
    }

    //read lines from file
    while(fgets(buffer, buf_size, fileDesc)){
        if(strncmp("#",buffer,1) == 0){
            printf("handle env variables\n");
        }
        else{
            printf("handle normal porgrams\n");
            handle_programs(buffer);

        }
    }

    printf("This is the end.\n");
    fclose(fileDesc);
    return 0;
}

void handle_programs(const char *buffer) {
    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork error\n");
        return;
    }
    else if(pid == 0){
        /*if (execlp("echo", "hello from execlp") < 0) {
            fprintf(stderr, "execle error\n");
            return;
        }*/
        printf("Hello from child\n");
        exit(0);
    }
    if(wait(&status) != pid){
        fprintf(stderr, "wait error\n");
        return;
    }
    printf("status: %d\n", status);




}

