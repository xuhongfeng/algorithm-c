/*
 * dict.h
 *
 *  Created on: May 18, 2014
 *      Author: xuhongfeng
 */

#ifndef DICT_H_
#define DICT_H_

/***************************  macro ******************************/
#define DICT_OK 0
#define DICT_ERR 1

/***************************  struct ******************************/

typedef struct DictEntry {
    int key;
    int value;
    struct DictEntry* next;
} DictEntry;

typedef struct DictHashTable {
    DictEntry ** table;
    int size;
    int sizeMask;
    int used;
} DictHashTable;

typedef struct Dict {
    DictHashTable hashTable[2];
    int rehashIndex;
} Dict;

typedef struct DictIterator {
    Dict *dict;
    int table, index;
    DictEntry *current;
} DictIterator;

/***************************  API ******************************/

Dict *dictCreate();

void dictPut(Dict *dict, int key, int value);
int dictGet(Dict *dict, int key);
void dictDelete(Dict *dict, int key);
int dictSize(Dict *dict);

DictIterator *dictGetIterator(Dict *dict);
void dictFreeIterator(DictIterator *iterator);
DictEntry *dictIteratorNext(DictIterator *iterator);

void freeDict(Dict *dict);

#endif /* DICT_H_ */
