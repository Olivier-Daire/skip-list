typedef struct Node{
    int key;
    int value;
    struct Node **nextNode;
} Node;

typedef struct{
    Node *header;
    int level;
} SkipList;

void initList(SkipList *list);
void initializeFromFile(SkipList *list, char const * path);
Node *insertNode(SkipList *list, int key, int value);
Node *deleteNode(SkipList *list, int key);
void printSkipList(SkipList *list);
Node *search(SkipList *list, int key);
void freeNode(Node *node);
int randomLevel(int p, int maxLevel);