#ifndef parser_h
#define parser_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"
#include "prec.h"
#include "symtable.h"


/* struct for keeping one line of code */
typedef struct tLine {
    TString *token;
    int tokenID;
    struct tLine *next;
} *tLinePtr;


/* list containing all lines from stdin */
typedef struct tCode {
    struct tLine *lineData;
    struct tCode *next;
} *tCodePtr;

typedef struct {
    tCodePtr first;
    tCodePtr last;
} tCodeList;

int main();

void tCodeInit(tCodeList *);

int tCodeCreateNewLine(tCodeList *);
int tCodeInsertToken(tCodeList *, TString *, int);
void tCodeDispose(tCodeList *);

#include "syntax_check.h"


#endif
