#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <memory.h>

#define BUFFER_LEN	256

typedef struct COMMAND{
    struct COMMAND *next;
    char arguments[BUFFER_LEN][BUFFER_LEN];
} COMMAND;

char **split_str(char *str);

char **split_str_by_pipe(char *str, int *num);


int main(int argc, char **argv)
{

    int pipefd[2];
    int pid;
    char **to_execute;
    char **args1;
    char **args2;
    char ** args3;
    int count;
    int status;

    COMMAND command;
    char start[] = "ls -l | head -6 | tail -5 | tail -1\0";
    to_execute = split_str_by_pipe(start, &count);
    //printf("count = %d\n", count);

    char **argsy[count];
    for(int i = 0; i<count; i++){
        argsy[i] = split_str(to_execute[i]);
        //printf("argsy[%d] %s\n", i, argsy[i][0]);
    }


/*
    char *str1 = to_execute[0];
    char *str2 = to_execute[1];*/


   /* args1 = split_str(str1);
    args2 = split_str(str2);
    args3 = split_str(to_execute[2]);*/



    int i = 0;
    int numPipes = count - 1;



    int pipefds[2*numPipes];

    for(i = 0; i < 2*(numPipes); i++){
        if(pipe(pipefds + i*2) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for(int j = 0; j<count; j++){
        //printf("j = %d\n",j);
        //printf("Should execute %s\n", argsy[j][0]);
        pid = fork();
        if (pid == 0) {
            sleep(1);

            //if not first command
            if (j != 0) {
                //printf("j!=0\n");
               if (dup2(pipefds[(j - 1) * 2], 0) < 0) {
                    perror(" dup2");///j-2 0 j+1 1
                    exit(EXIT_FAILURE);

                }
                //printf("j = %d  dup(pipefd[%d], 0])\n",j, (j-1)*2);
            }
            //if not last command
            //printf("j = %d, count-1 =%d\n", j, count-1);

            if(j < count - 1) {
                printf("j<count -1\n");
                if(dup2(pipefds[j * 2 + 1], 1) < 0) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                printf("j = %d dup(pipefd[%d], 1])\n", j,2*j+1);
            } else{
                printf("Now we should get result\n");
            }

            for (i = 0; i < 2 * numPipes; i++) {
                close(pipefds[i]);
            }

           // printf("j = %d %s\n",j, argsy[j][0]);
            fprintf(stderr, "j = %d %s\n",j, argsy[j][0]);
            if (execvp(argsy[j][0], argsy[j]) < 0) {
                perror(argsy[j][0]);
                exit(EXIT_FAILURE);
            }


        }
        else if(pid < 0) {
                perror("error");
                exit(EXIT_FAILURE);
            }


    }
    for(i = 0; i < 2 * numPipes; i++){
       close(pipefds[i]);
        //puts("closed pipe in parent");
    }
    while(waitpid(0,0,0) <= 0);



    return 0;
}


char **split_str(char *str) {
    char * pch;

    int count = 0;
    char *last_comma;
    char **result;

    while(strncmp(str, " ", 1) == 0) { //get rid of spaces int the begining
        str++;
    }

    char *tmp = str;


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


    do{

        pch = strtok (NULL, " ,\n");
        result[i] = malloc(sizeof(char)*strlen(pch));

        strcpy(result[i], pch);
        i++;
    }while (pch != NULL && i<count);

    result[count] = NULL;
    return result;
}

char **split_str_by_pipe(char *str, int *num) {
    char * pch;
    char *tmp = str;
    int count = 0;
    char *last_comma;
    char **result;

    while (*tmp)
    {
        if (strncmp(tmp, "|", 1) == 0)
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
    pch = strtok (str,"|");
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
        pch = strtok (NULL, "|\n");
        result[i] = malloc(sizeof(char)*strlen(pch));
        strcpy(result[i], pch);
        i++;
    }
    result[count] = NULL;

    *num = count;
    return result;
}
