#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <memory.h>


#define ARGS_NO 30
pid_t pid;
int status;

void handle_programs(char **arg);


char ** split_str(char *str, char **result);
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
        fprintf(stderr, "fopen error\n");
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
            printf("handle normal programs\n");
            printf("In buffer: %s\n", buffer);
            char **arg = split_str(buffer,arg);

            handle_programs(arg);

        }
    }

    fclose(fileDesc);
    return 0;
}


void handle_programs(char **arg) {
    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork error\n");
        return;
    }
    else if(pid == 0){
        static char** yoMum;
        //split_str(buffer, yoMum);
        int i = 0;
        while(arg[i]!=NULL){
            printf("arg: %s\n", arg[i++]);
        }

        if (execvp(arg[0], arg) < 0) {
            fprintf(stderr, "execle error\n");
            return;
        }
        printf("Hello from child\n");
        exit(0);
    }
    if(wait(&status) != pid){
        fprintf(stderr, "wait error\n");
        return;
    }
    printf("status: %d\n", status);


}

char **split_str(char *str, char **result) {
    char * pch;
    char *tmp = str;
    int count = 0;
    char *last_comma;

    while (*tmp)
    {
        if (strncmp(tmp, " ", 1) == 0)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (str + strlen(str) - 1); //increase count if the last el wasn't delim
    printf("COUNT %d\n", count);

    result = (char **)malloc((count+1)*sizeof(char*));
    if(result == NULL){
        fprintf(stderr,"malloc error\n");
        EXIT_FAILURE;
    }

    int i = 0;
    pch = strtok (str," ,");
    if(pch == NULL){
        fprintf(stderr, "split string error\n");
        EXIT_FAILURE;
    }
    else{
        result[i] = pch;
        i++;
    }
    while (pch != NULL && i<count)
    {
        printf("pch = %s\n", pch);
        pch = strtok (NULL, " ,");
        result[i] = malloc(sizeof(char)*strlen(pch));
        strcpy(result[i], pch);
        i++;
    }
    result[count] = NULL;
    for(i=0; i<count; i++){
        printf("result[%d] = %s\n",i, result[i]);
    }

    return result;
}

