#ifndef SYNTAX_CHECK_H
#define SYNTAX_CHECK_H

#define MAX_STACK 1000000

#include "parser.h"

#include <stdio.h>

int syntax_analysis(tCodePtr *);

typedef struct {
    int value[MAX_STACK];
    int top;
}tStack;

/* operations with stack */
void stackInit(tStack *);
int stackEmpty(tStack *);
void sPush(tStack *, int);
void sPop(tStack *);
int sTop(tStack *);


#endif //SYNTAX_CHECK_H
