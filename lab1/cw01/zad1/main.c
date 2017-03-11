#include "libaddrbook.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int iter = 1000;
    char firstName[BUFFER_LEN];
    char lastName[BUFFER_LEN];
    char birthDate[BUFFER_LEN];
    char email[BUFFER_LEN];
    char phone[BUFFER_LEN];

    NODE *tmp;
    LIST *list = createList();

    for(int i = 0; i<iter; i++) //generate list with random names
    {
        sprintf(firstName, "%d", i);
        sprintf(lastName, "%d", rand());
        sprintf(birthDate, "%d%d%d%d-%d-%d",1,9,rand()%10, rand()%10, rand()%12+1,rand()%31+1);
        sprintf(email,"%d@com.pl",rand());
        sprintf(phone,"%d%d%d-%d%d%d-%d%d%d", rand()%10, rand()%10, rand()%10, rand()%10,rand()%10, rand()%10, rand()%10,rand()%10,rand()%10);
        lAddFront(createNode(firstName,lastName,birthDate,email,phone),list);

    }
//-----------tree-------
    TREE *tree = createTree();

    for(int i = 0; i<iter; i++){
        sprintf(firstName, "%d", i);
        sprintf(lastName, "%d", 10 - i);
        sprintf(birthDate, "%d%d%d%d-%d-%d",1,9,rand()%10, rand()%10, rand()%12+1,rand()%31+1);
        sprintf(email,"%d@com.pl",rand());
        sprintf(phone,"%d%d%d-%d%d%d-%d%d%d", rand()%10, rand()%10, rand()%10, rand()%10,rand()%10, rand()%10, rand()%10,rand()%10,rand()%10);
        tAdd(tcreateNode(firstName,lastName,birthDate,email,phone),tree);
    }

    tPrintInOrder(tree);
    printf("delete 1\n");
    tDeleteNode(tFindPerson("1","9",tree),tree);
    tPrintInOrder(tree);


    tDeleteTree(tree);
    lDeleteList(list);
    return 0;
}









