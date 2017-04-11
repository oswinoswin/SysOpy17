#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/*write better parsing function*/

#define MAX_ARGS 10
#define COMMANDS_NO 20;
//size_t  *restrict MAX_LINE=200;

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

int handle_one(char ***line){
    printf("handle one\n");

    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        //printf("Executing: %s",line[0][0]);
        //const char *tmp = (const char*) malloc(sizeof(char)* MAX_ARGS);
        //strcpy(tmp, line[0][0]);
        //char * const testowy[] = {line[0][0], NULL};
        //printf("testowy[0] = %s\n", testowy[0]);
        //execvp(testowy[0], (char * const*) testowy);

        char *testowy[] = {"ls", NULL};
        strcpy(testowy[0], line[0][0]);
        execvp((const char*)testowy[0],(char * const *) testowy);
    } else{
        waitpid(pid, 0, 0);
    }


    return 0;
}


int main (int argc, char **argv)
{

   /* char input[] = "ls -l | tail -5 | uniq | awk {print$1} | head -1\0";
    handle_input(input);

    strcpy(input, "echo Ala");
    handle_input(input);*/
    size_t max_line = 20;


    char *input;

    input = (char *)malloc(200*sizeof(char));

    int char_no;


    char *testowy[] = {"ls", NULL};
    //execvp(testowy[0], testowy);
    char ***line = (char***)malloc(sizeof(char**)*MAX_ARGS);
    line[0] = testowy;

    fork_pipes(1,line);


    while((char_no = getline(&input, &max_line,stdin)) > 1){

        handle_input(input);
        printf("There was %d chars read\n", char_no);
        printf("Write something...\n");
    }


    free(input);
    return 0;
}

void handle_input(char *input){
    int num;
    char **splitted_by_pippe;
    splitted_by_pippe = split_str_by_pipe(input, &num);
    char **line[num];
    for(int i = 0; i<num; i++){
        line[i] = split_str(splitted_by_pippe[i]);
    }
    printf("num = %d\n", num);

    if(num == 1 && line[0][1] == NULL){
        handle_one(line);
        return;
    }

    else{
        fork_pipes(num, line);
        return;

    }


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
