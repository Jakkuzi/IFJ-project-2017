/************************************************************************/
/*      Toto je header pro soubor symbol_table.c	 	        */
/*      Zde budou includovany knihovny, definice datovych typu		*/
/*	a prototypy funkci pro praci s binarnim vyhledavacim 		*/
/*	stromem.							*/
/************************************************************************/



#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#define STACK_MAX 100	// maximalni velikost pomocneho zasobniku


///////////////////////////////////////////
///////         Datove typy         ///////
///////////////////////////////////////////

// datove typy promennych
typedef enum { 
	varInteger,
	varDouble,
	varFloat,
	varChar,
	varBool
} varDataType;

// struktura popisujici jednu promennou
typedef struct BTItem {
	char* varID;		// klic, podle ktereho bude uzel vyhledavan (identifikator promenne)
	varDataType type;	// datovy typ promenne
	void* data;		// efektivni (uzitecna) data, pretypovani ukazatele podle datoveho typu promenne
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
void ErrMessage(int, char *);
void BTInit(BTNodePtr *);			// inicializace stromu, parametrem je ukazatel na ukazetel na koren
bool BTInsert(BTNodePtr *, BTItem * );		// vlozeni nove polozky do stromu, predani struktury ukazatelem (efektivnejsi)

void BTStackInit(BTStack *);			// inizializace pomocneho zasobniku ukazatelu
bool BTStackPush(BTStack *, BTNodePtr); 	// vlozeni ukazatele na vrchol zasobniku
BTNodePtr BTStackPop(BTStack *);		// precteni vrcholu zasobniku a snizeni vrcholu zasobniku
