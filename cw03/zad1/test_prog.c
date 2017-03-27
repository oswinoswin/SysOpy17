#include <stdio.h>
#include <stdlib.h>


int main (int argc, char *args[]) {

    char *var;
    char *name;
    printf("Hello from test_prog\n");
    if(argc != 2) {
        fprintf(stderr, "usage: [variable name]\n");
        exit(EXIT_FAILURE);
    }
    name = args[1];
    var = getenv(name);
    if(var != NULL){
        printf("name: %s, value: %s\n", name, var);

    } else{
        printf("Variable %s is not defined\n", name);
    }

    return 0;
}
