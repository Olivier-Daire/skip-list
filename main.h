typedef struct{
    int value;   
    struct Node *nextNode[1];
} Node;

typedef struct{
    Node *header;
    int level;
} SkipList;

SkipList *initList();
