/*
 * string_builder.c
 *
 *  Created on: Apr 19, 2014
 *      Author: xuhongfeng
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "string_builder.h"

#define INIT_CAPACITY 8;

void checkCapacity(StringBuilder* sb, int delta);

StringBuilder* newStringBuilder() {
    StringBuilder* sb = (StringBuilder*) malloc(sizeof(StringBuilder));
    sb->capacity = INIT_CAPACITY;
    sb->size = 0;
    sb->buf = (char*) malloc(sb->capacity);
    return sb;
}

void stringBuilderAppendChar(StringBuilder* sb, char c) {
    checkCapacity(sb, 1);
    sb->buf[sb->size] = c;
    sb->size++;
}

void stringBuilderAppend(StringBuilder* sb, char* str) {
    int len = strlen(str);
    checkCapacity(sb, len);
    for (int i=0; i<len; i++) {
        sb->buf[sb->size++] = str[i];
    }
}

void stringBuilderAppendInt(StringBuilder* sb, int v) {
    int len = sizeof(int);
    checkCapacity(sb, len + 1);
    sprintf(sb->buf + sb->size, "%d", v);
    sb->size += len;
}

void freeStringBuilder(StringBuilder* sb) {
    free(sb->buf);
    free(sb);
}

char* stringBuilderToString(StringBuilder* sb) {
    char* str = (char*) malloc(sb->size+1);
    for (int i=0; i<sb->size; i++) {
        str[i] = sb->buf[i];
    }
    str[sb->size] = '\0';
    return str;
}

void checkCapacity(StringBuilder* sb, int delta) {
    int capacity = sb->size + delta;
    if (sb->capacity >= capacity) {
        return;
    }

    int newCapacity = sb->capacity;
    while (newCapacity < capacity) {
        newCapacity = newCapacity << 1;
    }

    char* oldBuf = sb->buf;
    char* newBuf = (char*) malloc(newCapacity);
    for (int i=0; i<sb->size; i++) {
        newBuf[i] = oldBuf[i];
    }

    sb->buf = newBuf;
    sb->capacity = newCapacity;

    free(oldBuf);
}

int test() {
    StringBuilder* sb = newStringBuilder();
    stringBuilderAppend(sb, "hello world");
    stringBuilderAppendChar(sb, '!');
    stringBuilderAppendInt(sb, 8);
    char* str = stringBuilderToString(sb);
    freeStringBuilder(sb);
    printf("%s\n", str);
    free(str);

    return 0;
}

int main() {
    return test();
}
