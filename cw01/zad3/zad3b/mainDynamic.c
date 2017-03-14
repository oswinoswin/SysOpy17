#include "libaddrbook.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdint.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <dlfcn.h>

struct timeval uStart, uEnd, sStart, sEnd, rEnd, rStart;
struct rusage ruStart, ruEnd;
long double uSum, sSum, rSum;

//dynamic library functions
static LIST *(*_createList)();
static void (*_lDeleteList)(LIST*);
static NODE *(*_createNode)(char*, char*, char*, char*, char*);
static void (*_lAddFront)(NODE*, LIST*);
static NODE *(*_lFindPerson)(LIST*, char*, char*);
static void (*_lPrintPerson)(NODE*);
static NODE *(*_lFindByIndex)( int, LIST*);
static void (*_lDeleteNode)(NODE*, LIST*);
static void (*_lSort)(LIST*, char*);

static TREE *(*_createTree)();
static TNODE *(*_tcreateNode)(char*, char*, char*, char*, char*);
static void (*_tAdd)(TNODE*, TREE*);
static void (*_tDeleteNode)(TNODE*, TREE*);
static TNODE *(*_tFindPerson)(char*, char*, TREE*);
static void (*_tDeleteTree)(TREE*);
static TREE *(*_tRebuild)(TREE*, char* );

static void *handle;
void *dlsymFunction(void *hanlde, const char* symbol);
void loadDynamicLib();

void startTime(){

    getrusage(RUSAGE_SELF, &ruStart);
    gettimeofday(&rStart, NULL);

}
void stopTime(){
    getrusage(RUSAGE_SELF, &ruEnd);
    gettimeofday(&rEnd, NULL);

    uStart = ruStart.ru_utime;
    uEnd = ruEnd.ru_utime;
    sStart = ruStart.ru_stime;
    sEnd = ruEnd.ru_stime;
}

void stopTimeAndPrint(){
    stopTime();
    printf("user: %09ld ms, system: %09ld ms, real: %09ld\n",
           (( uEnd.tv_sec - uStart.tv_sec)*1000000L
            + uEnd.tv_usec) - uStart.tv_usec,
           (( sEnd.tv_sec - sStart.tv_sec)*1000000L
            + sEnd.tv_usec) - sStart.tv_usec,
           (( rEnd.tv_sec - rStart.tv_sec)*1000000L
            + rEnd.tv_usec) - rStart.tv_usec);
}

int main(int argc, char *argv[])
{
    loadDynamicLib();
    srand(time(NULL));
    int iter = 1000;
    char firstName[BUFFER_LEN];
    char lastName[BUFFER_LEN];
    char birthDate[BUFFER_LEN];
    char email[BUFFER_LEN];
    char phone[BUFFER_LEN];

    printf("LIST:\n");
    printf("Creating address book:\n");
    startTime();
    LIST *list = (*_createList)();
    stopTimeAndPrint();



    printf("Adding an element:\n");
    uSum = 0; sSum = 0; rSum = 0;
    for(int i = 0; i<iter; i++) //generate list with random names
    {
        startTime();
        sprintf(firstName, "%d", i);
        sprintf(lastName, "%d", rand());
        sprintf(birthDate, "%d%d%d%d-%d-%d",1,9,rand()%10, rand()%10, rand()%12+1,rand()%31+1);
        sprintf(email,"%d@com.pl",rand());
        sprintf(phone,"%d%d%d-%d%d%d-%d%d%d", rand()%10, rand()%10, rand()%10, rand()%10,rand()%10, rand()%10, rand()%10,rand()%10,rand()%10);
        (*_lAddFront)((*_createNode)(firstName,lastName,birthDate,email,phone),list);
        stopTime();
        uSum += (( uEnd.tv_sec - uStart.tv_sec)*1000000L + uEnd.tv_usec) - uStart.tv_usec;
        sSum += (( sEnd.tv_sec - sStart.tv_sec)*1000000L + sEnd.tv_usec) - sStart.tv_usec;
        rSum += (( rEnd.tv_sec - rStart.tv_sec)*1000000L + rEnd.tv_usec) - rStart.tv_usec;

    }
    printf("user: %09ld ms, system: %09ld ms, real: %09ld\n", (long int)uSum/iter, (long int)sSum/iter,(long int) rSum/iter);




    printf("Deleting an element: optimistic: \n");
    startTime();
    (*_lDeleteNode)(list->head, list);
    stopTimeAndPrint();


    printf("Deleting an element: pesymistic: \n");
    NODE *tmp = (*_createNode)("1001", "a", "a","a","a");
    startTime();
    (*_lDeleteNode)(tmp, list);
    stopTimeAndPrint();


    printf("Searching for an element: optimistic:\n");
    startTime();
    (*_lFindByIndex)(0,list);
    stopTimeAndPrint();

    printf("Searching for an element: pesymistic:\n");
    startTime();
    (*_lFindByIndex)(1001,list);
    stopTimeAndPrint();

    printf("Sort:\n");
    startTime();
    (*_lSort)(list,"lastName");
    stopTimeAndPrint();

//-----------tree-------
    printf("TREE:\n");
    printf("Create tree:\n");
    startTime();
    TREE *tree = (*_createTree)();
    stopTimeAndPrint();

    //(*_tcreateNode)(firstName,lastName,birthDate,email,phone);



       printf("Adding an element:\n");
       uSum = 0; sSum = 0; rSum = 0;

       for(int i = 0; i<iter; i++){
           startTime();
           sprintf(firstName, "%d", i);
           sprintf(lastName, "%d", 10 - i);
           sprintf(birthDate, "%d%d%d%d-%d-%d",1,9,rand()%10, rand()%10, rand()%12+1,rand()%31+1);
           sprintf(email,"%d@com.pl",rand());
           sprintf(phone,"%d%d%d-%d%d%d-%d%d%d", rand()%10, rand()%10, rand()%10, rand()%10,rand()%10, rand()%10, rand()%10,rand()%10,rand()%10);
           (*_tAdd)((*_tcreateNode)(firstName,lastName,birthDate,email,phone),tree);
           stopTime();
           uSum += (( uEnd.tv_sec - uStart.tv_sec)*1000000L + uEnd.tv_usec) - uStart.tv_usec;
           sSum += (( sEnd.tv_sec - sStart.tv_sec)*1000000L + sEnd.tv_usec) - sStart.tv_usec;
           rSum += (( rEnd.tv_sec - rStart.tv_sec)*1000000L + rEnd.tv_usec) - rStart.tv_usec;

       }
       printf("user: %09ld ms, system: %09ld ms, real: %09ld\n", (long int)uSum/iter, (long int)sSum/iter,(long int) rSum/iter);

       printf("Deleting an element: optimistic: \n");
       startTime();
       (*_tDeleteNode)(tree->root, tree);

       stopTimeAndPrint();


       printf("Deleting an element: pesymistic: \n");
       TNODE *tnode1 = tree->root;
       while(tnode1-> left != NULL){
           tnode1 = tnode1->left;
       }

       startTime();
       (*_tDeleteNode)(tnode1, tree);
       stopTimeAndPrint();


       printf("Searching for an element: optimistic:\n");
       startTime();
       (*_tFindPerson)(tree->root->firstName, tree->root->lastName, tree);
       stopTimeAndPrint();


       printf("Searching for an element: pesymistic:\n");
       TNODE *tnode = (*_tcreateNode)("1001", "a","a","a","a");
       startTime();
       (*_tFindPerson)(tnode->firstName, tnode->lastName, tree);
       stopTimeAndPrint();

       printf("Rebuild:\n");
       startTime();
    (*_tRebuild)(tree, "lastName");
       stopTimeAndPrint();




    (*_tDeleteTree)(tree);
    (*_lDeleteList)(list);
    return 0;
}


void loadDynamicLib() {
    //opening and loading dynamic library

    handle = dlopen("libdynamic.so", RTLD_NOW);
    if(handle == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    dlerror();

    //loading functions
    _createList = dlsymFunction(handle, "createList");
    _lDeleteList = dlsymFunction(handle, "lDeleteList");
    _createNode = dlsymFunction(handle, "createNode");
    _lAddFront = dlsymFunction(handle, "lAddFront");
    _lFindPerson = dlsymFunction(handle, "lFindPerson");
    _lPrintPerson = dlsymFunction(handle, "lPrintPerson");
    _lFindByIndex = dlsymFunction(handle, "lFindByIndex");
    _lDeleteNode = dlsymFunction(handle, "lDeleteNode");
    _lSort = dlsymFunction(handle, "lSort");

    _createTree = dlsymFunction(handle, "createTree");
    _tcreateNode = dlsymFunction(handle, "tcreateNode");
    _tAdd = dlsymFunction(handle, "tAdd");
    _tDeleteNode = dlsymFunction(handle, "tDeleteNode");
    _tFindPerson = dlsymFunction(handle, "tFindPerson");
    _tRebuild = dlsymFunction(handle, "tRebuild");
    _tDeleteTree = dlsymFunction(handle, "tDeleteTree");
}

void *dlsymFunction(void *hanlde, const char* symbol) {
    void *tmp = dlsym(handle, symbol);
    char *error;

    if((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    return tmp;
}


