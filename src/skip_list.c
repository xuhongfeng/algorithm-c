/*
 * skip_list.c
 *
 *  Created on: Jun 1, 2014
 *      Author: xuhongfeng
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "skip_list.h"


/*******************  API  *************************/

SkipNode *createSkipNode() {
    SkipNode *node = (SkipNode*) malloc(sizeof(SkipNode));
    node->key = 0;
    node->value = 0;
    node->levels = (SkipNode**) calloc(SKIP_LIST_MAX_LEVEL, sizeof(SkipNode*));
    return node;
}

void freeSkipNode(SkipNode *node) {
    free(node->levels);
    free(node);
}

SkipList *createSkipList() {
    SkipList *list = (SkipList*) malloc(sizeof(SkipList));
    list->size = 0;
    list->level = 0;
    list->head = createSkipNode();
    return list;
}

void freeSkipList(SkipList *list) {
    SkipNode *p = list->head, *q;

    while (p) {
        q = p->levels[0];
        free(p);
        p = q;
    }

    free(list);
}




/**********************  test  **************************/
int main() {
    SkipList *list;
    SkipNode *p;

    // test create and free
    p = createSkipNode();
    freeSkipNode(p);
    list = createSkipList();
    freeSkipList(list);

    printf("test pass\n");
}
