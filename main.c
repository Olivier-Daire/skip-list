/**
* Execute via
* ./skip-list
*      or
* ./skip-list <path to a text file>
*/

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#define MAX_LEVEL 3
#define NIL NULL
#define MAX_INT 32767
#define MAX_CHAR_FILE 1000

int main(int argc, char const *argv[])
{	

	SkipList *list;
	list = initList();

	// If executed with arguments, insert nodes from text file
	if (argc > 1)
	{
		int i = 0;
		int value;
		FILE *file = fopen(argv[1], "r");
		char buffer[MAX_CHAR_FILE];

		while(fgets(buffer, MAX_CHAR_FILE, file) != NULL){
			value = atoi(buffer);
			insertNode(list, i, value);
			i++;
		}
		fclose(file);

		printf("---------------------Insert from file ---------------------\n");
		printSkipList(list);

	} else {

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
	}

	free(list->header);
	free(list);
	return 0;
}

/* Initialize list */
SkipList *initList() {
	int i;
	SkipList *list = calloc(1, sizeof(SkipList));

    if ((list->header = calloc(1, sizeof(Node) + MAX_LEVEL*sizeof(Node *))) == 0) {
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

// SkipList *initializeFromFile(){

// }

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

    	temp = calloc(1, sizeof(Node) + sizeof(Node*) * (newLevel));
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