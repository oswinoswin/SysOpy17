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



struct timeval uStart, uEnd, sStart, sEnd, rEnd, rStart;
struct rusage ruStart, ruEnd;
long double uSum, sSum, rSum;

void startTime();
void stopTime();
void stopTimeAndPrint();

struct rlimit limit;
int tl, ml;

int main (int argc, char *args[]) {


    const char *filename;
    int buf_size = 512; //record length
    FILE *fileDesc;
    char *buffer;
    char **arg;



    if(argc != 4) {
        fprintf(stderr, "usage: [filename][time limit (sec)][memory limit(MB)]\n");
        exit(EXIT_FAILURE);
    }
    filename = args[1];
    tl = atoi(args[2]);
    ml = atoi(args[3]);




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
        printf("\n");
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
        printf("At: %s is value: %s\n", name, getenv(name));
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

void print_limits(){
    if(getrlimit(RLIMIT_CPU, &limit) < 0){
        fprintf(stderr, "getlimit error");
        return;
    }
    printf("current CPU limit: ");
    if(limit.rlim_cur == RLIM_INFINITY){
        printf("infinity\n");
    } else{
        printf("%ld\n", limit.rlim_cur);
    }

    printf("max CPU limit: ");
    if(limit.rlim_max == RLIM_INFINITY){
        printf("infinity\n");
    } else{
        printf("%ld\n", limit.rlim_max);
    }

    if(getrlimit(RLIMIT_AS, &limit) < 0){
        fprintf(stderr, "getlimit error");
        return;
    }

    printf("current memory limit: ");
    if(limit.rlim_cur == RLIM_INFINITY){
        printf("infinity\n");
    } else{
        printf("%ld\n", limit.rlim_cur);
    }

    printf("max memory limit: ");
    if(limit.rlim_max == RLIM_INFINITY){
        printf("infinity\n");
    } else{
        printf("%ld\n", limit.rlim_max);
    }
}

void set_cpu_limits(int curr, int max){
    limit.rlim_cur = curr;
    limit.rlim_max = max;
    if(setrlimit(RLIMIT_CPU, &limit) != 0){
        fprintf(stderr, "setrlimit error\n");
        EXIT_FAILURE;
    }
}

void set_mem_limits(int curr, int max){
    limit.rlim_cur = curr;
    limit.rlim_max = max;
    if(setrlimit(RLIMIT_AS, &limit) != 0){
        fprintf(stderr, "setrlimit error\n");
        EXIT_FAILURE;
    }
}

void handle_programs(char **arg, char *buffer) {


    print_limits();



    startTime();

    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork error\n");
        return;
    }
    else if(pid == 0){
        set_cpu_limits(tl, tl);
        set_mem_limits(ml, ml);
        print_limits();

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
    stopTimeAndPrint();

}


void startTime(){

    getrusage(RUSAGE_CHILDREN, &ruStart);

}
void stopTime(){
    getrusage(RUSAGE_CHILDREN, &ruEnd);

    uStart = ruStart.ru_utime;
    uEnd = ruEnd.ru_utime;
    sStart = ruStart.ru_stime;
    sEnd = ruEnd.ru_stime;
}

void stopTimeAndPrint(){
    stopTime();
    printf("user: %09ld ms, system: %09ld ms\n",
           (( uEnd.tv_sec - uStart.tv_sec)*1000000L
            + uEnd.tv_usec) - uStart.tv_usec,
           (( sEnd.tv_sec - sStart.tv_sec)*1000000L
            + sEnd.tv_usec) - sStart.tv_usec);
}


/* char name;
    int resource;
    struct rlimit limit;
    if(getrlimit(RLIMIT_CPU, &limit) < 0){
        fprintf(stderr, "getrlimit error\n");
        EXIT_FAILURE;
    }
    if( limit.rlim_cur == RLIM_INFINITY){
        printf("infinity\n");
    }
    else{
        printf("rlimit curr: %ld\n",limit.rlim_max);
    }*/