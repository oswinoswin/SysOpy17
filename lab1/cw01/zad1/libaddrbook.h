
#ifndef _LIBADDRBOOK
#define _LIBADDRBOOK

#define BUFFER_LEN	256

typedef struct NODE {
    struct NODE *prev;
    struct NODE *next;
    char firstName[BUFFER_LEN];
    char lastName[BUFFER_LEN];
    char birthDate[BUFFER_LEN];
    char email[BUFFER_LEN];
    char phone[BUFFER_LEN];
} NODE;

typedef struct LIST {

    struct NODE *head;
    struct NODE *tail;
} LIST;

typedef struct TNODE {

    struct TNODE *parent;
    struct TNODE *left;
    struct TNODE *right;
    char firstName[BUFFER_LEN];
    char lastName[BUFFER_LEN];
    char birthDate[BUFFER_LEN];
    char email[BUFFER_LEN];
    char phone[BUFFER_LEN];
} TNODE;

typedef struct TREE {
    char key[BUFFER_LEN];
    struct TNODE *root;
} TREE;

LIST *createList();

NODE *createNode(char *firstName, char *lastName, char *birthDate,
                 char *email, char *phone);

void lAddFront(NODE *node, LIST *list);

void lAddBack(NODE *node, LIST *list);

void lDeleteNode(NODE *node, LIST *list);

void lDeleteList(LIST *list);

NODE *lFindByIndex(int index, LIST *list);

NODE *lFindPerson( char *firstName, char *lastName, LIST *list);

void lPrint(LIST *list);

void lPrintPerson(NODE *person);

int lIsInList(NODE *node, LIST *list);

void lSort(LIST *list, char *key);

int lCompare(NODE *node1, NODE *node2);

int lCompare2(NODE *node1, NODE *node2, char *field);

void lSwap(NODE *node1, NODE *node2, LIST *list);

TNODE *tcreateNode(char *firstName, char *lastName, char *birthDate,
                   char *email, char *phone);
TREE *createTree();

int tCompare(TNODE *tnode1, TNODE *tnode2);

int tCompare2(TNODE *tnode1, TNODE *tnode2, char *field);

void tPrintPerson(TNODE *person);

void tAdd(TNODE *tnode, TREE *tree);

TNODE *tInsert(TNODE *tnode, TNODE *root);

void tPrintInOrder(TREE *tree);

void tPrintInOrderNode(TNODE *root);

void tDeletePom(TNODE *tnode);

void tDeleteTree(TREE *tree);

TNODE *tFindPerson( char *firstName, char *lastName, TREE *tree);

TNODE *tFindPersonPom(char *firstName, char *lastName, TNODE *tnode);

void setTreeKey(char *key, TREE *tree);

LIST *treeToList(TREE *tree, LIST *list);

LIST *treeToListPom(TNODE *tnode, LIST *list);

#endif
