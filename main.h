typedef struct Node{
    int key;
    int value;
    struct Node *nextNode[1];
} Node;

typedef struct{
    Node *header;
    int level;
} SkipList;

SkipList *initList();
Node *insertNode(SkipList *list, int key, int value);
int randomLevel();
Node *search(SkipList *list, int key);
Node *deleteNode(SkipList *list, int key);
void freeNode(Node *node);
void printSkipList(SkipList *list);