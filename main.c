#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#define MAXLEVEL 3
#define NIL NULL


int main(int argc, char const *argv[])
{	
	SkipList *list;
	list = initList();

	insertNode(list, 1, 1);
	insertNode(list, 2, 2);

	Node *node = search(list, 2);
	printf("%d\n", node->value);
	return 0;
}

/* Initialize list */
SkipList *initList() {
	int i;
	SkipList *list = malloc(sizeof(SkipList));

    if ((list->header = malloc(sizeof(Node) + MAXLEVEL*sizeof(Node *))) == 0) {
        printf ("Error during memory allocation\n");
        exit (EXIT_FAILURE);
    }

    for (i = 0; i <= MAXLEVEL; i++){
        list->header->nextNode[i] = NIL;
    }

    list->level = 0;

    return list;
}

/* Insert a new node */
Node *insertNode(SkipList *list, int key, int value){
	int i;
	Node *temp;
	Node *update[MAXLEVEL+1];
	temp = list->header;

	/* Find where to put the data */
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

int randomLevel(){
	int level = 0;
	while (rand() < RAND_MAX/2 && level < MAXLEVEL) {
        level++;
	}
    return level;
}
