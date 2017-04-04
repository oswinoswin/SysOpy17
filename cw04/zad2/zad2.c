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

    printf ("PARENT received Sending PID: %ld, UID: %ld, asked: %d\n",
            (long)siginfo->si_pid, (long)siginfo->si_uid, asked);

    //kill(siginfo->si_pid, SIGUSR2);
   // printf("parent sendt response\n");

    /*if(asked == K){
        printf("We got this\n");
        for(int i=0; i<asked; i++){
            kill(children[i], SIGALRM);
        }
    }
    if(asked > K){
        kill(siginfo->si_pid, SIGALRM);
    }*/
}

static void hdl2(int sig, siginfo_t *siginfo, void *context)
{
    printf ("Parent received goodbye from child: Sending PID: %ld, UID: %ld, asked: %d\n",
            (long)siginfo->si_pid, (long)siginfo->si_uid, asked);
}

void child_stopped(int sig){
    printf("child [%d] received signal %d\n", getpid(), sig);
    perm = 1;
}

int main(void)
{
    pid_t child_pid, wpid;
    int status = 0;
    int i;
    int a[3] = {1, 2, 1};

    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    //Use the sa_sigaction field because the handles has two additional parameters
    act.sa_sigaction = &hdl;
    //The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler.
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        perror ("sigaction");
        return 1;
    }

    printf("parent_pid = %d\n", getpid());
    for (i = 0; i < 3; i++)
    {
        printf("i = %d\n", i);
        if ((child_pid = fork()) == 0)
        {
            printf("In child process (pid = %d)\n", getpid());
            if (a[i] < 2)
            {
                sleep(3);
                kill(getppid(), SIGUSR1);
                printf("Should be accept\n");

                exit(1);

            }
            else
            {
                printf("Should be reject\n");
                exit(0);
            }
            /*NOTREACHED*/
        }
    }




    while ((wpid = wait(&status)) > 0)
    {
        printf("Exit status of %d was %d (%s)\n", (int)wpid, status,
               (status > 0) ? "accept" : "reject");
        //sleep(2);
    }

    return 0;
}

/*
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
    //Use the sa_sigaction field because the handles has two additional parameters
    act.sa_sigaction = &hdl;
    //The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler.
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        perror ("sigaction");
        return 1;
    }

    struct sigaction act2;
    memset(&act2, '\0', sizeof(act2));
    act2.sa_sigaction = &hdl2;
    act2.sa_flags = SA_SIGINFO;
    if (sigaction(SIGRTMIN, &act2, NULL) < 0) {
        perror ("sigaction2");
        return 1;
    }


    for(int i = 0; i<N; i++){
        printf("pid: %d\n");

        if ((pid = fork()) < 0) {
            fprintf(stderr, "fork error\n");
            exit(EXIT_FAILURE);
        }
        if(pid ==  0){

            printf("Hello from child! %d  pid: %d\n",i, getpid());
            kill(getppid(), SIGUSR1);
            printf("ooo macarena\n");
            //sleep(1);
            printf("Child no %d finished\n", getpid());
            exit(0);
        }
        else{
            //children[i] = pid;
        }

    }
    printf("LOOP ENDED\n");


    while(1){
        sleep(3);
        printf("asked: %d\n", asked);
        //wait(&status);
    }

  
    if(pid != 0){
        for(int i = 0; i<asked; i++){

        }
    }

    return 0;
}*/