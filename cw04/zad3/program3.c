#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

int sendAndReceive(int);

int COUNTER_SIGUSR1 = 0;
char isWaiting = 1;

void handler(int signo) {
    if (signo == SIGUSR1) {
        COUNTER_SIGUSR1++;
    } else if (signo == SIGUSR2) {
        isWaiting = 0;
    }
}


int type;
int main(int argc, char *argv[]) {
    sigset_t sigset;
    int signalCount;

    if (argc != 3) {
        fprintf(stderr, "usage: [number of signals] [type]\n");
        exit(EXIT_FAILURE);
    }

    signalCount = atoi(argv[1]);
    type = atoi(argv[2]);

    /* Get currect signal mask */
    if (sigprocmask(0, NULL, &sigset) < 0) {
        perror("sigprocmask error");
        exit(EXIT_FAILURE);
    }

    /* Add SIGUSR1 and SIGUSR2 to blocked signals */
    if (sigaddset(&sigset, SIGUSR1) || sigaddset(&sigset, SIGUSR2) || sigaddset(&sigset, SIGRTMIN) || sigaddset(&sigset, SIGRTMIN+1)) {
        perror("sigaddset error");
        exit(EXIT_FAILURE);
    }

    /* Set new sginal mask */
    if (sigprocmask(SIG_SETMASK, &sigset, NULL) < 0) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    if (sendAndReceive(signalCount) == -1) {
        perror("sendAndReceive error");
        exit(EXIT_FAILURE);
    }

    printf("PARENT: Sent: %d\nPARENT: Received: %d\n", signalCount, COUNTER_SIGUSR1);

    return 0;
}

int sendAndReceive(int signalCount) {
    pid_t pid;
    int i;

    if ((pid = fork()) == -1) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
        /* Child process */
        if (execl("./child", "child", (char *)0) < 0){
            perror("execl error");
            return -1;
        }
    } else {
        /* Parent process */
        sigset_t emptySigset;
        struct sigaction act;

        act.sa_handler = handler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigemptyset(&emptySigset); //Empty mask to catch signals

        sigaction(SIGUSR1, &act, NULL);
        sigaction(SIGUSR2, &act, NULL);

        /* Sending signals */
        if(type == 1) {
            for (i = 0; i < signalCount; i++) {
                if (kill(pid, SIGUSR1) == -1) {
                    perror("kill error");
                    return -1;
                }
            }

            if (kill(pid, SIGUSR2) == -1) {
                perror("kill error");
                return -1;
            }
        }else if (type == 2){
            union sigval mysigval;
            for (i = 0; i < signalCount; i++) {
                if (sigqueue(pid, SIGUSR1, mysigval) == -1) {
                    perror("sigqueue error");
                    return -1;
                }
            }

            if (sigqueue(pid, SIGUSR2, mysigval) == -1) {
                perror("sigqueue error");
                return -1;
            }
        }else if(type == 3){
            union sigval mysigval;
            for (i = 0; i < signalCount; i++) {
                //printf("signaling\n");
                if (sigqueue(pid, SIGRTMIN, mysigval) == -1) {
                    perror("kill error");
                    return -1;
                }
            }

            if (sigqueue(pid, SIGRTMIN+1, mysigval) == -1) {
                perror("kill error");
                return -1;
            }
        }

        /* Receiving signals */
        while(isWaiting) {
            sigsuspend(&emptySigset);
            if (errno != EINTR) {
                perror("sigsuspend error");
                return -1;
            }
        }
    }

    return 0;
}

