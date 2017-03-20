#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define COMMANDS_COUNT 9
#define COMMANDS_BUFF_SIZE 64

struct flock *fun(short type, short whence, off_t start, off_t len);
int setLock(int fileDesc, off_t offset, short type);
int unsetLock(int fileDesc, off_t offset);
int readByte(int fileDesc, off_t offset, char *character);
int writeByte(int fileDesc, off_t offset, char *character);
void listLocks(int fileDesc);
void printHelp(char commands[][COMMANDS_BUFF_SIZE]);
int setLockBlocking(int fileDesc, off_t offset, short type);

int main(int argc, char *args[]) {
    int fileDesc;
    short runFlag;
    off_t offset;

    char *command;
    char character;

    command = malloc(sizeof(char) * 16);
    if (command == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(EXIT_FAILURE);
    }

    char commands[COMMANDS_COUNT][COMMANDS_BUFF_SIZE] =	{
            "swrlck - set write lock\0",
            "swrlckb - set write lock (blocking)\0",
            "srdlck - set read lock\0",
            "srdlckb - set read lock (blocking)\0",
            "lslck - list existing locks\0",
            "uslck - unset lock",
            "write - write char",
            "read - read char",
            "exit - exit program\0",
    };


    // Check args and open file
    if (argc != 2) {
        fprintf(stderr, "usage: <file path>\n");
        exit(EXIT_FAILURE);
    }

    if ((fileDesc = open(args[1], O_RDWR)) == -1) {
        fprintf(stderr, "open error\n");
        exit(EXIT_FAILURE);
    }

    // Interactive loop start
    runFlag = 1;
    printf("Choose command (type 'help' for command list)\n");
    while(runFlag) {
        printf(">> ");
        scanf("%s", command);

        if (strcmp(command, "help") == 0) {
            printHelp(commands);
        } else if (strcmp(command, "swrlck") == 0) {
            printf("Specify byte position: ");
            scanf("%ld", &offset);
            if(setLock(fileDesc, offset, F_WRLCK) == -1) {
                fprintf(stderr, "setLock error\n");
                //exit(EXIT_FAILURE);
            }

        }else if (strcmp(command, "swrlckb") == 0) {
            printf("Specify byte position: ");
            scanf("%ld", &offset);
            if(setLockBlocking(fileDesc, offset, F_WRLCK) == -1) {
                fprintf(stderr, "setLock error\n");
                //exit(EXIT_FAILURE);
            }

        } else if (strcmp(command, "srdlck") == 0) {
            printf("Specify byte position: ");
            scanf("%ld", &offset);
            if(setLock(fileDesc, offset, F_RDLCK) == -1) {
                fprintf(stderr, "setLock error\n");
                //exit(EXIT_FAILURE);
            }

        }else if (strcmp(command, "srdlckb") == 0) {
            printf("Specify byte position: ");
            scanf("%ld", &offset);
            if(setLockBlocking(fileDesc, offset, F_RDLCK) == -1) {
                fprintf(stderr, "setLock error\n");
                //exit(EXIT_FAILURE);
            }

        } else if (strcmp(command, "lslck") == 0) {
            listLocks(fileDesc);

        } else if (strcmp(command, "uslck") == 0) {
            printf("Specify byte position: ");
            scanf("%ld", &offset);
            if (unsetLock(fileDesc, offset) == -1) {
                fprintf(stderr, "setLock error\n");
                //exit(EXIT_FAILURE);
            }

        } else if (strcmp(command, "write") == 0) {
            printf("Specify byte position and byte to write: ");
            scanf("%ld %c", &offset, &character);
            writeByte(fileDesc, offset, &character);

        } else if (strcmp(command, "read") == 0) {
            printf("Specify byte position: ");
            scanf("%ld%*[^\n]%*c", &offset);
            readByte(fileDesc, offset, &character);
            printf("Character: %c\n", character);

        } else if (strcmp(command, "exit") == 0) {
            runFlag = 0;

        } else {
            printf("undefined command\n");
        }
    }

    // Close file
    if (close(fileDesc) == -1) {
        fprintf(stderr, "close error\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int setLock(int fileDesc, off_t offset, short type) {
    struct flock *fPointer;

    if (lseek(fileDesc, 0, SEEK_END) < offset) {
        fprintf(stderr, "setLock: invalid offset\n");
        return -1;
    }

    fPointer = fun(type, SEEK_SET, offset, 1);
    if (fPointer == NULL) {
        fprintf(stderr, "fun error\n");
        return -1;
    }

    if (fcntl(fileDesc, F_SETLK, fPointer) == -1) {
        fprintf(stderr, "fcntl error\n");
        return -1;
    }
    return 0;
}


int setLockBlocking(int fileDesc, off_t offset, short type) {
    struct flock *fPointer;

    if (lseek(fileDesc, 0, SEEK_END) < offset) {
        fprintf(stderr, "setLock: invalid offset\n");
        return -1;
    }

    fPointer = fun(type, SEEK_SET, offset, 1);
    if (fPointer == NULL) {
        fprintf(stderr, "fun error\n");
        return -1;
    }

    if (fcntl(fileDesc, F_SETLKW, fPointer) == -1) {
        fprintf(stderr, "fcntl error\n");
        return -1;
    }
    return 0;
}

int unsetLock(int fileDesc, off_t offset) {
    struct flock *fPointer;

    if (lseek(fileDesc, 0, SEEK_END) < offset) {
        fprintf(stderr, "unsetLock: invalid offset\n");
        return -1;
    }

    fPointer = fun(F_UNLCK, SEEK_SET, offset, 1);
    if (fPointer == NULL) {
        fprintf(stderr, "fun error\n");
        return -1;
    }

    if (fcntl(fileDesc, F_SETLK, fPointer) == -1) {
        fprintf(stderr, "fcntl error\n");
        return -1;
    }

    return 0;
}

void listLocks(int fileDesc) {
    off_t currOffset = 0;
    off_t endOffset;
    if ((endOffset = lseek(fileDesc, 0, SEEK_END)) == -1) {
        fprintf(stderr, "lseek error\n");
        return;
    }

    struct flock *fPointer;
    fPointer = fun(F_WRLCK, SEEK_SET, currOffset, 1);
    if (fPointer == NULL) {
        fprintf(stderr, "fun error\n");
        return;
    }

    for(currOffset = 0; currOffset < endOffset; currOffset++) {
        fPointer->l_type = F_WRLCK;
        fPointer->l_whence = SEEK_SET;
        fPointer->l_start = currOffset;
        fPointer->l_len = 1;

        if (fcntl(fileDesc, F_GETLK, fPointer) == -1) {
            fprintf(stderr, "fcntl error\n");
            return;
        }

        if (fPointer->l_type == F_RDLCK) {
            printf("Lock - byte: %ld, type: RDLCK, pid: %ld\n", (long)currOffset, (long)fPointer->l_pid);
        } else if (fPointer->l_type == F_WRLCK) {
            printf("Lock - byte: %ld, type: WRLCK, pid: %ld\n", (long)currOffset, (long)fPointer->l_pid);
        }
    }
}

int readByte(int fileDesc, off_t offset, char *character) {

    if(setLock(fileDesc, offset, F_RDLCK) == -1) {
        fprintf(stderr, "setLock error\n");
        return -4;
    }

    if (lseek(fileDesc, 0, SEEK_END) < offset || character == NULL) {
        fprintf(stderr, "readByte error\n");
        return -1;
    }

    if (lseek(fileDesc, offset, SEEK_SET) == -1) {
        fprintf(stderr, "lseek error\n");
        return -2;
    }

    if (read(fileDesc, character, 1) == -1) {
        fprintf(stderr, "read error\n");
        return -3;
    }

    return 0;
}

int writeByte(int fileDesc, off_t offset, char *character) {

    if(setLock(fileDesc, offset, F_WRLCK) == -1) {
        fprintf(stderr, "setLock error\n");
        return -4;
    }

    if (lseek(fileDesc, 0, SEEK_END) < offset || character == NULL) {
        fprintf(stderr, "writeByte error\n");
        return -1;
    }

    if (lseek(fileDesc, offset, SEEK_SET) == -1) {
        fprintf(stderr, "lseek error\n");
        return -2;
    }

    if (write(fileDesc, character, 1) == -1) {
        fprintf(stderr, "write error\n");
        return -3;
    }

    return 0;
}

struct flock *fun(short type, short whence, off_t start, off_t len) {
    struct flock *tmp;
    tmp = malloc(sizeof(struct flock));
    if (tmp == NULL) {
        fprintf(stderr, "malloc error\n");
        return NULL;
    }

    tmp->l_type = type;
    tmp->l_start = start;
    tmp->l_whence = whence;
    tmp->l_len = len;
    tmp->l_pid = getpid();

    return tmp;
}

void printHelp(char commands[][64]) {
    int i;
    for(i = 0; i < COMMANDS_COUNT; i++) {
        printf("%s\n", commands[i]);
    }
}
