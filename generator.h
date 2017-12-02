#ifndef generator_h
#define generator_h

#include "scanner.h"
#include "parser.h"
#include "strings.h"
#include "symtable.h"

// dodatecne operace pro jazyk IFJcode17 (implicitni konverze datovych typu)
#define Int2Float 200
#define Float2Int 201
#define Float2Int2Float 202 // pro IntDiv je potreba nejdrive oseknout, pote prevest na float pro deleni a zpet na int


int generateLine(tLinePtr, BTNodePtr);
void copyString(char *, char*);

#endif
