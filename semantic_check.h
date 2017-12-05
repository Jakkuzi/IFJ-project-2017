#ifndef SEMANTIC_CHECK_H
#define SEMANTIC_CHECK_H

#include <stdio.h>

#include "strings.h"
#include "parser.h"
#include "symtable.h"
#include "generator.h"

int semantic_check(tCodeList *, BTNodePtr);

int addBuiltInFunctions(BTNodePtr);
void multiFree(char *, char *, char *, char *);
int check_func_for_definition(BTNodePtr);

varDataType getType(int);

#endif