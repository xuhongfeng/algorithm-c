/*
 * skip_list.h
 *
 *  Created on: Jun 1, 2014
 *      Author: xuhongfeng
 */

#ifndef SKIP_LIST_H_
#define SKIP_LIST_H_

#define SKIP_LIST_MAX_LEVEL 10

typedef struct SkipNode {
    int key, value;
    struct SkipNode** levels;
} SkipNode;

typedef struct SkipList {
    SkipNode* head;
    int size;
    int level;
} SkipList;

/*******************  API  *************************/

SkipNode *createSkipNode();
void freeSkipNode(SkipNode *node);

SkipList *createSkipList();
void freeSkipList(SkipList *list);

#endif /* SKIP_LIST_H_ */
