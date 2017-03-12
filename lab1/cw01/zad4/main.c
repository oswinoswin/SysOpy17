#include "../zad1/libaddrbook.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdint.h>

struct timeval uStart, uEnd, sStart, sEnd, rEnd, rStart;
struct rusage ruStart, ruEnd;
long double uSum, sSum, rSum;

/* W programie zmierz i wypisz czasy realizacji podstawowych operacji:

- stworzenie ksiazki adresowej,

- dodanie pojedynczego elementu (czasy mierz dla dodawania po kolei kazdego elementu)

- usuwanie kontaktu z ksiazki (zmierz czasy dla przypadku optymistycznego oraz pesymistycznego)

- wyszukanie elementu w ksiazce (zmierz czasy dla przypadku optymistycznego oraz pesymistycznego)

- przesortowanie/przebudowanie ksiazki

Mierzac czasy pokaz trzy wartosci: czas rzeczywisty, czas uzytkownika i czas systemowy. */

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
    printf("main4\n");
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
    LIST *list = createList();
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
        lAddFront(createNode(firstName,lastName,birthDate,email,phone),list);
        stopTime();
        uSum += (( uEnd.tv_sec - uStart.tv_sec)*1000000L + uEnd.tv_usec) - uStart.tv_usec;
        sSum += (( sEnd.tv_sec - sStart.tv_sec)*1000000L + sEnd.tv_usec) - sStart.tv_usec;
        rSum += (( rEnd.tv_sec - rStart.tv_sec)*1000000L + rEnd.tv_usec) - rStart.tv_usec;

    }
    printf("user: %09ld ms, system: %09ld ms, real: %09ld\n", (long int)uSum/iter, (long int)sSum/iter,(long int) rSum/iter);




    printf("Deleting an element: optimistic: \n");
    startTime();
    lDeleteNode(list->head, list);
    stopTimeAndPrint();


    printf("Deleting an element: pesymistic: \n");
    NODE *tmp = createNode("1001", "a", "a","a","a");
    startTime();
    lDeleteNode(tmp, list);
    stopTimeAndPrint();


    printf("Searching for an element: optimistic:\n");
    startTime();
    lFindByIndex(0,list);
    stopTimeAndPrint();

    printf("Searching for an element: pesymistic:\n");
    startTime();
    lFindByIndex(1001,list);
    stopTimeAndPrint();

    printf("Sort:\n");
    startTime();
    lSort(list,"lastName");
    stopTimeAndPrint();

//-----------tree-------
    printf("TREE:\n");
    printf("Create tree:\n");
    startTime();
    TREE *tree = createTree();
    stopTimeAndPrint();



    printf("Adding an element:\n");
    uSum = 0; sSum = 0; rSum = 0;

    for(int i = 0; i<iter; i++){
        startTime();
        sprintf(firstName, "%d", i);
        sprintf(lastName, "%d", 10 - i);
        sprintf(birthDate, "%d%d%d%d-%d-%d",1,9,rand()%10, rand()%10, rand()%12+1,rand()%31+1);
        sprintf(email,"%d@com.pl",rand());
        sprintf(phone,"%d%d%d-%d%d%d-%d%d%d", rand()%10, rand()%10, rand()%10, rand()%10,rand()%10, rand()%10, rand()%10,rand()%10,rand()%10);
        tAdd(tcreateNode(firstName,lastName,birthDate,email,phone),tree);
        stopTime();
        uSum += (( uEnd.tv_sec - uStart.tv_sec)*1000000L + uEnd.tv_usec) - uStart.tv_usec;
        sSum += (( sEnd.tv_sec - sStart.tv_sec)*1000000L + sEnd.tv_usec) - sStart.tv_usec;
        rSum += (( rEnd.tv_sec - rStart.tv_sec)*1000000L + rEnd.tv_usec) - rStart.tv_usec;

    }
    printf("user: %09ld ms, system: %09ld ms, real: %09ld\n", (long int)uSum/iter, (long int)sSum/iter,(long int) rSum/iter);

    printf("Deleting an element: optimistic: \n");
    startTime();
    tDeleteNode(tree->root, tree);

    stopTimeAndPrint();


    printf("Deleting an element: pesymistic: \n");
    TNODE *tnode1 = tree->root;
    while(tnode1-> left != NULL){
        tnode1 = tnode1->left;
    }

    startTime();
    tDeleteNode(tnode1, tree);
    stopTimeAndPrint();


    printf("Searching for an element: optimistic:\n");
    startTime();
    tFindPerson(tree->root->firstName, tree->root->lastName, tree);
    stopTimeAndPrint();


    printf("Searching for an element: pesymistic:\n");
    TNODE *tnode = tcreateNode("1001", "a","a","a","a");
    startTime();
    tFindPerson(tnode->firstName, tnode->lastName, tree);
    stopTimeAndPrint();

    printf("Rebuild:\n");
    startTime();
    tRebuild(tree, "lastName");
    stopTimeAndPrint();




    tDeleteTree(tree);
    tDeleteTree(tree); //generate core dump
    lDeleteList(list);
    return 0;
}
