/*
 * dict.c
 *
 *  Created on: May 28, 2014
 *      Author: xuhongfeng
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dict.h"

/********************  PRIVATE PROTOTYPE  ***********************/

static void _initDict(Dict *dict);
static void _resetHashTable(DictHashTable *ht);
static int _dict_hash(int key);
static void _check_capacity(Dict* dict);
static int _is_rehashing(Dict *dict);
static void _allocate_hash_table(DictHashTable *ht, int size);
static void _do_rehash(Dict* dict, int steps);
static int _key_index(DictHashTable *ht, int key);

/***************************  API  ******************************/

Dict *dictCreate() {
    Dict *dict = (Dict*) malloc(sizeof(Dict));
    _initDict(dict);
    return dict;
}

void freeDict(Dict *dict) {
    free(dict);
}

void dictPut(Dict *dict, int key, int value) {
    int idx;
    DictHashTable *ht;
    DictEntry *p;

    _check_capacity(dict);
    if (_is_rehashing(dict)) {
        _do_rehash(dict, 1);
    }

    ht = _is_rehashing(dict) ? &(dict->hashTable[1]) : &(dict->hashTable[0]);
    idx = _key_index(ht, key);
    p = ht->table[idx];
    while (p != NULL) {
        if (p->key == key) {
            p->value = value;
            return;
        }
        p = p->next;
    }
    p = (DictEntry*) malloc(sizeof(DictEntry));
    p->key = key;
    p->value = value;
    p->next = ht->table[idx];
    ht->table[idx] = p;
    ht->used++;
}

int dictSize(Dict *dict) {
    return dict->hashTable[0].used + dict->hashTable[1].used;
}

int dictGet(Dict *dict, int key) {
    int i, idx;
    DictHashTable *ht;
    DictEntry *p;

    if (_is_rehashing(dict)) {
        _do_rehash(dict, 1);
    }

    for (i=0; i<2; i++) {
        ht = &(dict->hashTable[i]);
        idx = _key_index(ht, key);
        p = ht->table[idx];
        while (p != NULL) {
            if (p->key == key) {
                return p->value;
            }
            p = p->next;
        }
        if (!_is_rehashing(dict)) {
            break;
        }
    }
    return -1;
}

void dictDelete(Dict *dict, int key) {
    int i, idx;
    int flagFound = 0;
    DictHashTable *ht;
    DictEntry *p, *q;

    if (_is_rehashing(dict)) {
        _do_rehash(dict, 1);
    }

    for (i=0; i<2; i++) {
        ht = &(dict->hashTable[i]);
        idx = _key_index(ht, key);
        p = ht->table[idx];
        while (p != NULL) {
            if (p->key == key) {
                if (ht->table[idx] == p) {
                    ht->table[idx] = p->next;
                } else {
                    q->next = p->next;
                }
                free(p);
                ht->used--;
                flagFound = 1;
                break;
            }
            q = p;
            p = p->next;
        }
        if (!_is_rehashing(dict) || flagFound) {
            break;
        }
    }
}

DictIterator *dictGetIterator(Dict *dict) {
    DictIterator *iterator = (DictIterator*) malloc(sizeof(DictIterator));
    iterator->dict = dict;
    iterator->table = 0;
    iterator->index = -1;
    iterator->current = NULL;
    return iterator;
}

void dictFreeIterator(DictIterator *iterator) {
    free(iterator);
}

DictEntry *dictIteratorNext(DictIterator *iterator) {
    DictHashTable *ht;
    while (1) {
        if (iterator->current) {
            iterator->current = iterator->current->next;
        } else {
            ht = &(iterator->dict->hashTable[iterator->table]);
            if (iterator->index+1 < ht->size) {
                iterator->current = ht->table[++iterator->index];
            } else if (iterator->table == 0 && _is_rehashing(iterator->dict)) {
                iterator->index = -1;
                iterator->table = 1;
            } else {
                break;
            }
        }

        if (iterator->current) {
            return iterator->current;
        }
    }

    return NULL;
}

/******************  PRIVATE IMPLEMENTATION **********************/

static void _initDict(Dict *dict) {
    _resetHashTable(&dict->hashTable[0]);
    _resetHashTable(&dict->hashTable[1]);
    dict->rehashIndex = -1;
}

static void _resetHashTable(DictHashTable *ht) {
    ht->table = NULL;
    ht->size = 0;
    ht->sizeMask = 0;
    ht->used = 0;
}

static int _dict_hash(int key) {
    return key;
}

static void _check_capacity(Dict *dict) {
    DictHashTable* ht0 = &(dict->hashTable[0]);
    DictHashTable* ht1 = &(dict->hashTable[1]);
    int newSize;

    if (_is_rehashing(dict)) {
        return;
    }

    if (ht0->size == 0) {
        _allocate_hash_table(ht0, 1);
        return;
    }

    if (ht0->used == ht0->size) {
        newSize = ht0->size << 1;
        _allocate_hash_table(ht1, newSize);
        dict->rehashIndex = 0;
    }
}

static void _allocate_hash_table(DictHashTable *ht, int size) {
    _resetHashTable(ht);
    ht->size = size;
    ht->sizeMask = size - 1;
    ht->table = (DictEntry**)calloc(size, sizeof(DictEntry*));
}

static int _is_rehashing(Dict *dict) {
    if (dict->rehashIndex == -1) {
        return 0;
    }
    return 1;
}

static void _do_rehash(Dict* dict, int steps) {
    DictEntry *de, *nextDe;
    int idx;

    if (!_is_rehashing(dict)) {
        return;
    }
    while (steps--) {
        if (dict->hashTable[0].used == 0) {
            free(dict->hashTable[0].table);
            dict->hashTable[0] = dict->hashTable[1];
            _resetHashTable(&dict->hashTable[1]);
            dict->rehashIndex = -1;
            return;
        }

        while (dict->hashTable[0].table[dict->rehashIndex] == NULL) {
            dict->rehashIndex++;
        }

        de = dict->hashTable[0].table[dict->rehashIndex];
        while (de) {
            nextDe = de->next;

            idx = _key_index(&dict->hashTable[1], de->key);
            de->next = dict->hashTable[1].table[idx];
            dict->hashTable[1].table[idx] = de;
            dict->hashTable[0].used--;
            dict->hashTable[1].used++;

            de = nextDe;
        }
        dict->hashTable[0].table[dict->rehashIndex] = NULL;
        dict->rehashIndex++;
    }
}

static int _key_index(DictHashTable *ht, int key) {
    int h = _dict_hash(key);
    return ht->sizeMask & h;
}

/***************************  TEST  ******************************/

int _testDict() {
    Dict *dict;
    DictHashTable *ht;
    int i;
    DictEntry *p, *q;
    DictIterator *iterator;

    //test create and free
    dict = dictCreate();
    freeDict(dict);
    dict = dictCreate();

    //test _allocate_hash_table
    ht = (DictHashTable*) malloc(sizeof(DictHashTable));
    _allocate_hash_table(ht, 10);
    assert(ht->size == 10);
    assert(ht->sizeMask == 9);
    for (i=0; i<10; i++) {
        assert(ht->table[i] == NULL);
    }

    //test _check_capacity
    freeDict(dict);
    dict = dictCreate();
    _check_capacity(dict);
    assert(dict->hashTable[0].size == 1);

    //test rehash
    freeDict(dict);
    dict = dictCreate();
    _allocate_hash_table(&dict->hashTable[0], 4);
    _allocate_hash_table(&dict->hashTable[1], 8);
    p = (DictEntry*) malloc(sizeof(DictEntry));
    p->key = 2;
    p->value = 2;
    q = (DictEntry*) malloc(sizeof(DictEntry));
    q->key = 6;
    q->value = 6;
    p->next = q;
    dict->hashTable[0].table[2] = p;
    dict->hashTable[0].used = 2;
    dict->rehashIndex = 0;
    _do_rehash(dict, 10);
    assert(dict->rehashIndex == -1);
    assert(dict->hashTable[1].table == NULL);
    assert(dict->hashTable[0].size == 8);
    assert(dict->hashTable[0].used == 2);
    assert(dict->hashTable[0].table[2]->key == 2);
    assert(dict->hashTable[0].table[6]->key == 6);

    //test put get delete
    freeDict(dict);
    dict = dictCreate();
    dictPut(dict, 1, 1);
    dictPut(dict, 2, 2);
    dictPut(dict, 3, 3);
    assert(dictSize(dict) == 3);
    assert(dictGet(dict, 1) == 1);
    assert(dictGet(dict, 2) == 2);
    assert(dictGet(dict, 3) == 3);
    assert(dictGet(dict, 4) == -1);
    dictDelete(dict, 1);
    assert(dictSize(dict) == 2);
    assert(dictGet(dict, 1) == -1);

    //test iterator
    freeDict(dict);
    dict = dictCreate();
    dictPut(dict, 1, 1);
    dictPut(dict, 2, 2);
    dictPut(dict, 3, 3);
    iterator = dictGetIterator(dict);
    assert(dictIteratorNext(iterator)->value == 2);
    assert(dictIteratorNext(iterator)->value == 3);
    assert(dictIteratorNext(iterator)->value == 1);
    assert(dictIteratorNext(iterator) == NULL);
    dictFreeIterator(iterator);

    printf("test pass\n");

    return 0;
}
