/*
 * linked_list.h
 *
 *  Created on: May 9, 2014
 *      Author: xuhongfeng
 */

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

typedef struct ListNode {
    struct ListNode *prev;
    struct ListNode *next;
    void *value;
} ListNode;

typedef struct List {
    ListNode *head;
    ListNode *tail;

    void *(*dup)(void *value);
    void (*free)(void *value);
    int (*match)(void *v1, void *v2);

    unsigned long len;
} List;

//direction definition for ListIterator
#define DIRECT_FORWARD 0
#define DIRECT_BACKWARD 1

typedef struct ListIterator {
    ListNode *next;
    int direction;
} ListIterator;

/* Functions implemented as macros */
#define listLen(list) ((list)->len)
#define listEmpty(list) (listLen(list) == 0 ? 1 : 0)

#define listIterHasNext(iter) (iter)->next == NULL ? 0 : 1

/* Prototypes */
List *newList();
void freeList(List *list);
void freeListNode(List* list, ListNode *node);

void listPush(List *list, void *value);
void* listPop(List *list);
void listShift(List *list, void *value);
void* listUnshift(List *list);

ListIterator *listGetIterator(List *list, int direction);
void* listIteratorNext(ListIterator *iter);

#endif /* LINKED_LIST_H_ */
