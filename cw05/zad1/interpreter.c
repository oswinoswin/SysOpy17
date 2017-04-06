#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <memory.h>

char **split_str(char *str, int *num);

char **split_str_by_pipe(char *str, int *num);

int main(int argc, char **argv)
{
    int pipefd[2];
    int pid;
    char **to_execute;
    char **args_child;
    char **args_parent;
    int count;
    int status;

    char start[] = "ls -l | head -3\0";
    to_execute = split_str_by_pipe(start, &count);
    char *str1 = to_execute[0];
    char *str2 = to_execute[1];


    args_child = split_str(str1, &count);
    args_parent = split_str(str2, &count);

    pipe(pipefd);

    pid = fork();
    if (pid == 0)
    {
        close(pipefd[0]);

        if(dup2(pipefd[1], STDOUT_FILENO)<0){
            fprintf(stderr, "dup2\n");
            EXIT_FAILURE;
        }

        if(execvp(args_child[0], args_child)<0){
            fprintf(stderr,"execvp error\n");
            EXIT_FAILURE;
        }

    }
    else
    {
        close(pipefd[1]);

        if(dup2(pipefd[0], STDIN_FILENO)<0){
            fprintf(stderr, "dup2\n");
            EXIT_FAILURE;
        }

        if(execvp(args_parent[0], args_parent)<0){
            fprintf(stderr,"execvp error\n");
            EXIT_FAILURE;
        }

        close(pipefd[0]);

        waitpid(pid, NULL, 0);

    }

    return 0;
}


char **split_str(char *str, int *num) {
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
    *num = count;
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

