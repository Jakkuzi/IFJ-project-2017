/************************************************************************/
/*	V tomto souboru se nachazi implementace tabulky symbolu		*/
/*	pomoci binarniho vyhledavaciho stromu a vsech funkci		*/
/*	potrebnych k manipulaci s timto stromem.			*/
/*									*/
/*	V souboru symbol_table.h jsou definice datovych typu		*/
/*	a prototypy funkci pouzivanych v tomto souboru.			*/
/************************************************************************/


#include "symbol_table.h"

///////////////////////////////////////////////
///////		Chybova hlaseni		///////
///////////////////////////////////////////////

// vypise chybove hlaseni na standartni chybovy vystup
void ErrMessage(int ErrCode, char *varID)
{
	fprintf(stderr, "%s\t", varID);	// vypsani nazvu promenne se kterou je problem

	switch(ErrCode)
	{
		case 2:
			fprintf(stderr, "Chyba alokace pameti pri pokusu o vlozeni nove polozky do binarniho stromu\n");
			break;
		case 3:
			fprintf(stderr, "Chyba! Pokus o deklaraci promenne, ktera jiz existuje.\n");
			break;
		default:
			fprintf(stderr, "Neznama chyba.\n");
			break;
	}
}


//////////////////////////////////////////////////
///////         Zasobnik ukazatelu         ///////
//////////////////////////////////////////////////

// inicializace pomocneho zasobniku
void BTStackInit(BTStack *stack)
{
	stack->top = 0;	// vrchol zasobniku na nulu
}

// vlozeni ukazatele na vrchol zasobniku
bool BTStackPush(BTStack *stack, BTNodePtr ptr)
{
	if(stack->top==STACK_MAX)
	{
		fprintf(stderr, "Chyba! Preteceni zasobniku s ukazateli.\n");
		return false;
	}
	stack->top++;
	stack->st[stack->top] = ptr;
	return true;
}

// precte a odstrani hodnotu z vrcholu zasobniku
BTNodePtr BTStackPop(BTStack *stack)
{
	if(stack->top == 0)
	{
		fprintf(stderr, "Chyba! Podteceni zasobniku s ukazateli.\n");
		return NULL;
	}
	return (stack->st[stack->top--]);
}





////////////////////////////////////////////////////////////////
///////         Binarni vyhledavaci strom (BVS)	   	////////
////////////////////////////////////////////////////////////////

// inicializace BVS pred prvnim pouzitim
void BTInit(BTNodePtr *BTRoot)
{
	(*BTRoot) = NULL;
}

// vlozeni nove polozky do BVS
bool BTInsert(BTNodePtr *BTRoot, BTItem *newItem)
{
	int compare;	// pomocna promenna pro uchovani hodnoty porovnani dvou stringu (identifikatoru promennych)
			// compare = 0		~ identifikatory jsou stejne
			// compare < 0 		~ novy identifikator je mensi
			// compare > 0		~ novy identifikator je vetsi

	BTNodePtr activeNode = (*BTRoot);		// prave aktivni uzel pri pruchodu stromem
	BTNodePtr activeNodeParent = activeNode;	// otec aktivniho uzlu


	while( activeNode != NULL)	// dokud nedojde na konec stromu (list stromu)
	{	
		activeNodeParent = activeNode;		// aktualizace otce aktivniho uzlu
		compare = strcmp(newItem->varID, activeNode->item.varID);	// porovnani identifikatoru
		if(compare < 0)
		{
			activeNode = activeNode->LPtr;	// pokracuje se levym podstromem
		}
		else if(compare > 0)
		{
			activeNode = activeNode->RPtr;	// pokracuje se pravym podstromem
		}
		else	// identifikator jiz existuje
		{
			ErrMessage(2, newItem->varID);	// chybove hlaseni
			return false;
		}
	}

	BTNodePtr newNode = (BTNodePtr) malloc(sizeof(struct BTNode));	// vytvoreni noveho uzlu

	if(newNode == NULL)	// chyba alokace pameti
	{
		ErrMessage(3, newItem->varID);
		return false;
	}

        newNode->item = (*newItem);	// naplneni nove alokovaneho uzlu
        newNode->LPtr = NULL;
        newNode->RPtr = NULL;

	if( (*BTRoot) == NULL)	// strom je prazdny, pridava se prvni uzel
	{
		(*BTRoot) = newNode;
	}
	else if(compare < 0)	// novy uzel se prida do leveho podstromu (varID je mensi)
	{
		activeNodeParent->LPtr = newNode;
	}
	else if(compare > 0)	// novy uzel se prida do praveho podstromu (varID je vetsi)
	{
		activeNodeParent->RPtr = newNode;
	}

	return true;	// novy uzel byl uspesne pridan
}

// zrusi cely BVS a korektne uvolni alokovanou pamet
void BTDispose(BTNodePtr *BTRoot)
{
	BTStack stack;		// pomocny zasobnik ukazatelu pro nerekurzivni pruchod BVS
	BTStackInit(&stack);	// inicializace zasobniku

	do
	{
		if( (*BTRoot) == NULL)		// pokud se dojde na konec BVS (list stromu)
		{
			if(stack.top != 0)	// zasobnik ukazatelu neni prazdny
			{
				(*BTRoot) = BTStackPop(&stack);
			}
		}
		else
		{
			if( (*BTRoot)->RPtr != NULL)	// existuje pravy podstrom
			{
				BTStackPush(&stack, (*BTRoot)->RPtr);
			}
			BTNodePtr deletedNode;		// ruseny uzel
			deletedNode = (*BTRoot);
			(*BTRoot) = (*BTRoot)->LPtr;	// pruchod do leveho podstromu
			
			free(deletedNode->item.varID);	// uvolneni alokovane pameti pro varID
			free(deletedNode);		// zruseni uzlu
		}
	}
	while( (*BTRoot) != NULL || (stack.top != 0));	// nejsme v listu nebo zasobnik neni prazdny
}



////////////////////////////////////////////////
///////         Testovaci funkce         ///////
////////////////////////////////////////////////


// TEMPORARY // Graficky zobrazi cely BVS //
void BTPrint(BTNodePtr TempTree, char *sufix, char fromdir)
{
     if (TempTree != NULL)
     {
        char* suf2 = (char*) malloc(strlen(sufix) + 4);
        strcpy(suf2, sufix);
        if (fromdir == 'L')
        {
           suf2 = strcat(suf2, "  |");
           printf("%s\n", suf2);
        }
        else
           suf2 = strcat(suf2, "   ");
        BTPrint(TempTree->RPtr, suf2, 'R');
        printf("%s  +-[%s]\n", sufix, TempTree->item.varID);
        strcpy(suf2, sufix);
        if (fromdir == 'R')
           suf2 = strcat(suf2, "  |");
        else
           suf2 = strcat(suf2, "   ");
        BTPrint(TempTree->LPtr, suf2, 'L');
        if (fromdir == 'R') printf("%s\n", suf2);
        free(suf2);
    }
}


// TEMPORARY // Generuje pseudonahodne nazvy promennych (delka stringu 3-10) z malych pismen abecedy (a-z) //
char *ID_generate()
{
	int length = rand() % 10 + 3;

	char *str = (char*) malloc(sizeof(char)*length);
	
	for(int i = 0; i < length; i++)
	{
		str[i] = rand() % 26 + 97;
	}
	return str;
}


// TEMPORARY // Testovaci funkce //
int main()
{

	const int pocetID = 20;	// pocet generovanych uzlu stromu (nazvu promennych)

	BTItem item[pocetID];	// vytvoreni statickeho pole techto promennych


        BTNodePtr *BTRoot = malloc(sizeof(BTNodePtr));	// koren stromu

        BTInit(BTRoot);					// inicializace stromu pred pouzitim


	for(int i = 0; i < pocetID; i++)		// cyklus vygeneruje nahodne nazvy promennych
	{						// a vlozei je do BVS
		item[i].varID = ID_generate();

	        BTInsert(BTRoot, &(item[i]));
	}

	BTPrint((*BTRoot), "", 'X');		// zobrazeni stromu
	
	BTDispose(BTRoot);			// nerekurzivni smazani celeho stromu (uvolneni pameti)


	free(BTRoot);				// uvolneni ukazatele na koren BVS

	return 0;
}
