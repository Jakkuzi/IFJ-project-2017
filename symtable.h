/*
 * Implementace prekladace imperativního jazyka IFJ17.
 * xwilla00 - Tomas Willaschek
 * xmarek67 - Ondrej Marek
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STACK_MAX 1000	// maximalni velikost pomocneho zasobniku


///////////////////////////////////////////
///////         Datove typy         ///////
///////////////////////////////////////////

typedef struct BTNode BTNode;   // deklarace kvuli pouziti pred definici (pouzito ve strukture funcData)

// datove typy promennych
typedef enum {
    var_integer,
    var_double,
    var_string,
//    var_bool,
//    var_void
} varDataType;

// typ polozky BVS (promenna nebo funkce)
typedef enum {
    item_type_variable,
    item_type_function
} itemType;

// struktura popisujici data promenne
typedef struct varData {
    varDataType type;       // datovy typ promenne
    void* data;             // efektivni (uzitecna) data, pretypovani ukazatele podle datoveho typu promenne
} *varDataPtr;

// struktura popisujici jednu promennou
typedef struct BTItem {
    char* itemID;		// klic, podle ktereho bude uzel vyhledavan (identifikator promenne)
    itemType itemType;	// typ polozky (promenna nebo funkce)
    varDataPtr varData;	// ukazatel na data promenne (pokud je itemType = item_type_variable)
    varDataType returnType;
    int *parameterTypes;
    struct BTNode *ParamRootPtr;
    int declared;
    int defined;
    int paramCount;
} BTItemPtr;

// jeden uzel binarniho vyhledavaciho stromu (BVS)
typedef struct BTNode {
    BTItemPtr *item;		// struktura popisujici uzitecnou hodnotu (promennou)
    struct BTNode *LPtr;	// ukazatel na levy podstrom
    struct BTNode *RPtr;	// ukazatel na pravy podstrom
} *BTNodePtr;		// ukazatel na uzel

// pomocny zasobnik pro uchovani ukazatelu na uzly
typedef struct {
    BTNodePtr st[STACK_MAX];
    int top;
} BTStack;


////////////////////////////////////////////////
///////         Prototypy funkci         ///////
////////////////////////////////////////////////

/* navratove typy (bool) nekterych nasledujicich funkci slouzi pro zjisteni spravnosti provedeni funkce */

/// Vnitrni funkce ///
int BTInsert(BTNodePtr, BTItemPtr *);		// vlozeni nove polozky do stromu, predani struktury ukazatelem (efektivnejsi)

/// API rozhrani - funkce, ktere pouzivejte ///
void BTInit(BTNodePtr);                       // inicializace stromu, parametrem je ukazatel na
BTItemPtr *BTSearch(BTNodePtr, char *);	// najde polozku podle identifikatoru (char *)
						// v BVS (BTNodePtr) a vrati ji pres ukazatel (BTItem *)

int BTInsertVarInt(BTNodePtr , char *, int);		// vlozi integer
int BTInsertVarDouble(BTNodePtr , char *, double);	// vlozi double
int BTInsertVarString(BTNodePtr , char *, char *);	// vlozi string


int BTInsertFunc(BTNodePtr, varDataType, char *);	// vlozi funkci

void BTDispose(BTNodePtr);			// nerekurzivne zrusi cely strom (uvolni alokovanou pamet)
// vyuziva zasobniku ukazatelu BTStack


#endif