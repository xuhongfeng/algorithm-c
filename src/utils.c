/*
 * utils.c
 *
 *  Created on: May 14, 2014
 *      Author: xuhongfeng
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char* mallocString(char* str) {
    int len = strlen(str) + 1;
    char* dest = (char*)malloc(len);
    memcpy(dest, str, len);
    return dest;
}
