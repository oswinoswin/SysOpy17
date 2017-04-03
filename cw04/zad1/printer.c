#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
int forward;
void au(int sig_no) {
    printf("Otrzymale signal SIGINIT\n");
    exit(0);
}

void sig_handler(int sig){
    forward = (-1)*forward; //toggle
    signal(sig, sig_handler);
}

int main() {

    forward = 1;
    char znak = 'A';
    struct sigaction act;
    act.sa_handler = au;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);

    signal(SIGTSTP, sig_handler);

    while(1) {
        printf("%c\n",znak);
        znak = znak + forward;
        if(znak > 'Z' && forward == 1)
            znak = 'A';
        if(znak < 'A' && forward == -1)
            znak = 'Z';
        sleep(1);

    }
}

