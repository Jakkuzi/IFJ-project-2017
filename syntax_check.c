#include "syntax_check.h"


int syntax_analysis(tCodePtr *sCode){
    tStack *s;
    stackInit(s);


}

void stackInit(tStack *s){
    s->top = -1;
}

int stackEmpty(tStack *s){
    return (s->top == -1) ? 1 : 0;
}

void sPush(tStack *s, int num){
    s->value[++s->top] = num;
}

void sPop(tStack *s){
    if(!stackEmpty(s))
        s->top--;
}

int sTop(tStack *s){
    return s->value[s->top];
}