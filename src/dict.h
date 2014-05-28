/*
 * dict.h
 *
 *  Created on: May 18, 2014
 *      Author: xuhongfeng
 */

#ifndef DICT_H_
#define DICT_H_

typedef struct DictType {
    unsigned int (*hashFunction) (const void *key);
    void* (*keyDup) (const void *key);
    void* (*valueDup) (const void *value);
    int (*keyCompare) (const void *k1, const void *k2);
    void (*freeKey) (void *key);
    void (*freeValue) (void *value);
} DictType;

typedef struct DictEntry {
    void* key;
    void* value;
    struct DictEntry* next;
} DictEntry;

typedef struct DictHashTable {
    DictEntry ** table;
    unsigned long size;
    unsigned long sizeMask;
    unsigned long used;
} DictHashTable;

typedef struct Dict {
    DictType* type;
    DictHashTable hashTable[2];
} Dict;


/***************************  API ******************************/

DictType *dictCreateType();

Dict *dictCreate(DictType *type);

void freeDict(Dict *dict);

#endif /* DICT_H_ */
