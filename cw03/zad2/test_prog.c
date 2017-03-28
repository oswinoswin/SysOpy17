#include <stdio.h>
#include <stdlib.h>


int main (int argc, char *args[]) {

    printf("Hello from test_prog\n");
    for( int i=0; i<100000; i++){
        printf("still alive %d\n", i);
    }

    return 0;
}


