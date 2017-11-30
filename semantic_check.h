#ifndef SEMANTIC_CHECK_H
#define SEMANTIC_CHECK_H

#include "parser.h"
#include "symtable.h"
#include <stdio.h>

int semantic_check(tCodeList *, BTNodePtr);

int addBuiltInFunctions(BTNodePtr);
void multiFree(char *, char *, char *, char *);

#endif