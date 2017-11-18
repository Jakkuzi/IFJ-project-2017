#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "syntax_check.h"
#include "strings.h"

/* default function called from main */
int parser();

typedef struct tLine tLine; // deklarace pred pouzitim v tCode

/* list containing all lines from stdin */
typedef struct tCode {
    struct tLine *line;
    struct tCode *next;
} *tCodePtr;

/* struct for keeping one line of code */
typedef struct tLine {
    struct TString *line;
    struct tLine *next;
} *tLinePtr;

void tCodeInit(tCodePtr *);
void tCodeInsert(tCodePtr *);
void tCodeDispose(tCodePtr *);

void tLineInit(tLinePtr *);
void tLineInsert(tLinePtr *);
void tLineDispose(tLinePtr *);


#endif PARSER_H
