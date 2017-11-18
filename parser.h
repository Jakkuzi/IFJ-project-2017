#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include "syntax_check.h"
#include "strings.h"

/* default function called from main */
int main();

typedef struct tLine tLine; // deklarace pred pouzitim v tCode

/* list containing all lines from stdin */
typedef struct tCode {
    struct tLine *lineData;
    struct tCode *next;
} *tCodePtr;

/* struct for keeping one line of code */
typedef struct tLine {
    TString *token;
    struct tLine *next;
} *tLinePtr;

typedef struct {
    tCodePtr first;
    tCodePtr last;
} tCodeList;

void tCodeInit(tCodeList *);
int tCodeCreateNewLine(tCodeList *);
void tCodeInsertToken(tCodeList *, TString);



#endif PARSER_H
