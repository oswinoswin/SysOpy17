#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>
#include <string.h>
#include <sched.h>
#include <stdint.h>

#define STACK_SIZE (1024 * 1024)
#define BILLION 1000000000L
#define MEGA 1000000L

void spawnChildren_fork(int);
void spawnChildren_vfork(int);
void spawnChildren_clone(int);
void spawnChildren_cloneFork(int);

static int childFun(void *args);
void normalizeTime();
void printTimes(int, char);

struct timespec *normalizeTimespec(struct timespec, struct timespec);
struct timeval *normalizeTimeval(struct timeval, struct timeval);

static int licznik = 0;

static struct rusage childUsageStart;
static struct rusage childUsageEnd;
static struct rusage parentUsageStart;
static struct rusage parentUsageEnd;

static struct timespec childrenTimeStart;
static struct timespec childrenTimeEnd;
static struct timespec parentTimeStart;
static struct timespec parentTimeEnd;
static struct timespec temp;

int main(int argc, char *args[]) {	
	int type;
	int number;
	char flag = 0;
	
	getrusage(RUSAGE_SELF, &parentUsageStart); 
	if (clock_gettime(CLOCK_MONOTONIC, &parentTimeStart) == -1) {
		fprintf(stderr, "clock gettime error");
		exit(EXIT_FAILURE);
	}
	
	if (argc < 3) {
		fprintf(stderr, "usage: [flag] <number of childern> <type>\n");
		exit(EXIT_FAILURE);
	}
	
	if (argc == 4) {
		if (strcmp("-t", args[1]) == 0) {
			flag = 1;
		}
		type = atoi(args[3]);
		number = atoi(args[2]);
	} else {
		type = atoi(args[2]);
		number = atoi(args[1]);
	}
	
	switch (type) {
		case 1:
			spawnChildren_fork(number);
			break;
		
		case 2:
			spawnChildren_vfork(number);
			break;
		
		case 3:
			spawnChildren_clone(number);
			break;
		
		case 4:
			 spawnChildren_cloneFork(number);
			break;
		
		default:
			fprintf(stderr, "wrong arguments\n");
	}

	if (clock_gettime(CLOCK_MONOTONIC, &parentTimeEnd) == -1) {
		fprintf(stderr, "clock gettime error");
		exit(EXIT_FAILURE);
	}

	getrusage(RUSAGE_SELF, &parentUsageEnd); 
	
	printTimes(number, flag);
	
	return 0;
}

void printTimes(int number, char flag) {
	struct timeval start;
	struct timeval stop;
	struct timespec *tmp;
	struct timeval *tmpVal;
	
	if (flag == 0) {
		fprintf(stdout, "Children count: %d\n", number);
		fprintf(stdout, "licznik: %d\n", licznik);
		
		/* Parent times */
		tmp = normalizeTimespec(parentTimeStart, parentTimeEnd);
		fprintf(stdout, "Parent:\n");
		fprintf(stdout, "Real: %ld.%09ld\n", tmp->tv_sec, tmp->tv_nsec);
		free(tmp);
		
		start = parentUsageStart.ru_utime;
		stop = parentUsageEnd.ru_utime;
		tmpVal = normalizeTimeval(start, stop);
		fprintf(stdout, "User: %ld.%06ld\n", tmpVal->tv_sec, tmpVal->tv_usec);
		free(tmpVal);

		start = parentUsageStart.ru_stime;
		stop = parentUsageEnd.ru_stime;
		tmpVal = normalizeTimeval(start, stop);
		fprintf(stdout, "Sys: %ld.%06ld\n", tmpVal->tv_sec, tmpVal->tv_usec);
		free(tmpVal);
		
		/* Children times */
		fprintf(stdout, "Children:\n");
		fprintf(stdout, "Real: %ld.%09ld\n", temp.tv_sec, temp.tv_nsec);
		
		start = childUsageStart.ru_utime;
		stop = childUsageEnd.ru_utime;
		tmpVal = normalizeTimeval(start, stop);
		fprintf(stdout, "User: %ld.%06ld\n", tmpVal->tv_sec, tmpVal->tv_usec);
		free(tmpVal);

		start = childUsageStart.ru_stime;
		stop = childUsageEnd.ru_stime;
		tmpVal = normalizeTimeval(start, stop);
		fprintf(stdout, "Sys: %ld.%06ld\n", tmpVal->tv_sec, tmpVal->tv_usec);
		free(tmpVal);
		
	} else if (flag == 1) {
		fprintf(stdout, "%d, ", number);
		
		/* Parent times */
		tmp = normalizeTimespec(parentTimeStart, parentTimeEnd);
		fprintf(stdout, "%ld.%09ld, ", tmp->tv_sec, tmp->tv_nsec);
		
		start = parentUsageStart.ru_utime;
		stop = parentUsageEnd.ru_utime;
		tmpVal = normalizeTimeval(start, stop);
		fprintf(stdout, "%ld.%06ld, ", tmpVal->tv_sec, tmpVal->tv_usec);

		start = parentUsageStart.ru_stime;
		stop = parentUsageEnd.ru_stime;
		tmpVal = normalizeTimeval(start, stop);
		fprintf(stdout, "%ld.%06ld, ", tmpVal->tv_sec, tmpVal->tv_usec);
		
		/* Children times */
		fprintf(stdout, "%ld.%09ld, ", temp.tv_sec, temp.tv_nsec);
		
		start = childUsageStart.ru_utime;
		stop = childUsageEnd.ru_utime;
		tmpVal = normalizeTimeval(start, stop);
		fprintf(stdout, "%ld.%06ld, ", tmpVal->tv_sec, tmpVal->tv_usec);

		start = childUsageStart.ru_stime;
		stop = childUsageEnd.ru_stime;
		tmpVal = normalizeTimeval(start, stop);
		fprintf(stdout, "%ld.%06ld\n", tmpVal->tv_sec, tmpVal->tv_usec);
	
	}
}

struct timespec *normalizeTimespec(struct timespec start, struct timespec end) {
	struct timespec *tmp = malloc(sizeof(struct timespec));
	
	if ((end.tv_nsec - start.tv_nsec) < 0) {
		tmp->tv_sec = end.tv_sec - start.tv_sec - 1;
		tmp->tv_nsec = BILLION + end.tv_nsec - start.tv_nsec;
	} else {
		tmp->tv_sec = end.tv_sec - start.tv_sec ;
		tmp->tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	
	return tmp;
}

struct timeval *normalizeTimeval(struct timeval start, struct timeval end) {
	struct timeval *tmp = malloc(sizeof(struct timeval));
	if ((end.tv_usec - start.tv_usec) < 0) {
		tmp->tv_sec = end.tv_sec - start.tv_sec - 1;
		tmp->tv_usec = MEGA + end.tv_usec - start.tv_usec;
	} else {
		tmp->tv_sec = end.tv_sec - start.tv_sec ;
		tmp->tv_usec = end.tv_usec - start.tv_usec;
	}
	
	return tmp;
}

void normalizeTime() {
	/* Normalize time */
	if ((childrenTimeEnd.tv_nsec - childrenTimeStart.tv_nsec) < 0) {
		temp.tv_sec += childrenTimeEnd.tv_sec - childrenTimeStart.tv_sec - 1;
		temp.tv_nsec += BILLION + childrenTimeEnd.tv_nsec - childrenTimeStart.tv_nsec;
	} else {
		temp.tv_sec += childrenTimeEnd.tv_sec - childrenTimeStart.tv_sec ;
		temp.tv_nsec += childrenTimeEnd.tv_nsec - childrenTimeStart.tv_nsec;
	}
	
	if (temp.tv_nsec / BILLION) {
		temp.tv_sec += temp.tv_nsec / BILLION;
		temp.tv_nsec %= BILLION;
	}
}

void spawnChildren_fork(int number) {
	pid_t pid;
	int i;
	int ptr;
	
	getrusage(RUSAGE_CHILDREN, &childUsageStart); 
	
	for (i = 0; i < number; i++) {
		if ((pid = fork()) == -1) {
			fprintf(stderr, "fail\n");
			return;
		} else if (pid == 0) {
			/* child process */
			licznik++;
			_exit(0);
		} else {
			/* parent process */
			if (clock_gettime(CLOCK_MONOTONIC, &childrenTimeStart) == -1) {
				fprintf(stderr, "clock gettime error");
				exit(EXIT_FAILURE);
			}
			
			waitpid(pid, &ptr, 0);
			
			if (clock_gettime(CLOCK_MONOTONIC, &childrenTimeEnd) == -1) {
				fprintf(stderr, "clock gettime error");
				exit(EXIT_FAILURE);
			}
			
			normalizeTime();

			if (!WIFEXITED(ptr)) {
				fprintf(stderr, "child err\n");
			}
		}
	}
	
	getrusage(RUSAGE_CHILDREN, &childUsageEnd);
}

void spawnChildren_vfork(int number) {
	pid_t pid;
	int i;
	
	getrusage(RUSAGE_CHILDREN, &childUsageStart); 
		
	for (i = 0; i < number; i++) {
		if ((pid = vfork()) == -1) {
			fprintf(stderr, "fail\n");
			return;
		} else if (pid == 0) {
			/* child process */
			licznik++;
			_exit(0);
		} else {
			/* parent process */
			
			if (clock_gettime(CLOCK_MONOTONIC, &childrenTimeStart) == -1) {
				fprintf(stderr, "clock gettime error");
				exit(EXIT_FAILURE);
			}
			
			waitpid(pid, NULL, 0);
			
			
			if (clock_gettime(CLOCK_MONOTONIC, &childrenTimeEnd) == -1) {
				fprintf(stderr, "clock gettime error");
				exit(EXIT_FAILURE);
			}
			
			normalizeTime();
		}
		
	}
	
	getrusage(RUSAGE_CHILDREN, &childUsageEnd);
}

static int childFun(void *args) {
	licznik++;
	_exit(0);
}

void spawnChildren_clone(int number) {
	char *stack;
	char *stackTop;
	int pid;
	int i;
	
	getrusage(RUSAGE_CHILDREN, &childUsageStart);
	
	for (i = 0; i < number; i++) {
		/* stack for child process */
		stack = malloc(STACK_SIZE);
		if (stack == NULL) {
			fprintf(stderr, "malloc error\n");
			exit(EXIT_FAILURE);
		}
		
		stackTop = stack + STACK_SIZE;
		
		pid = clone(childFun, stackTop, SIGCHLD, NULL);
		if (pid == -1) {
			fprintf(stderr, "clone error\n");
			exit(EXIT_FAILURE);
		}
		
		if (clock_gettime(CLOCK_MONOTONIC, &childrenTimeStart) == -1) {
			fprintf(stderr, "clock gettime error");
			exit(EXIT_FAILURE);
		}
		
		waitpid(pid, NULL, 0);
		free(stack);
		
		if (clock_gettime(CLOCK_MONOTONIC, &childrenTimeEnd) == -1) {
			fprintf(stderr, "clock gettime error");
			exit(EXIT_FAILURE);
		}
		
		normalizeTime();
	}

	getrusage(RUSAGE_CHILDREN, &childUsageEnd);
}

void spawnChildren_cloneFork(int number) {
	char *stack;
	char *stackTop;
	int pid;
	int i;
	
	getrusage(RUSAGE_CHILDREN, &childUsageStart);
	
	for (i = 0; i < number; i++) {
		/* stack for child process */
		stack = malloc(STACK_SIZE);
		if (stack == NULL) {
			fprintf(stderr, "malloc error\n");
			exit(EXIT_FAILURE);
		}
		
		stackTop = stack + STACK_SIZE;
		
		pid = clone(childFun, stackTop, SIGCHLD | CLONE_VFORK | CLONE_VM, NULL);
		if (pid == -1) {
			fprintf(stderr, "clone error\n");
			exit(EXIT_FAILURE);
		}
		
		if (clock_gettime(CLOCK_MONOTONIC, &childrenTimeStart) == -1) {
			fprintf(stderr, "clock gettime error");
			exit(EXIT_FAILURE);
		}
		
		waitpid(pid, NULL, 0);
		free(stack);
		
		if (clock_gettime(CLOCK_MONOTONIC, &childrenTimeEnd) == -1) {
			fprintf(stderr, "clock gettime error");
			exit(EXIT_FAILURE);
		}
		
		normalizeTime();
	}
	
	getrusage(RUSAGE_CHILDREN, &childUsageEnd);
}





//////

struct timeval *timeval_subtract (struct timeval *x, struct timeval *y)
{
	struct timeval *result = malloc(sizeof(struct timeval));
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_usec < y->tv_usec) {
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000) {
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait.
       tv_usec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	return result;
}
