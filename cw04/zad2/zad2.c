#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <memory.h>

pid_t pid;
int status;
int asked = 0;
int K;
pid_t *children;
int perm;

static void hdl (int sig, siginfo_t *siginfo, void *context)
{
    children[asked] = siginfo->si_pid;
    asked++;
    printf ("Sending PID: %ld, UID: %ld, asked: %d\n",
            (long)siginfo->si_pid, (long)siginfo->si_uid, asked);


    if(asked == K){
        printf("We got this\n");
        for(int i=0; i<asked; i++){
            kill(children[i], SIGALRM);
        }
    }
    if(asked > K){
        kill(siginfo->si_pid, SIGALRM);
    }

}

void child_stopped(int sig){
    printf("child [%d] received signal %d\n", getpid(), sig);
    perm = 1;
}


int main(int argc, char *args[]) {
    int N;

    if(argc != 3) {

        fprintf(stderr, "usage: N K\n");
        exit(EXIT_FAILURE);
    }
    N = atoi(args[1]);
    K = atoi(args[2]);
    children = malloc(N*sizeof(pid_t));
    if(!children){
        fprintf(stderr, "malloc error\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i<N; i++){
        children[i] = -1;
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


    for(int i = 0; i<N; i++){
        printf("pid: %d\n");

        if ((pid = fork()) < 0) {
            fprintf(stderr, "fork error\n");
            exit(EXIT_FAILURE);
        }
        if(pid ==  0){
            perm = 0;
            printf("Hello from child! %d \n",i);
            sleep(3);
            kill(getppid(), SIGUSR1);
            printf("Child no %d sent signal\n", i);
            //signal(SIGUSR2, child_stopped);
            pause();
            printf("Childno %d finished\n", i);
            exit(0);
        }
        else{
            //children[i] = pid;
        }


    }


    while (asked < N) {

        printf("Alive. K: %d, asked: %d\n", K, asked);
        sleep(3);
    }

    if(pid != 0){
        for(int i = 0; i<N; i++){
            wait(&status);
            printf("Status: %d\n", status);
        }
    }

    return 0;
}