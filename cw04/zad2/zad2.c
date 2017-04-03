#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <memory.h>

pid_t pid;
int status;

static void hdl (int sig, siginfo_t *siginfo, void *context)
{
    printf ("Sending PID: %ld, UID: %ld\n",
            (long)siginfo->si_pid, (long)siginfo->si_uid);
}


int main(int argc, char *args[]) {
    int N;
    int K;
    pid_t *children;
    if(argc != 3) {

        fprintf(stderr, "usage: N K\n");
        exit(EXIT_FAILURE);
    }
    N = atoi(args[1]);
    K = atoi(args[1]);
    children = malloc(N*sizeof(pid_t));
    if(!children){
        fprintf(stderr, "malloc error\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i<N; i++){
        printf("pid: %d\n");

        if ((pid = fork()) < 0) {
            fprintf(stderr, "fork error\n");
            exit(EXIT_FAILURE);
        }
        if(pid ==  0){
            printf("Hello from child! %d parent: %d\n", pid, getppid());
            kill(getppid(), SIGUSR1);
            return 0;
        }
        else{
            children[i] = pid;
        }


    }

    if(pid != 0){
        for(int i = 0; i<N; i++){
            wait(&status);
            printf("Status: %d\n", status);
        }
    }


    struct sigaction act;

    memset(&act, '\0', sizeof(act));

    /* Use the sa_sigaction field because the handles has two additional parameters */
    act.sa_sigaction = &hdl;

    /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
    act.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        perror ("sigaction");
        return 1;
    }

    while (1) {
        printf("Alive.\n");
        sleep(3);
    }
    return 0;
}