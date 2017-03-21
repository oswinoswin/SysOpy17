#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <ftw.h>
#include <limits.h>
#include <string.h>

#define INITIAL_BUFF_SIZE 1024
int process(const char *, const struct stat *, int, struct FTW *);
char *displayRights(long rights);
static size_t bufflen = INITIAL_BUFF_SIZE;

static char *ptr;

long size;


int main(int argc, char *args[]) {

    int len;
    char *pathbuffer;

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
        if ((ptr = realpath(args[1], ptr)) == NULL) {
            fprintf(stderr, "realpath error\n");
            exit(EXIT_FAILURE);
        }
    }


    nftw(ptr, process, 512, FTW_PHYS);

    return 0;
}

int process(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    const time_t *time;
    const struct tm *date;
    if (typeflag == FTW_F) {
        if ( (long)sb->st_size <= size) {
            time = &(sb->st_atime);
            date = localtime(time);
            printf("%s,rights: %s,size: %ld,access time: %s", fpath,displayRights((long)sb->st_mode), (long)sb->st_size, asctime(date));
        }
    }

    return 0;
}

char *displayRights(long rights){
    char *res;
    long pom = rights - 32768;
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





