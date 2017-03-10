#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libaddrbook.h"

LIST *createList() {
    LIST *tmp = malloc(sizeof(LIST));
    if(tmp == NULL) {
        fprintf(stderr, "Malloc failed!\n");
        exit(EXIT_FAILURE);
    }
    tmp->head = NULL;
    tmp->tail = NULL;
    return tmp;
}

NODE *createNode(char *firstName, char *lastName, char *birthDate,
                 char *email, char *phone) {
    NODE *tmp = malloc(sizeof(NODE));
    if(tmp == NULL) {
        fprintf(stderr, "Malloc failed!\n");
        exit(EXIT_FAILURE);
    }

    memset(tmp->firstName, '\0', sizeof(tmp->firstName)); //allpcate memory
    strncpy(tmp->firstName, firstName, BUFFER_LEN); //save values

    memset(tmp->lastName, '\0', sizeof(tmp->lastName));
    strncpy(tmp->lastName, lastName, BUFFER_LEN);

    memset(tmp->birthDate, '\0', sizeof(tmp->birthDate));
    strncpy(tmp->birthDate, birthDate, BUFFER_LEN);

    memset(tmp->email, '\0', sizeof(tmp->email));
    strncpy(tmp->email, email, BUFFER_LEN);

    memset(tmp->phone, '\0', sizeof(tmp->phone));
    strncpy(tmp->phone, phone, BUFFER_LEN);

    tmp->next = NULL;
    tmp->prev = NULL;

    return tmp;
}

void lAddFront(NODE *node, LIST *list) {
    if(node != NULL && list != NULL) {
        if(list->head != NULL) {
            node->next = list->head;
            list->head->prev = node;
            list->head = node;
        } else {
            list->head = node;
            list->tail = node;
        }
    }
}

void lAddBack(NODE *node, LIST *list) {
    if(node != NULL && list != NULL) {
        if(list->head != NULL) {
            node->prev = list->tail;
            list->tail->next = node;
            list->tail = node;
        } else {
            list->head = node;
            list->tail = node;
        }
    }
}

void lDeleteNode(NODE *node, LIST *list) {
    if(node != NULL && list != NULL) {
        if(lIsInList(node, list) == 1) {
            if(list->head == node) {
                list->head = node->next;
                node->next->prev = NULL;
            } else if(list->tail == node) {
                list->tail = node->prev;
                node->prev->next = NULL;
            } else {
                node->next->prev = node->prev;
                node->prev->next = node->next;
            }
        }
    }
    free(node);
}

void lDeleteList(LIST *list) {
    NODE *tmp, *point;
    if(list != NULL) {
        tmp = list->head;
        while(tmp != NULL) {
            point = tmp;
            tmp = tmp->next;
            free(point);
        }
        free(list);
    }
}

NODE *lFindByIndex( int index, LIST *list) {
    NODE *tmp;
    if(list != NULL) {
        tmp = list->head;
        for(int i = 1; i < index && tmp != NULL; i++) {
            tmp = tmp->next;
        }
    }
    return tmp;
}

NODE *lFindPerson( char *firstName, char *lastName, LIST *list) {
    NODE *tmp = NULL;
    if(list != NULL) {
        tmp = list->head;
        while(tmp != NULL && (strcmp(tmp->firstName, firstName) != 0)
              && (strcmp(tmp->lastName, lastName)) != 0) {
            tmp = tmp->next;
        }
    }
    return tmp;
}

int lIsInList(NODE *node, LIST *list) {
    if(list != NULL && node != NULL) {
        NODE *tmp = NULL;
        if(list->head != NULL) {
            tmp = list->head;
            while(tmp != NULL && tmp != node) {
                tmp = tmp->next;
            }
            return (tmp != NULL) ? 1 : 0;
        }
    }
    return 0;
}

//insert-sort
void lSort(LIST *list, char *key) {
    if(list != NULL) {
        NODE *tmpHead = list->head;
        NODE *tmpActual, *tmp;

        while(tmpHead != NULL) {
            tmpActual = tmpHead->next;
            while(tmpActual != NULL && lCompare2(tmpHead, tmpActual, key) <= 0) {
                tmpActual = tmpActual->next;
            }
            if(tmpActual != NULL) {
                tmp = tmpHead->next;
                lSwap(tmpHead, tmpActual, list);
                tmpHead = tmp;
            } else {
                tmpHead = tmpHead->next;
            }
        }
    }
}

void lSwap(NODE *node1, NODE *node2, LIST *list) {
    if((node1 != NULL && node2 != NULL) && (node1 != node2)) {
        NODE *tmp1, *tmp2;
        if(list->head == node1) {
            list->head = node2;
        } else if(list->head == node2) {
            list->head = node1;
        }

        if(list->tail == node1) {
            list->tail = node2;
        } else if(list->tail == node2) {
            list->tail = node1;
        }

        tmp1 = node1->next;
        tmp2 = node2->next;
        node1->next = tmp2;
        node2->next = tmp1;
        if(tmp1 != NULL) tmp1->prev = node2;
        if(tmp2 != NULL) tmp2->prev = node1;

        tmp1 = node1->prev;
        tmp2 = node2->prev;
        node1->prev = tmp2;
        node2->prev = tmp1;
        if(tmp1 != NULL) tmp1->next = node2;
        if(tmp2 != NULL) tmp2->next = node1;
    }
}

int lCompare(NODE *node1, NODE *node2) {
    if(node1 != NULL && node2 != NULL) {
        int cmpResult = strcmp(node1->firstName, node2->firstName);
        if(cmpResult < 0) {
            return -1;
        } else if(cmpResult > 0) {
            return 1;
        } else {
            return strcmp(node1->lastName, node2->lastName);
        }
    }
    return 0;
}

int lCompare2(NODE *node1, NODE *node2, char *field) {
    if(node1 != NULL && node2 != NULL) {
        if(strcmp(field,"firstName") == 0 ) {
            return strcmp(node1->firstName, node2->firstName);

        }

        else if(strcmp(field,"lastName") == 0 ) {
            return strcmp(node1->lastName, node2->lastName);
        }
        else if(strcmp(field,"email") == 0 ) {
            return strcmp(node1->email, node2->email);
        }
        else if(strcmp(field,"birthDate") == 0 ) {
            return strcmp(node1->birthDate, node2->birthDate);
        }
        else if(strcmp(field,"phone") == 0 ) {
            return strcmp(node1->phone, node2->phone);
        }
        else {
            fprintf(stderr, "Wrong argument!\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

void lPrint(LIST *list) {
    if(list != NULL) {
        NODE *tmp = list->head;
        while(tmp != NULL) {
            lPrintPerson(tmp);
            tmp = tmp->next;
        }
    }
}

void lPrintPerson(NODE *person) {
    if(person != NULL) {
        printf("Person: %s %s, %s, %s, %s\n",
               person->firstName, person->lastName, person->birthDate,
               person->email, person->phone);
    }
}
//////////////////////////////////////////////////////////////////////

TNODE *tcreateNode(char *firstName, char *lastName, char *birthDate,
                 char *email, char *phone) {
    TNODE *tmp = malloc(sizeof(TNODE));
    if(tmp == NULL) {
        fprintf(stderr, "Malloc failed!\n");
        exit(EXIT_FAILURE);
    }

    memset(tmp->firstName, '\0', sizeof(tmp->firstName)); //allocate memory
    strncpy(tmp->firstName, firstName, BUFFER_LEN); //save values

    memset(tmp->lastName, '\0', sizeof(tmp->lastName));
    strncpy(tmp->lastName, lastName, BUFFER_LEN);

    memset(tmp->birthDate, '\0', sizeof(tmp->birthDate));
    strncpy(tmp->birthDate, birthDate, BUFFER_LEN);

    memset(tmp->email, '\0', sizeof(tmp->email));
    strncpy(tmp->email, email, BUFFER_LEN);

    memset(tmp->phone, '\0', sizeof(tmp->phone));
    strncpy(tmp->phone, phone, BUFFER_LEN);

    tmp->parent = NULL;
    tmp->left = NULL;
    tmp->right = NULL;

    return tmp;
}


void setTreeKey(char *key, TREE *tree){
    memset(tree->key, '\0', sizeof(tree->key));
    strncpy(tree->key, key, BUFFER_LEN);

}


TREE *createTree() {
    TREE *tmp = malloc(sizeof(TREE));
    if(tmp == NULL) {
        fprintf(stderr, "Malloc failed!\n");
        exit(EXIT_FAILURE);
    }
    tmp->root = NULL;
    setTreeKey("firstName", tmp); //default key
    return tmp;
}

int tCompare(TNODE *node1, TNODE *node2) {
    if(node1 != NULL && node2 != NULL) {
        int cmpResult = strcmp(node1->firstName, node2->firstName);
        if(cmpResult < 0) {
            return -1;
        } else if(cmpResult > 0) {
            return 1;
        } else {
            return strcmp(node1->lastName, node2->lastName);
        }
    }
    return 0;
}

int tCompare2(TNODE *node1, TNODE *node2, char *field) {
    if(node1 != NULL && node2 != NULL) {
        if(strcmp(field,"firstName") == 0 ) {
            return strcmp(node1->firstName, node2->firstName);

        }

        else if(strcmp(field,"lastName") == 0 ) {
            return strcmp(node1->lastName, node2->lastName);
        }
        else if(strcmp(field,"email") == 0 ) {
            return strcmp(node1->email, node2->email);
        }
        else if(strcmp(field,"birthDate") == 0 ) {
            return strcmp(node1->birthDate, node2->birthDate);
        }
        else if(strcmp(field,"phone") == 0 ) {
            return strcmp(node1->phone, node2->phone);
        }
        else {
            fprintf(stderr, "Wrong argument!\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

void tPrintPerson(TNODE *person) {
    if(person != NULL) {
        printf("Person: %s %s, %s, %s, %s\n",
               person->firstName, person->lastName, person->birthDate,
               person->email, person->phone);
    }
}


void tPrintInOrder(TREE *tree){
    tPrintInOrderNode(tree->root);
}

void tPrintInOrderNode(TNODE *tnode){
    if(tnode != NULL){
        tPrintInOrderNode(tnode->left);
        tPrintPerson(tnode);
        tPrintInOrderNode(tnode->right);
    }
}

void tAdd(TNODE *tnode, TREE *tree){
    if(tnode != NULL && tree != NULL){
        tree->root = tInsert(tnode, tree->root);
    }
}

TNODE *tInsert(TNODE *tnode, TNODE *root){
    if(tnode != NULL && root != NULL){
        if(tCompare(tnode,root) < 0){
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

void tDeletePom(TNODE *tnode){
    if(tnode != NULL){
        tDeletePom(tnode->left);
        tDeletePom(tnode->right);
        //printf("Deleting node: ");
        //tPrintPerson(tnode);
        free(tnode);
    }
}

void tDeleteTree(TREE *tree){
    tDeletePom(tree->root);
}


TNODE *tFindPerson( char *firstName, char *lastName, TREE *tree){
    if(tree != NULL && tree->root != NULL){
        return  tFindPersonPom(firstName, lastName, tree->root);
    }
    else return NULL;
}

TNODE *tFindPersonPom(char *firstName, char *lastName, TNODE *tnode){
    if(tnode != NULL){
        if( strcmp(tnode->firstName,firstName) == 0 && strcmp(tnode->lastName,lastName) == 0){
            return tnode;
        }
        TNODE *leftResult = tFindPersonPom(firstName,lastName,tnode->left);
        TNODE *rightResult = tFindPersonPom(firstName, lastName, tnode->right);
        if(leftResult != NULL){
            return leftResult;
        }
        if(rightResult != NULL){
            return rightResult;
        }
    }
    return NULL;
}


LIST *treeToListPom(TNODE *tnode, LIST *list){
    if(tnode != NULL){
        treeToListPom(tnode->left, list);
        treeToListPom(tnode->right, list);
        lAddFront(createNode(tnode->firstName, tnode->lastName, tnode->birthDate, tnode->email, tnode->phone),list);
    }
}

LIST *treeToList(TREE *tree, LIST *list){
    treeToListPom(tree->root, list);
}