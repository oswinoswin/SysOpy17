#include "libaddrbook.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


void tDeleteNode(TNODE *tnode, TREE *tree);
void tRebuild(TREE *tree, char *key);
TNODE *tInsertWithKey(TNODE *tnode, TNODE *root, char *key);


int main()
{
    srand(time(NULL));
    int iter = 10;
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


    for(int i = 0; i<10; i++){
        sprintf(firstName, "%d", i);
        sprintf(lastName, "%d", 10 - i);
        sprintf(birthDate, "%d%d%d%d-%d-%d",1,9,rand()%10, rand()%10, rand()%12+1,rand()%31+1);
        sprintf(email,"%d@com.pl",rand());
        sprintf(phone,"%d%d%d-%d%d%d-%d%d%d", rand()%10, rand()%10, rand()%10, rand()%10,rand()%10, rand()%10, rand()%10,rand()%10,rand()%10);
        tAdd(tcreateNode(firstName,lastName,birthDate,email,phone),tree);
    }

    //tPrintInOrder(tree);
    tRebuild(tree, "lastName");
    //tPrintInOrder(tree);


    tDeleteTree(tree);

    lDeleteList(list);
    return 0;
}

TNODE *tInsertWithKey(TNODE *tnode, TNODE *root, char *key){
    if(tnode != NULL && root != NULL){
        if(tCompare2(tnode,root,key) < 0){
            TNODE *curr = tInsert(tnode, root->left);
            root->left = curr;
            curr->parent = root;
        }
        else {
            TNODE *curr = tInsert(tnode, root->right);
            root->right = curr;
            curr->parent = root;
        }
    }
    else {
        root = tnode;
    }
    return root;
}

void tAddWithKey(TNODE *tnode, TREE *tree, char *key){
    if(tnode != NULL && tree != NULL){
        tree->root = tInsertWithKey(tnode, tree->root, key);
    }
}

void tRebuild(TREE *tree, char *key){

    LIST *list = createList();
    TREE *tmp = createTree();
    TNODE *tmp2 = NULL;
    setTreeKey(key,tree);
    list = treeToList(tree,list);
    if(list != NULL) {
        NODE *node = NULL;
        if(list->head != NULL) {
            node = list->head;
            while(node->next != NULL) {
                tmp2 = tcreateNode(node->firstName, node->lastName, node->birthDate, node->email, node->phone);
                tAddWithKey(tmp2, tmp, key);
                node = node->next;
            }

        }
    }
    printf("Tree after rebuild: ");
    tPrintInOrder(tmp);
    printf("czupakabra");
    lDeleteList(list);
    tDeleteTree(tree);
    tree = tmp;

}





