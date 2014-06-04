/*
 * d_string.h
 *
 * Dynamic String
 *
 *  Created on: Jun 2, 2014
 *      Author: xuhongfeng
 */

#ifndef D_STRING_H_
#define D_STRING_H_

/************************ struct ***************************/

typedef char* DString;

typedef struct DStringHead {
    int len;
    int free;
    char buf[];
} DStringHead;


/************************ api ***************************/
DString newDString(char* str);
void freeDString(DString ds);

DString dsCopy(DString ds);
void dsClear(DString ds);
DString dsAppend(DString ds, char* str);


#endif /* D_STRING_H_ */
