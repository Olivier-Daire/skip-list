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
	SkipList list;
	initList(&list);

	// If executed with arguments, insert nodes from text file
	if (argc > 1)
	{
		initializeFromFile(&list, argv[1]);
		printf("---------------------Insert from file ---------------------\n");
		printSkipList(&list);
		
	} else {

		printf("---------------------Insert---------------------\n");
		insertNode(&list, 1, 10);
		insertNode(&list, 2, 2);
		printSkipList(&list);

		printf("---------------------Search for key 1---------------------\n");
		Node *toto = search(&list, 1);
		printf("Key value : %d\n", toto->value);

		printf("---------------------Delete key 1---------------------\n");
		deleteNode(&list, 1);
		printSkipList(&list);

		printf("---------------------Delete key 2---------------------\n");
		deleteNode(&list, 2);
		printSkipList(&list);
	}

	free(list.header->nextNode);
	free(list.header);	
	return 0;
}

/* Initialize list */
void initList(SkipList *list){
	int i;
	list->header = calloc(1, sizeof(Node));
	list->header->key = MAX_INT;

    if ((list->header->nextNode = calloc(2, sizeof(Node*) * MAX_LEVEL)) == 0) {
        printf ("Error during memory allocation\n");
        exit (EXIT_FAILURE);
    }

    for (i = 0; i <= MAX_LEVEL; i++){
        list->header->nextNode[i] = NIL;
    }
    list->level = 0;

}

void initializeFromFile(SkipList *list, char const * path){
	int i = 0;
	int value;
	FILE *file = fopen(path, "r");
	char buffer[MAX_CHAR_FILE];

	if (file == NULL)
	{
		printf("Error opening the file !\n");
		exit (EXIT_FAILURE);
	} else {
		while(fgets(buffer, MAX_CHAR_FILE, file) != NULL){
			value = atoi(buffer);
			insertNode(list, value, value);
			i++;
		}
		fclose(file);
	}
}

/* Insert a new node */
Node *insertNode(SkipList *list, int key, int value){
	int i;
	Node *temp;
	// Array containing pointers to elements before the new one
	Node *update[MAX_LEVEL-1];
	temp = list->header;

	/* Find where to put the data, equivalent to search */
    for (i = list->level ; i >= 0; i--) {
        while (temp->nextNode[i] != NIL &&  temp->nextNode[i]->key < key){
            temp = temp->nextNode[i];
        }
        update[i] = temp;
    }

	temp = temp->nextNode[0];

	// If duplicate key, replace it
    if (temp != NIL && temp->key == key){
    	
    	temp->value = value;
    	
    	return temp;

    } else {

    	int newLevel = randomLevel(1/4, MAX_LEVEL);

    	if (newLevel > list->level)
    	{
    		for (i = list->level + 1; i <= newLevel; i++){
    			update[i] = list->header;
    		}
    		list->level = newLevel;
    	}

    	temp = calloc(1, sizeof(Node));
    	temp->key = key;
    	temp->value = value;
    	temp->nextNode = calloc(1, sizeof(Node*) * (newLevel+1));

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
	
	for (i = list->level; i > 0; i--)
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
		free(node->nextNode);
		free(node);
	}
}

int randomLevel(int p, int maxLevel){
	int level = 0;
	while (rand() < p && level < maxLevel) {
        level++;
	}
    return level;
}