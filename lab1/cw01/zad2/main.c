#include "../zad1/libaddrbook.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdint.h>


/* W programie zmierz i wypisz czasy realizacji podstawowych operacji:

- stworzenie ksiazki adresowej,

- dodanie pojedynczego elementu (czasy mierz dla dodawania po kolei kazdego elementu)

- usuwanie kontaktu z ksiazki (zmierz czasy dla przypadku optymistycznego oraz pesymistycznego)

- wyszukanie elementu w ksiazce (zmierz czasy dla przypadku optymistycznego oraz pesymistycznego)

- przesortowanie/przebudowanie ksiazki

Mierzac czasy pokaz trzy wartosci: czas rzeczywisty, czas uzytkownika i czas systemowy. */

/*
    realStart = time(NULL);

    realStop = time(NULL);
    realDiff = difftime(realStop, realStart);
    printf("Time: real: %f\n", realDiff/iter);
*/

/* Subtract the ‘struct timeval’ values X and Y,
        storing the result in RESULT.
Return 1 if the difference is negative, otherwise 0.
 http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html*/

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

    /* Return 1 if result is negative. */
    return result;
}


int main(int argc, char *argv[])
{

    time_t realStart; //real time
    time_t realStop;
    double realDiff;

    struct timeval tvalStart, tvalEnd;

    struct rusage ruStart, ruEnd;



    /*printf("RUSAGE :ru_utime => %lld [sec] : %lld [usec], :ru_stime => %lld [sec] : %lld [usec] ¥n",
           (int64_t)utime.tv_sec, (int64_t)utime.tv_usec,
           (int64_t)stime.tv_sec, (int64_t)stime.tv_usec);*/

    srand(time(NULL));
    int iter = 10000;
    char firstName[BUFFER_LEN];
    char lastName[BUFFER_LEN];
    char birthDate[BUFFER_LEN];
    char email[BUFFER_LEN];
    char phone[BUFFER_LEN];

    printf("Creating address book: list:\n");
    //realStart = time(NULL);

    getrusage(RUSAGE_SELF, &ruStart);
    LIST *list = createList();
    getrusage(RUSAGE_SELF, &ruEnd);

    tvalStart = ruStart.ru_utime;
    tvalEnd = ruEnd.ru_utime;
    printf("Time in microseconds: %09ld microseconds\n",
           ((tvalEnd.tv_sec - tvalStart.tv_sec)*1000000L
            +tvalEnd.tv_usec) - tvalStart.tv_usec);
   // realStop = time(NULL);
    //realDiff = difftime(realStop, realStart);


    getrusage(RUSAGE_SELF, &ruStart);
    printf("Time: real: %f\n", realDiff);

    printf("Adding an element to list:\n");
    for(int i = 0; i<iter; i++) //generate list with random names
    {
        sprintf(firstName, "%d", i);
        sprintf(lastName, "%d", rand());
        sprintf(birthDate, "%d%d%d%d-%d-%d",1,9,rand()%10, rand()%10, rand()%12+1,rand()%31+1);
        sprintf(email,"%d@com.pl",rand());
        sprintf(phone,"%d%d%d-%d%d%d-%d%d%d", rand()%10, rand()%10, rand()%10, rand()%10,rand()%10, rand()%10, rand()%10,rand()%10,rand()%10);
        lAddFront(createNode(firstName,lastName,birthDate,email,phone),list);
        realStop = time(NULL);

    }
    getrusage(RUSAGE_SELF, &ruEnd);
    tvalStart = ruStart.ru_utime;
    tvalEnd = ruEnd.ru_utime;
    printf("Time in microseconds: %ld microseconds\n",
           ((tvalEnd.tv_sec - tvalStart.tv_sec)*1000000L
            +tvalEnd.tv_usec) - tvalStart.tv_usec);

    printf("Time: real: %f\n", realDiff/iter);

    printf("Deleting an element: optimistic: \n");
    realStart = time(NULL);
    lDeleteNode(list->head, list);
    realStop = time(NULL);
    realDiff = difftime(realStop, realStart);
    printf("Time: real: %f\n", realDiff);

    printf("Deleting an element: pesymistic: \n");
    NODE *tmp = createNode("1001", "a", "a","a","a");
    realStart = time(NULL);
    lDeleteNode(tmp, list);
    realStop = time(NULL);
    realDiff = difftime(realStop, realStart);
    printf("Time: real: %f\n", realDiff);

    printf("Searching for an element: optimistic:\n");
    printf("Searching for an element: pesymistic:\n");
    printf("Sorting list:\n");
//-----------tree-------
    clock_t start = ((1000*clock())/CLOCKS_PER_SEC);
    TREE *tree = createTree();



    for(int i = 0; i<iter; i++){
        sprintf(firstName, "%d", i);
        sprintf(lastName, "%d", 10 - i);
        sprintf(birthDate, "%d%d%d%d-%d-%d",1,9,rand()%10, rand()%10, rand()%12+1,rand()%31+1);
        sprintf(email,"%d@com.pl",rand());
        sprintf(phone,"%d%d%d-%d%d%d-%d%d%d", rand()%10, rand()%10, rand()%10, rand()%10,rand()%10, rand()%10, rand()%10,rand()%10,rand()%10);
        tAdd(tcreateNode(firstName,lastName,birthDate,email,phone),tree);
        //tPrintInOrder(tree);
    }






    tDeleteTree(tree);
    lDeleteList(list);
    return 0;
}
