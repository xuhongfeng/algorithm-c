/*
 * string_builder.h
 *
 *  Created on: Apr 19, 2014
 *      Author: xuhongfeng
 */

#ifndef STRING_BUILDER_H_
#define STRING_BUILDER_H_

typedef struct StringBuilder {
    char* buf;
    int capacity;
    int size;
} StringBuilder;

StringBuilder* newStringBuilder();

void stringBuilderAppend(StringBuilder* sb, char* str);
void stringBuilderAppendChar(StringBuilder* sb, char c);
void stringBuilderAppendInt(StringBuilder* sb, int v);

void freeStringBuilder(StringBuilder* sb);
char* stringBuilderToString(StringBuilder* sb);


#endif /* STRING_BUILDER_H_ */
