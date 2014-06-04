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
        freeSkipNode(p);
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

SkipListIterator *slGetIterator(SkipList *list) {
    SkipListIterator *iterator = (SkipListIterator*) malloc(sizeof(SkipListIterator));
    iterator->list = list;
    iterator->current = list->head;
    return iterator;
}

void freeSkipListIterator(SkipListIterator *iterator) {
    free(iterator);
}

SkipNode* slIteratorNext(SkipListIterator *iterator) {
    if (iterator->list->size == 0 || iterator->current->levels[0] == NULL) {
        return NULL;
    }
    iterator->current = iterator->current->levels[0];
    return iterator->current;
}

void slDelete(SkipList *list, int key) {
    SkipNode *update[SKIP_LIST_MAX_LEVEL];
    int i;
    SkipNode *p, *found = NULL;

    p = list->head;
    for (i=list->level-1; i>=0; i--) {
        update[i] = NULL;
        while (p->levels[i] != NULL) {
            if (p->levels[i]->key == key) {
                update[i] = p;
                found = p->levels[i];
                break;
            } else if (p->levels[i]->key < key) {
                p = p->levels[i];
            } else {
                break;
            }
        }
    }
    if (found != NULL) {
        for (i=list->level-1; i>=0; i--) {
            if (update[i] != NULL) {
                update[i]->levels[i] = found->levels[i];
            }
        }
        free(found);
        list->size--;
    }
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
int testSkipList() {
    SkipList *list;
    SkipNode *p;
    SkipListIterator *iterator;
    int i;

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


    // test iterator
    iterator = slGetIterator(list);
    for (i=0; i<10; i++) {
        assert(slIteratorNext(iterator)->value == i+1);
    }
    assert(slIteratorNext(iterator) == NULL);
    freeSkipListIterator(iterator);

    // test delete
    slDelete(list, 5);
    assert(list->size == 9);
    assert(slGet(list, 5) == -1);
    slDelete(list, 5);
    assert(list->size == 9);

    freeSkipList(list);

    printf("test pass\n");
}
