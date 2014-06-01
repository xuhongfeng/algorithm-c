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


/**************  PRIVATE PROTOTYPES ********************/
static int _random_level();


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


void slInsert(SkipList *list, int key, int value) {
    SkipNode* update[SKIP_LIST_MAX_LEVEL];
    SkipNode *p;
    int i, level;

    p = list->head;
    for (i=list->level-1; i>=0; i--) {
        while (p->levels[i] != NULL) {
            if (p->levels[i]->key < key) {
                p = p->levels[i];
            } else if (p->levels[i]->key == key) {
                return;
            } else {
                break;
            }
        }
        update[i] = p;
    }

    level = _random_level();
    if (level > list->level) {
        for (i=list->level; i<level; i++) {
            update[i] = list->head;
        }
        list->level = level;
    }

    p = createSkipNode();
    p->key = key;
    p->value = value;

    for (i=level-1; i>=0; i--) {
        p->levels[i] = update[i]->levels[i];
        update[i]->levels[i] = p;
    }

    list->size++;
}

int slGet(SkipList *list, int key) {
    int i;
    SkipNode *p;

    p = list->head;
    for (i=list->level-1; i>=0; i--) {
        while (p->levels[i] != NULL) {
            if (p->levels[i]->key == key) {
                return p->levels[i]->value;
            } else if (p->levels[i]->key < key) {
                p = p->levels[i];
            } else {
                break;
            }
        }
    }

    return -1;
}

/********************  PRIVATE ************************/
static int _random_level() {
    int level=1;
    while ((random()&0xFFFF) < (0.25*0xFFFF)) {
        level++;
    }
    return level < SKIP_LIST_MAX_LEVEL ? level : SKIP_LIST_MAX_LEVEL;
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

    //test insert and get
    list = createSkipList();
    slInsert(list, 1, 1);
    slInsert(list, 2, 2);
    slInsert(list, 3, 3);
    slInsert(list, 4, 4);
    slInsert(list, 5, 5);
    slInsert(list, 6, 6);
    slInsert(list, 7, 7);
    slInsert(list, 8, 8);
    slInsert(list, 9, 9);
    slInsert(list, 10, 10);
    assert(slGet(list, 10) == 10);
    assert(slGet(list, 11) == -1);

    printf("test pass\n");
}
