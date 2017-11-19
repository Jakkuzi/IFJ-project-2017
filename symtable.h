/************************************************************************/
/*      Toto je header pro soubor symtable.c	 	        	*/
/*      Zde budou includovany knihovny, definice datovych typu		*/
/*	a prototypy funkci pro praci s binarnimi vyhledavacimi 		*/
/*	stromy.								*/
/************************************************************************/

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

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
	var_bool,
	var_void
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

// struktura popisujici data funkce
typedef struct funcData {
        varDataType returnType; // datovy typ navratove hodnoty promenne
	BTNode **ParamRootPtr;	// ukazatel (na ukazatel) na koren binarniho stromu parametru funkce
} *funcDataPtr;

// struktura popisujici jednu promennou
typedef struct BTItem {
	char* itemID;		// klic, podle ktereho bude uzel vyhledavan (identifikator promenne)
	itemType itemType;	// typ polozky (promenna nebo funkce)
	varDataPtr varData;	// ukazatel na data promenne (pokud je itemType = item_type_variable)
	funcDataPtr funcData;	// ukazatel na data funkce (pokud je itemType = item_type_function)
} BTItem;

// jeden uzel binarniho vyhledavaciho stromu (BVS)
typedef struct BTNode {
	BTItem item;		// struktura popisujici uzitecnou hodnotu (promennou)
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
void ErrMessage(int, char *);
bool BTInsert(BTNodePtr *, BTItem * );		// vlozeni nove polozky do stromu, predani struktury ukazatelem (efektivnejsi)

void BTStackInit(BTStack *);			// inizializace pomocneho zasobniku ukazatelu
bool BTStackPush(BTStack *, BTNodePtr); 	// vlozeni ukazatele na vrchol zasobniku
BTNodePtr BTStackPop(BTStack *);		// precteni vrcholu zasobniku a snizeni vrcholu zasobniku


/// API rozhrani - funkce, ktere pouzivejte ///
void BTInit(BTNodePtr *);                       // inicializace stromu, parametrem je ukazatel na
bool BTSearch(BTNodePtr, char *, BTItem *);	// najde polozku podle identifikatoru (char *)
						// v BVS (BTNodePtr) a vrati ji pres ukazatel (BTItem *)
bool BTInsertVarInt(BTNodePtr *, char *, int);		// vlozi integer
bool BTInsertVarDouble(BTNodePtr *, char *, double);	// vlozi double
bool BTInsertVarString(BTNodePtr *, char *, char *);	// vlozi string

bool BTInsertFunc(BTNodePtr *, varDataType, char *, BTNodePtr *);	// vlozi funkci

void BTDispose(BTNodePtr *);			// nerekurzivne zrusi cely strom (uvolni alokovanou pamet)
						// vyuziva zasobniku ukazatelu BTStack


#endif