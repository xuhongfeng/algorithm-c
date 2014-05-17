/*
 * linked_list.c
 *
 *  Created on: May 13, 2014
 *      Author: xuhongfeng
 */

#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "string_builder.h"
#include "utils.h"

/* -------------------------- private prototypes ---------------------------- */

ListNode* _newListNode(void *value);

/* initialize the given list with one node */
void _initList(List *list, ListNode *node);



/* ----------------------------- API implementation ------------------------- */

List *newList() {
    List *list = (List*)malloc(sizeof(List));
    list->head = list->tail = NULL;
    list->len = 0;
    list->dup = NULL;
    list->free = NULL;
    list->match = NULL;
    return list;
}

void freeList(List *list) {
    ListIterator *iter = listGetIterator(list, DIRECT_BACKWARD);
    while (listIterHasNext(iter)) {
        ListNode* node = iter->next;
        listIteratorNext(iter);
        freeListNode(list, node);
    }
    free(list);
}

void freeListNode(List *list, ListNode *node) {
    list->free(node->value);
    free(node);
}

void listPush(List *list, void *value) {
    ListNode *node = _newListNode(value);
    if (listEmpty(list)) {
        _initList(list, node);
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
        list->len++;
    }
}

void* listPop(List *list) {
    if (listEmpty(list)) {
        return NULL;
    }
    ListNode *node = list->tail;
    list->tail = list->tail->prev;
    list->len--;
    if (!listEmpty(list)) {
        list->tail->next = NULL;
    }

    void *value = node->value;
    free(node);
    return value;
}

void listShift(List *list, void *value) {
    ListNode *node = _newListNode(value);
    if (listEmpty(list)) {
        _initList(list, node);
    } else {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
        list->len++;
    }
}

void* listUnshift(List *list) {
    if (listEmpty(list)) {
        return NULL;
    }
    ListNode *node = list->head;
    list->head = list->head->next;
    list->len--;
    if (!listEmpty(list)) {
        list->head->prev = NULL;
    }

    void *value = node->value;
    free(node);
    return value;
}

ListIterator *listGetIterator(List *list, int direction) {
    ListIterator *iter = (ListIterator*)malloc(sizeof(ListIterator));
    iter->next = list->head;
    iter->direction = direction;
    return iter;
}

void* listIteratorNext(ListIterator *iter) {
    void *value = iter->next->value;
    if (iter->direction == DIRECT_FORWARD) {
        iter->next = iter->next->next;
    } else {
        iter->next = iter->next->prev;
    }
    return value;
}

/* -------------------------- private implementation ---------------------------- */
ListNode* _newListNode(void *value) {
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->prev = node->next = NULL;
    node->value = value;
    return node;
}

void _initList(List *list, ListNode *node) {
    list->head = list->tail = node;
    list->len = 1;
}

/* -------------------------- test ---------------------------- */

typedef struct People {
    char *name;
    int id;
} People;

People* peopleDup(People *p) {
    People *q = (People*)malloc(sizeof(People));
    q->id = p->id;
    StringBuilder *sb = newStringBuilder();
    stringBuilderAppend(sb, p->name);
    q->name = stringBuilderToString(sb);
    freeStringBuilder(sb);
    return q;
}

void freePeople(People *p) {
    free(p->name);
    free(p);
}

int peopleMatch(People *p1, People *p2) {
    if (p1 == NULL || p2 == NULL) {
        return 0;
    }
    return p1->id == p2->id;
}

int main() {
    List* list = newList();
    list->dup = peopleDup;
    list->match = peopleMatch;
    list->free = freePeople;

    People *p = (People*) malloc(sizeof(People));
    p->id = 1;
    p->name = mallocString("bob");

    listPush(list, p);

    p = (People*) malloc(sizeof(People));
    p->id = 2;
    p->name = mallocString("jack");
    listPush(list, p);

    p = (People*) listUnshift(list);
    printf("unshift : %s\n", p->name);
    listShift(list, p);
    p = (People*) listPop(list);
    printf("pop: %s\n", p->name);
    listPush(list, p);

    ListIterator *iter = listGetIterator(list, DIRECT_FORWARD);
    while (listIterHasNext(iter)) {
        p = (People*) listIteratorNext(iter);
        printf("%s\n", p->name);
    }

    freeList(list);

    printf("end\n");
    return 0;
}
