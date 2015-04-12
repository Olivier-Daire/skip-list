#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#define MAXLEVEL 3
#define NIL NULL


int main(int argc, char const *argv[])
{	
	SkipList *list;
	list = initList();

	return 0;
}

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