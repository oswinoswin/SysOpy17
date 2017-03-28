#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler(int sig){
    printf("%s\n","whatever");
    signal(sig, sig_handler);
}


int main (int argc, char *args[]) {

    signal(SIGXCPU, sig_handler);
    while(1);

    return 0;
}
