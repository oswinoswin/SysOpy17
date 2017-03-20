#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <libgen.h>
#include <sys/types.h>
#include <values.h>

#define INITIAL_BUFF_SIZE 1024
char * displayRights(mode_t rights);

void traverseFiles(char *pathbuffer, char *name, long size);


static size_t bufflen = INITIAL_BUFF_SIZE;


int main(int argc, char *args[]) {
    char *pathbuffer;
    int len;

    long size;

    if (argc != 3) {
        fprintf(stderr, "usage: <file path> <max size in bythes>\n");
        exit(EXIT_FAILURE);
    }

    size = atol(args[2]);

    len = strlen(args[1]);
    if (len > bufflen) {
        bufflen = len;
    }

    if ((pathbuffer = malloc(sizeof(char) * bufflen)) == NULL) {
        fprintf(stderr, "malloc error: pathbuffer\n");
        exit(EXIT_FAILURE);
    }

    if (args[1][0] != '/') {
        /* relative path - get absolute path */
        if ((pathbuffer = realpath(args[1], pathbuffer)) == NULL) {
            fprintf(stderr, "realpath error\n");
            exit(EXIT_FAILURE);
        }
    }

    //strcpy(pathbuffer, args[1]);
    traverseFiles(pathbuffer, basename(pathbuffer), size);

    return 0;
}

void traverseFiles(char *pathbuffer, char *path, long size) {
    struct stat fStat;
    struct dirent *dirp;
    struct tm *date;
    time_t *time;
    DIR *dir;
    int len;

    if (lstat(pathbuffer, &fStat) == -1) {
        /* error - can't access */
        return;
    } else if (S_ISREG(fStat.st_mode) != 0) {
        /* is regular file */
        if ((long)fStat.st_size <= size) {
            time = &(fStat.st_atim.tv_sec);
            date = localtime(time);
            printf("%s, %ld bytes, rights %s, last accessed %s",
                   pathbuffer, (long)fStat.st_size,displayRights(fStat.st_mode), asctime(date));
        }
        return;
    } else if (S_ISDIR(fStat.st_mode) == 0) {
        /* not directory nor regular file */
        return;
    }

    /* is directory */
    len = strlen(pathbuffer);
    if (len + 2 + NAME_MAX > bufflen) {
        bufflen *= 2;
        if ((pathbuffer = (char *)realloc(pathbuffer, bufflen)) == NULL) {
            fprintf(stderr, "realloc error\n");
        }
    }
    pathbuffer[len] = '/';
    len++;
    pathbuffer[len] = 0;

    if ((dir = opendir(pathbuffer)) == NULL) {
        fprintf(stderr, "can't open\n");
        return;
    }

    while ((dirp = readdir(dir)) != NULL) {
        if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
            strcpy(&pathbuffer[len], dirp->d_name);
            traverseFiles(pathbuffer, path, size);
        }
    }

    if (closedir(dir) == -1) {
        fprintf(stderr, "can't close dir\n");
    }
}

char *displayRights(mode_t rights){
    char *res;
    int pom = (int)rights - 32768;
    if ((res = malloc(sizeof(char) * 1)) == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(EXIT_FAILURE);
    }
    strcpy(res,"-");

    if(pom & (1 << 8)) {
        res = strcat(res, "r");
    }
    else{
        res = strcat(res, "-");
    }

    if(pom & (1 << 7)) {
        res = strcat(res, "w");
    }
    else{
        res = strcat(res, "-");
    }

    if(pom & (1 << 6)) {
        res = strcat(res, "x");
    }
    else{
        res = strcat(res, "-");
    }

    if(pom & (1 << 5)) {
        res = strcat(res, "r");
    }
    else{
        res = strcat(res, "-");
    }

    if(pom & (1 << 4)) {
        res = strcat(res, "w");
    }
    else{
        res = strcat(res, "-");
    }

    if(pom & (1 << 3)) {
        res = strcat(res, "x");
    }
    else{
        res = strcat(res, "-");
    }


    if(pom & (1 << 2)) {
        res = strcat(res, "r");
    }
    else{
        res = strcat(res, "-");
    }

    if(pom & (1 << 1)) {
        res = strcat(res, "w");
    }
    else{
        res = strcat(res, "-");
    }

    if(pom & (1 << 0)) {
        res = strcat(res, "x");
    }
    else{
        res = strcat(res, "-");
    }
    return res;

}





