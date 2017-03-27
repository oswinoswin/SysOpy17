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
char *ev;
extern char** environ;

void handle_programs(char **arg, char * buffer);


char ** split_str(char *str, char **result);

void free_arg(char **pString);

void handle_env_variables(char *buffer);

void print_env();

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
    char **arg;


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
        printf("COMMAND: %s\n", buffer);
        if(strncmp("#",buffer,1) == 0){
            handle_env_variables(buffer);
        }
        else{
            arg = split_str(buffer, arg);
            handle_programs(arg, buffer);

        }
    }

    //free_arg(arg);
    free(buffer);
    fclose(fileDesc);
    return 0;
}



void handle_env_variables(char *buffer) {

    char *name = buffer;
    char *value;
    value = strtok (name, " ,\n");
    value =  strtok (NULL, " ,\n");
    name++; //get rid of #
    if(value != NULL){
        setenv(name, value, 1);
        printf("At %s is value: %s\n", name, getenv(name));
    }else if(getenv(name) != NULL){
        unsetenv(name);
        printf("Deleting: %s\n", name);
    }


}

void free_arg(char **pString) {
    int i = 0;
    while(pString[i] != NULL){
        printf("Time to free arg[%d] = %s\n", i, pString[i]);
        free(pString[i]);
        i++;
    }
    //free(pString);

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
        pch = strtok (NULL, " ,\n");
        result[i] = malloc(sizeof(char)*strlen(pch));
        strcpy(result[i], pch);
        i++;
    }
    result[count] = NULL;

    return result;
}

void print_env() {
    for (char **env = environ; *env; ++env)
        printf("%s\n", *env);
}

void handle_programs(char **arg, char *buffer) {
    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork error\n");
        return;
    }
    else if(pid == 0){

        if (execvp(arg[0], arg) < 0) {
            fprintf(stderr, "execle error\n");
            EXIT_FAILURE;
        }
        exit(0);
    }
    if(wait(&status) != pid){
        fprintf(stderr, "wait error\n");
        EXIT_FAILURE;
    }
    printf("status: %d\n", status);
    if(status != 0){
        fprintf(stderr, "Error ocured while excuting command: %s\n", buffer);
        exit(1);
    }

}

