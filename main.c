#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#define MAX_LEVEL 3
#define NIL NULL
#define MAX_INT 32767

int main(int argc, char const *argv[])
{	
	SkipList *list;
	list = initList();

	printf("---------------------Insert---------------------\n");
	insertNode(list, 1, 10);
	insertNode(list, 2, 2);
	printSkipList(list);

	printf("---------------------Delete key 1---------------------\n");
	deleteNode(list, 1);
	printSkipList(list);

	printf("---------------------Delete key 2---------------------\n");
	deleteNode(list, 2);
	printSkipList(list);

	free(list->header);
	free(list);
	return 0;
}

/* Initialize list */
SkipList *initList() {
	int i;
	SkipList *list = malloc(sizeof(SkipList));

    if ((list->header = malloc(sizeof(Node) + MAX_LEVEL*sizeof(Node *))) == 0) {
        printf ("Error during memory allocation\n");
        exit (EXIT_FAILURE);
    }

    list->header->key = MAX_INT;
    for (i = 0; i <= MAX_LEVEL; i++){
        list->header->nextNode[i] = NIL;
    }
    list->level = 0;

    return list;
}

/* Insert a new node */
Node *insertNode(SkipList *list, int key, int value){
	int i;
	Node *temp;
	// Array containing pointers to elements before the new one
	Node *update[MAX_LEVEL-1];
	temp = list->header;

	/* Find where to put the data equivalent to search */
    for (i = list->level; i >= 0; i--) {
        while (temp->nextNode[i] != NIL && temp->nextNode[i]->key < key){
            temp = temp->nextNode[i];
        }
        update[i] = temp;
    }

    temp = temp->nextNode[0];

    if (temp != NIL && temp->key == key){
    	temp->value = value;
    	return temp;
    } else {
    	int newLevel = randomLevel();

    	if (newLevel > list->level)
    	{
    		for (i = list->level + 1; i <= newLevel; i++){
    			update[i] = list->header;
    		}
    		list->level = newLevel;
    	}

    	temp = malloc(sizeof(Node) + sizeof(Node*) * (newLevel));
    	temp->key = key;
    	temp->value = value;

    	// Update links
    	for (i = 0; i <= newLevel; i++)
    	{
    		temp->nextNode[i] = update[i]->nextNode[i];
    		update[i]->nextNode[i] = temp;
    	}
    }

    return temp;
}

Node *deleteNode(SkipList *list, int key){
	int i;
	Node *temp = list->header;
	Node *update[MAX_LEVEL+1];

	for (i = list->level; i >= 0; i--)
	{
		while(temp->nextNode[i]->key < key){
			temp = temp->nextNode[i];
		}
		update[i] = temp;
	}

	temp = temp->nextNode[0];

	if (temp->key == key)
	{
		for (i = 0; i <= list->level; i++)
		{
			if (update[i]->nextNode[i] != temp)
			{
				break;
			}
			update[i]->nextNode[i] = temp->nextNode[i];
		}

		freeNode(temp);

		while(list->level > 0 && list->header->nextNode[list->level] == NIL){
			list->level--;
		}
	}

	return temp;
}

void printSkipList(SkipList *list){
	Node *temp;
	temp = list->header;

	while (temp && temp->nextNode[0] != list->header){
		printf("%d[%d]->", temp->key, temp->value);
		temp = temp->nextNode[0];
    }

    printf("NIL\n");
}

Node *search(SkipList *list, int key){
	int i;
	Node *temp = list->header;

	for (i = list->level; i >= 0; i--)
	{
		while(temp->nextNode[i]->key < key){
			temp = temp->nextNode[i];
		}
	}

	temp = temp->nextNode[0];
	
	if (temp->key == key)
	{
		return temp;
	} else {
		return NIL;
	}
}

void freeNode(Node *node){
	if (node)
	{
		free(node);
	}
}

int randomLevel(){
	int level = 0;
	while (rand() < RAND_MAX/2 && level < MAX_LEVEL) {
        level++;
	}
    return level;
}