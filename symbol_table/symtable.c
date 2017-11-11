/************************************************************************/
/*	V tomto souboru se nachazi implementace tabulky symbolu		*/
/*	pomoci binarniho vyhledavaciho stromu a vsech funkci		*/
/*	potrebnych k manipulaci s timto stromem.			*/
/*									*/
/*	V souboru symtable.h jsou definice datovych typu		*/
/*	a prototypy funkci pouzivanych v tomto souboru.			*/
/************************************************************************/


#include "symtable.h"

///////////////////////////////////////////////
///////		Chybova hlaseni		///////
///////////////////////////////////////////////

// vypise chybove hlaseni na standartni chybovy vystup
void ErrMessage(int ErrCode, char *itemID)
{
	fprintf(stderr, "[%s]\t", itemID);	// vypsani nazvu polozky se kterou je problem

	switch(ErrCode)
	{
		case 2:
			fprintf(stderr, "Chyba alokace pameti pri pokusu o vlozeni nove polozky do binarniho stromu\n");
			break;
		case 3:
			fprintf(stderr, "Chyba! Pokus o deklaraci polozky, ktera jiz existuje.\n");
			break;
		case 4:
			fprintf(stderr, "Chyba! Neznamy identifikator.\n");
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
	int compare;	// pomocna promenna pro uchovani hodnoty porovnani dvou stringu (identifikatoru)
			// compare = 0		~ identifikatory jsou stejne
			// compare < 0 		~ novy identifikator je mensi
			// compare > 0		~ novy identifikator je vetsi

	BTNodePtr activeNode = (*BTRoot);		// prave aktivni uzel pri pruchodu stromem
	BTNodePtr activeNodeParent = activeNode;	// otec aktivniho uzlu


	while( activeNode != NULL)	// dokud nedojde na konec stromu (list stromu)
	{
		activeNodeParent = activeNode;		// aktualizace otce aktivniho uzlu
		compare = strcmp(newItem->itemID, activeNode->item.itemID);	// porovnani identifikatoru
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
			ErrMessage(2, newItem->itemID);	// chybove hlaseni
			return false;
		}
	}

	BTNodePtr newNode = (BTNodePtr) malloc(sizeof(struct BTNode));	// vytvoreni noveho uzlu

	if(newNode == NULL)	// chyba alokace pameti
	{
		ErrMessage(3, newItem->itemID);
		return false;
	}

        newNode->item = (*newItem);	// naplneni nove alokovaneho uzlu
        newNode->LPtr = NULL;
        newNode->RPtr = NULL;

	if( (*BTRoot) == NULL)	// strom je prazdny, pridava se prvni uzel
	{
		(*BTRoot) = newNode;
	}
	else if(compare < 0)	// novy uzel se prida do leveho podstromu (itemID je mensi)
	{
		activeNodeParent->LPtr = newNode;
	}
	else if(compare > 0)	// novy uzel se prida do praveho podstromu (itemID je vetsi)
	{
		activeNodeParent->RPtr = newNode;
	}

	return true;	// novy uzel byl uspesne pridan
}


// najde polozku v BVS (RootPtr) podle identifikatoru (itemID) a vysledek vraci prostrednictvim ukazetele (output)
// navratova hodnota udava uspech (true) nebo neuspech (false)
bool BTSearch(BTNodePtr RootPtr, char *searchedID, BTItem *output)
{

        int compare;    // pomocna promenna pro uchovani hodnoty porovnani dvou stringu (identifikatoru)
                        // compare = 0          ~ identifikatory jsou stejne
                        // compare < 0          ~ hledany identifikator je mensi (pruchod do leveho syna)
                        // compare > 0          ~ hledany identifikator je vetsi (pruchod do praveho syna)

	if(RootPtr == NULL)	// BVS je prazdny. Nema cenu hledat, protoze polozka neexistuje
	{
                ErrMessage(4 ,searchedID);	// chybove hlaseni
		return false;			// konec funkce s neuspechem
	}

	do
	{
		compare = strcmp(searchedID, RootPtr->item.itemID);	// porovnani hledaneho a aktualniho id.
		if(compare == 0)	// identifikatory se shoduji
		{
			(*output) = RootPtr->item;	// vraceni struktury skrz ukazatel output
			return true;			// konec funkce s uspechem
		}
		else if(compare < 0)	// hledany identifikator je mensi nez aktualni
		{
			if(RootPtr->LPtr == NULL)	// nelze projit do leveho syna
			{
				ErrMessage(4 ,searchedID);	// chybove hlaseni
				return false;			// konec funkce s neuspechem
			}
			RootPtr = RootPtr->LPtr;		// pruchod do leveho syna
		}
		else if (compare > 0)	// hledany identifikator je vetsi nez aktualni
		{
			if(RootPtr->RPtr == NULL)	// nelze projit do praveho syna
			{
                                ErrMessage(4 ,searchedID);	// chybove hlaseni
                                return false;			// konec funkce s neuspechem
                        }
			RootPtr = RootPtr->RPtr;		// pruchod do praveho syna
		}
	}
	while(RootPtr != NULL);		// teoreticky by ke konci cyklu nikdy nemelo dojit
					// while(1) tam neni kvuli pripadne chybe v implementaci a zacykleni

	return false;	// Pouze kvuli warningu standardu jazyka C. Sem by funkce nikdy nemela dojit.
}

// vlozeni promenne typu integer do BVS
bool BTInsertVarInt(BTNodePtr *RootPtr, char *id, int value)
{
	BTItem newItem;
	newItem.itemID = id;
	newItem.itemType = item_type_variable;
	newItem.varData = malloc(sizeof(struct varData));
	if(newItem.varData == NULL)
	{
		ErrMessage(2 ,id);
		return false;
	}
	newItem.varData->type = var_integer;
	newItem.varData->data = (int *) malloc(sizeof(int));
	if(newItem.varData->data == NULL)
	{
		ErrMessage(2, id);
		return false;
	}
	*(int *)(newItem.varData->data) = value;

	if(BTInsert(RootPtr, &newItem))
	{
		return true;
	}
	return false;
}

// vlozeni promenne typu double do BVS
bool BTInsertVarDouble(BTNodePtr *RootPtr, char *id, double value)
{
        BTItem newItem;
        newItem.itemID = id;
        newItem.itemType = item_type_variable;
        newItem.varData = malloc(sizeof(struct varData));
        if(newItem.varData == NULL)
        {
                ErrMessage(2 ,id);
                return false;
        }
        newItem.varData->type = var_double;
        newItem.varData->data = (double *) malloc(sizeof(double));
        if(newItem.varData->data == NULL)
        {
                ErrMessage(2, id);
                return false;
        }
        *(double *)(newItem.varData->data) = value;

        if(BTInsert(RootPtr, &newItem))
        {
                return true;
        }
        return false;
}


// vlozeni promenne typu string do BVS
bool BTInsertVarString(BTNodePtr *RootPtr, char *id, char *value)
{
        BTItem newItem;
        newItem.itemID = id;
        newItem.itemType = item_type_variable;
        newItem.varData = malloc(sizeof(struct varData));
        if(newItem.varData == NULL)
        {
                ErrMessage(2 ,id);
                return false;
        }
        newItem.varData->type = var_string;
	int length = strlen(value);
        newItem.varData->data = (char *) malloc(sizeof(char) * length);
        if(newItem.varData->data == NULL)
        {
                ErrMessage(2, id);
                return false;
        }

	for(int i = 0; i < length; i++)
	{
        	((char *)newItem.varData->data)[i] = value[i];
	}

        if(BTInsert(RootPtr, &newItem))
        {
                return true;
        }
        return false;
}

// vlozeni funkce do BVS (rootPtr), navratovy typ (returnType), identifikator (id),
// koren BVS parametru (parameters)
bool BTInsertFunc(BTNodePtr *RootPtr, varDataType returnType, char *id, BTNodePtr *parameters)
{
	BTItem newItem;
        newItem.itemID = id;
        newItem.itemType = item_type_function;
        newItem.funcData = malloc(sizeof(struct funcData));
        if(newItem.funcData == NULL)
        {
                ErrMessage(2 ,id);
                return false;
        }
        newItem.funcData->returnType = returnType;
        newItem.funcData->ParamRootPtr = parameters;

        if(BTInsert(RootPtr, &newItem))
        {
                return true;
        }
        return false;
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
			///////////////////////////////////////////
			// TEMPORARY // VYPIS // POTOM ODSTRANIT //
			if((*BTRoot)->item.itemType == item_type_variable)
			{
				if((*BTRoot)->item.varData->type == var_integer)
					printf("%s\t%d\n",(*BTRoot)->item.itemID, *(int *)((*BTRoot)->item.varData->data));
				else if((*BTRoot)->item.varData->type == var_double)
					printf("%s\t%f\n",(*BTRoot)->item.itemID, *(double *)((*BTRoot)->item.varData->data));
				else if((*BTRoot)->item.varData->type == var_string)
					printf("%s\t%s\n",(*BTRoot)->item.itemID, (char *)((*BTRoot)->item.varData->data));
			}
			else
			{
				printf("\nfunkce:\n");
				switch((*BTRoot)->item.funcData->returnType)
				{
					case var_integer: printf("int"); break;
					case var_double: printf("double"); break;
					case var_string: printf("char *"); break;
					case var_bool:	printf("bool"); break;
					case var_void:	printf("void"); break;
				}
				printf(" %s(parametry nelze jednoduse zobrazit)\n",(*BTRoot)->item.itemID);
			}
			// 		TEMPORARY END		 //
			///////////////////////////////////////////

			if( (*BTRoot)->RPtr != NULL)	// existuje pravy podstrom
			{
				BTStackPush(&stack, (*BTRoot)->RPtr);
			}
			BTNodePtr deletedNode;		// ruseny uzel
			deletedNode = (*BTRoot);
			(*BTRoot) = (*BTRoot)->LPtr;	// pruchod do leveho podstromu

			if(deletedNode->item.itemType == item_type_variable) // jedna se o promennou
			{
				free(deletedNode->item.varData->data);
				free(deletedNode->item.varData);
			}
			else
			{
				free(deletedNode->item.funcData);
			}

			free(deletedNode->item.itemID);	// uvolneni alokovane pameti pro itemID
			free(deletedNode);		// zruseni uzlu
		}
	}
	while( (*BTRoot) != NULL || (stack.top != 0));	// nejsme v listu nebo zasobnik neni prazdny
	free(BTRoot);	// zruseni ukazatele na koren BVS
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
        printf("%s  +-[%s]\n", sufix, TempTree->item.itemID);
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


// TEMPORARY // Generuje pseudonahodne identifikatory (delka stringu 3-10) z malych pismen abecedy (a-z) //
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
	const int pocetID = 20;	// pocet generovanych uzlu dvou stromu promennych

	// koren stromu
        BTNodePtr *BTRoot = malloc(sizeof(BTNodePtr));	// strom promennych
        BTNodePtr *BTRoot_2 = malloc(sizeof(BTNodePtr));  // strom promennych
        BTNodePtr *BTRoot_func = malloc(sizeof(BTNodePtr));  // strom funkci

	// inicializace stromu pred pouzitim
        BTInit(BTRoot);
	BTInit(BTRoot_2);
	BTInit(BTRoot_func);

	// vygenerovani prvniho stromu promennych (z prvni poloviny "pocetID")
	int i;
	for(i = 0; i < pocetID / 2; i++)		// cyklus vygeneruje nahodne nazvy (identifikatory)
	{						// a vlozei je do BVS
		if((i % 3) == 0)
			BTInsertVarString(BTRoot, ID_generate(), "string123");
		else if((i % 2) == 0)
			BTInsertVarInt(BTRoot, ID_generate(), (rand() % 100));
		else
		{
			double value = (rand() % 1000 + 1);
			value /= (rand() % 100 + 1);
			BTInsertVarDouble(BTRoot, ID_generate(), value);
		}
	}

	// vygenerovani druheho stromu promennych (z druhe poloviny "pocetID")
	for(i=i; i < pocetID - 1; i++)            // cyklus vygeneruje nahodne nazvy (identifikatory)
        {                                               // a vlozei je do BVS
                if((i % 3) == 0)
                        BTInsertVarString(BTRoot_2, ID_generate(), "string567");
                else if((i % 2) == 0)
                        BTInsertVarInt(BTRoot_2, ID_generate(), (rand() % 100));
                else
                {
                        double value = (rand() % 1000 + 1);
                        value /= (rand() % 100 + 1);
                        BTInsertVarDouble(BTRoot_2, ID_generate(), value);
                }
        }

	// vlozeni posledniho prvku s identifikatorem "hledane_ID", do druheho stromu promennych
	char *searchedID = malloc(sizeof("hledane_ID"));
	searchedID[0] = 'h';
        searchedID[1] = 'l';
        searchedID[2] = 'e';
        searchedID[3] = 'd';
        searchedID[4] = 'a';
        searchedID[5] = 'n';
        searchedID[6] = 'e';
        searchedID[7] = '_';
        searchedID[8] = 'I';
        searchedID[9] = 'D';
	BTInsertVarInt(BTRoot_2, searchedID, 42);

	// vlozeni funkce do stromu funkci
	BTInsertFunc(BTRoot_func, var_integer, ID_generate(), BTRoot_2);

	printf("\n\nVypis prvniho stromu:\n");
        BTPrint((*BTRoot), "", 'X');            // zobrazeni stromu

	printf("\n\nVypis druheho stromu:\n");
        BTPrint((*BTRoot_2), "", 'X');            // zobrazeni stromu

	// chceme zkusit najit promenou s identifikatorem "hledane_ID"
	BTItem searchedItem;	// hledana polozka
	searchedItem.itemID = "hledane_ID";
	BTItem foundItem;	// nalezena polozka

	printf("\n\nPokus o vyhledani polozky \"%s\"\n", searchedItem.itemID);

	if( BTSearch( (*BTRoot_2), searchedItem.itemID, &foundItem ) )
	{
		printf("\nNalezeno: %s\n\n", foundItem.itemID);
	}
	else
	{
		printf("\nNenalezeno\n\n");
	}

	// zruseni vsech stromu
	printf("\n\nZruseni vsech stromu - vypis prvku\n");
	BTDispose(BTRoot);			// nerekurzivni smazani celeho stromu (uvolneni pameti)
	BTDispose(BTRoot_2);
	BTDispose(BTRoot_func);

	return 0;
}
