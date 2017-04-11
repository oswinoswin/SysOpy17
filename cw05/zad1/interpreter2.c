#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define MAX_ARGS 10
#define MAX_COMMANDS 20
#define MAX_COMMAND 100
#define LINE_LENGTH 500


char **split_str(char *str);

char **split_str_by_pipe(char *str, int *num);

void handle_input(char *input);

int spawn_proc (int in, int out, char **command)
{
    pid_t pid;

    if ((pid = fork ()) == 0)
    {
        if (in != 0)
        {
            dup2 (in, 0);
            close (in);
        }

        if (out != 1)
        {
            dup2 (out, 1);
            close (out);
        }

        return execvp(command[0],(char * const *) command);
    }

    return pid;
}

int fork_pipes(int n, char ***line)
{
    printf("fork pipes\n");
    int i;
    pid_t pid;
    int in, fd [2];

    /* The first process should get its input from the original file descriptor 0.  */
    in = 0;
    if(n == 1){
        printf("command[%d] = %s  ",0, line[0][0]);
        printf("command[%d] = %s  ",0, line[0][1]);
        if((pid = fork()) == 0){
            execvp(line[0][0], line[0]);
        }else{
            goto end;
        }
    }

    /* Note the loop bound, we spawn here all, but the last stage of the pipeline.  */
    for (i = 0; i < n - 1; i++)
    {
        pipe (fd);

        /* f [1] is the write end of the pipe, we carry `in` from the prev iteration.  */
        int j = 0;
        while(line[i][j] != NULL){
            printf("command[%d] = %s  ",j, line[i][j]);
            j++;
        }
        printf("\n");
        spawn_proc (in, fd [1], line[i]);

        /* No need for the write end of the pipe, the child will write here.  */
        close (fd [1]);

        /* Keep the read end of the pipe, the next child will read from there.  */
        in = fd [0];
    }

    /* Last stage of the pipeline - set stdin be the read end of the previous pipe
       and output to the original file descriptor 1. */

    if ((pid = fork ()) == 0)
    {
        if (in != 0)
        {
            dup2 (in, 0);
            close (in);
        }

        //execute last command
        execvp(line[i][0],line[i]);
    }
    else if(pid > 0){
        printf("juhu\n");
        end:
        /*wait for chilren*/
        while(waitpid(0,0,0) <= 0);


    }

    return 0;
}



int main (int argc, char **argv)
{


    char *input;
    size_t max_line = LINE_LENGTH;
    input = (char *)malloc(max_line*sizeof(char));


    char *tmp[MAX_COMMANDS] ;
    char **pom[MAX_ARGS];
    int commands_count = 0;
    int args_count = 0;
    //split with |
    char *tmp2 = (char*)malloc(sizeof(char)*MAX_COMMANDS);

    char example[] = "ls -l | tail -1";

    char * spp; //split by "|"
    char *sps; //split by " "

    spp = strtok(example, "|");
    if(spp == NULL){
        fprintf(stderr, "split string error\n");
        EXIT_FAILURE;
    }
    tmp[commands_count] = spp;

    commands_count++;
    while((spp = strtok(NULL, "|")) != NULL){
        tmp[commands_count] = spp;
        //printf("tmp:%s\n", tmp[commands_count]);

        commands_count++;

    }

    for(int i = 0 ; i < commands_count; i++){
        printf("tmp:%s\n", tmp[i]);
        args_count = 0;
        //tmp2 = tmp[i];
        strcpy(tmp2, tmp[i]);
        printf("tmp:%s\n", tmp[i]);
        sps = strtok(tmp2, " ");
        printf("tmp:%s\n", tmp[i]);
        if(sps == NULL){
            fprintf(stderr, "split string error\n");
            EXIT_FAILURE;
        }
        pom[i][args_count] = sps;
        printf("pom[%d][%d] = %s\n",i, args_count, pom[i][args_count]);

        args_count++;
        while((sps = strtok(NULL, " ")) != NULL){
            pom[i][args_count] = sps;
            args_count++;
        }
        pom[i][args_count] = NULL;
        execvp(pom[0][0], pom[0]);

    }

    return 0;




    sps = strtok(tmp, " ");
    if(sps == NULL){
        fprintf(stderr, "split string error\n");
        EXIT_FAILURE;
    }

    pom[args_count] = sps;
    args_count++;
    printf("first: %s\n", sps);
    while((sps = strtok(NULL, " ")) != NULL){
        pom[args_count] = sps;
        printf("spl:%s\n",pom[args_count]);
        args_count++;

    }
    pom[args_count] = NULL;
    printf("%d\n", args_count);
//    line[0] = pom;
 //   fork_pipes(1, line);



    /*while((char_no = getline(&input, &max_line,stdin)) > 1){

        handle_input(input);
        printf("There was %d chars read\n", char_no);
        int num = 0;
        char ** zenon = split_str_by_pipe(input, &num);
        for(int i = 0; i<num; i++){
            printf("zenon: %s\n", zenon[i]);
        }

        printf("Write something...\n");
    }*/


    free(input);
    return 0;
}



char **split_str(char *str) {
    char * pch;

    int count = 0;
    char *last_comma;
    char **result;

    while(strncmp(str, " ", 1) == 0) { //get rid of spaces in the begining
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
        printf("pch = %s\n", pch);
        result[i] = pch;
        i++;
    }


    while (pch != NULL && i<count){

        pch = strtok (NULL, " ,\n");
        result[i] = malloc(sizeof(char)*strlen(pch));

        strcpy(result[i], pch);
        printf("pch = %s\n", pch);
        i++;
    };

    if(count == 0){
        count++;
    }

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

    if(count == 0){
        count++;
    }
    result[count] = NULL;


    *num = count;
    return result;
}
