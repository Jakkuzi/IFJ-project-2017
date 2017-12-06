/*
 * Implementace prekladace imperativního jazyka IFJ17.
 * xmarek67 - Ondrej Marek
 */

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
#define Float2IntOsek 203	// oseknuti desetinne casti
#define NoDataConversion 250 // netreba implicitni datova konverze
#define StrConcate 251	// konkaternace stringu

void output_print();
void IFJcode17_embeddedFunctions();	// vypise 4 vestavene funkce do vysledneho kodu
int generateLine(tLinePtr, BTNodePtr);
void IFJcode17_writeString(char *myString);

#endif
