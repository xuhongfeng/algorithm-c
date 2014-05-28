/*
 * dict.c
 *
 *  Created on: May 28, 2014
 *      Author: xuhongfeng
 */

#include <stdio.h>
#include <stdlib.h>

#include "dict.h"


/***************************  API ******************************/

Dict *dictCreate(DictType *type) {
    Dict *dict = (Dict*) malloc(sizeof(Dict));
    dict->type = type;
    return dict;
}

DictType *dictCreateType() {
    DictType *type = (DictType*) malloc(sizeof(DictType));
    return type;
}

void freeDict(Dict *dict) {
    free(dict->type);
    free(dict);
}

/***************************  TEST  ******************************/

int main() {

    //test create and free
    DictType *type = dictCreateType();
    Dict *dict = dictCreate(type);
    freeDict(dict);

    printf("test pass\n");

    return 0;
}
