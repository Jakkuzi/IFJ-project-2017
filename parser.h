#ifndef parser_h
#define parser_h

#include <stdio.h>
#include <stdlib.h>

#include "strings.h"

/* struct for keeping one line of code */
typedef struct tLine {
    TString *token;
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
void tCodeInsertToken(tCodeList *, TString);

#include "syntax_check.h"


#endif
