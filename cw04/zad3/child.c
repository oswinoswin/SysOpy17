#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

int COUNTER_SIGUSR1 = 0;
char isWaiting = 1;

void handler(int signo) {
    if (signo == SIGUSR1) {
        COUNTER_SIGUSR1++;
    } else if (signo == SIGUSR2) {
        isWaiting = 0;
    }
    else if (signo == SIGRTMIN) {
        COUNTER_SIGUSR1++;
    } else if (signo == SIGRTMIN+1) {
        isWaiting = 0;
    }
}

int main(int argc, char *argv[]) {
    printf("Child started\n");
    struct sigaction act;
    sigset_t emptySigset;
    pid_t pid = getppid();
    int i;

    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigemptyset(&emptySigset);

    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    sigaction(SIGRTMIN, &act, NULL);
    sigaction(SIGRTMIN+1, &act, NULL);

    /* Receiving signals */
    while(isWaiting) {
        printf("Child waiting\n");
        sigsuspend(&emptySigset);
        if (errno != EINTR) {
            perror("sigsuspend error");
            return -1;
        }
    }

    printf("CHILD: Received: %d\n", COUNTER_SIGUSR1);

    /* Sending signals */
    for (i = 0; i < COUNTER_SIGUSR1; i++) {
        if (kill(pid, SIGUSR1) == -1) {
            perror("kill error");
            exit(EXIT_FAILURE);
        }
    }

    if (kill(pid, SIGUSR2) == -1) {
        perror("kill error");
        exit(EXIT_FAILURE);
    }

    return 0;
}
