/*
 * d_string.c
 *
 *  Created on: Jun 2, 2014
 *      Author: xuhongfeng
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "d_string.h"

#define DS_MAX_EXPAND_STEP 1024000

/************************ private prototypes ***************************/
static DStringHead *_get_ds_head(DString ds);
static void _ds_update_len(DString ds);
static DString _ds_check_capacity(DString ds, int delta);

/************************ api ***************************/
DString newDString(char* str) {
    int len = strlen(str);
    DStringHead *head = (DStringHead*) malloc(sizeof(DStringHead) + len + 1);
    head->len = len;
    head->free = 0;
    memcpy(head->buf, str, len+1);
    return head->buf;
}

void freeDString(DString ds) {
    free(_get_ds_head(ds));
}

DString dsCopy(DString ds) {
    return newDString(ds);
}

void dsClear(DString ds) {
    DStringHead *head = _get_ds_head(ds);
    head->free += head->len;
    head->len = 0;
    head->buf[0] = '\0';
}

DString dsAppend(DString ds, char* str) {
    ds = _ds_check_capacity(ds, strlen(str));
    strcat(ds, str);
    return ds;
}

/***************************  private *********************************/
static DStringHead *_get_ds_head(DString ds) {
    return (DStringHead*) (ds - sizeof(DStringHead));
}

static void _ds_update_len(DString ds) {
    DStringHead *head = _get_ds_head(ds);
    int realLen = strlen(ds);
    head->free += (head->len - realLen);
    head->len = realLen;
}

static DString _ds_check_capacity(DString ds, int delta) {
    DStringHead *head = _get_ds_head(ds);
    int minCapacity = head->len + delta;
    int newCapacity = head->len + head->free;

    if (head->free >= delta) {
        return ds;
    }

    while (newCapacity < minCapacity) {
        if (newCapacity < DS_MAX_EXPAND_STEP) {
            newCapacity *= 2;
        } else {
            newCapacity += DS_MAX_EXPAND_STEP;
        }
    }

    head = (DStringHead*) realloc(head, newCapacity + 1 + sizeof(DStringHead));
    head->free = newCapacity - head->len;

    return head->buf;
}

/***************************  test  *********************************/
int main() {
    DString ds;
    DStringHead *head;

    //test create
    ds = newDString("hello");
    assert(strcmp(ds, "hello") == 0);
    freeDString(ds);

    //test copy
    ds = newDString("hello");
    ds = dsCopy(ds);
    assert(strcmp(ds, "hello") == 0);

    //test clear
    dsClear(ds);
    head = _get_ds_head(ds);
    assert(head->free == 5);

    //test check capacity
    freeDString(ds);
    ds = newDString("hello");
    ds = _ds_check_capacity(ds, 6);
    head = _get_ds_head(ds);
    assert(head->len == 5);
    assert(head->free == 15);
    assert(strcmp(ds, "hello") == 0);

    //test append
    freeDString(ds);
    ds = newDString("hello");
    ds = dsAppend(ds, " world!");
    assert(strcmp(ds, "hello world!") == 0);

    printf("test pass\n");
}
