/************************************************************************/
/*	V tomto souboru se nachazi implementace tabulky symbolu		*/
/*	pomoci binarniho vyhledavaciho stromu a vsech funkci		*/
/*	potrebnych k manipulaci s timto stromem.			*/
/*									*/
/*	V souboru symtable.h jsou definice datovych typu		*/
/*	a prototypy funkci pouzivanych v tomto souboru.			*/
/************************************************************************/


#include "symtable.h"


//////////////////////////////////////////////////
///////         Zasobnik ukazatelu         ///////
//////////////////////////////////////////////////

// inicializace pomocneho zasobniku
void BTStackInit(BTStack *stack) {
    stack->top = 0;    // vrchol zasobniku na nulu
}

// vlozeni ukazatele na vrchol zasobniku
int BTStackPush(BTStack *stack, BTNodePtr ptr) {
    if (stack->top == STACK_MAX) {
        fprintf(stderr, "Chyba! Preteceni zasobniku s ukazateli.\n");
        return 0;
    }
    stack->top++;
    stack->st[stack->top] = ptr;
    return 1;
}

// precte a odstrani hodnotu z vrcholu zasobniku
BTNodePtr BTStackPop(BTStack *stack) {
    if (stack->top == 0) {
        fprintf(stderr, "Chyba! Podteceni zasobniku s ukazateli.\n");
        return NULL;
    }
    return (stack->st[stack->top--]);
}





////////////////////////////////////////////////////////////////
///////         Binarni vyhledavaci strom (BVS)	   	////////
////////////////////////////////////////////////////////////////

// inicializace BVS pred prvnim pouzitim
void BTInit(BTNodePtr BTRoot) {
    BTRoot->item = NULL;
    BTRoot->LPtr = NULL;
    BTRoot->RPtr = NULL;
}

// pomocna funkce
// vlozeni nove polozky do BVS
int BTInsert(BTNodePtr BTRoot, BTItemPtr *newItem) {
    int compare;    // pomocna promenna pro uchovani hodnoty porovnani dvou stringu (identifikatoru)
    // compare = 0		~ identifikatory jsou stejne
    // compare < 0 		~ novy identifikator je mensi
    // compare > 0		~ novy identifikator je vetsi

    BTNodePtr activeNode = BTRoot;        // prave aktivni uzel pri pruchodu stromem
    BTNodePtr activeNodeParent = activeNode;    // otec aktivniho uzlu


    while (activeNode != NULL)    // dokud nedojde na konec stromu (list stromu)
    {
        activeNodeParent = activeNode;        // aktualizace otce aktivniho uzlu
        compare = strcmp(newItem->itemID, activeNode->item->itemID);    // porovnani identifikatoru
        if (compare < 0)
            activeNode = activeNode->LPtr;    // pokracuje se levym podstromem
        else if (compare > 0)
            activeNode = activeNode->RPtr;    // pokracuje se pravym podstromem
        else    // identifikator jiz existuje
            return 3;
    }

    BTNodePtr newNode = (struct BTNode *) malloc(sizeof(struct BTNode));
    if(newNode == NULL)
        return 99;
    newNode->item = newItem;

    if(compare < 0)
        activeNodeParent->LPtr = newNode;
    else
        activeNodeParent->RPtr = newNode;
    return 0;
}

// najde polozku v BVS (RootPtr) podle identifikatoru (itemID) a vysledek vraci prostrednictvim ukazetele (output)
// navratova hodnota udava uspech (true) nebo neuspech (false)
BTItemPtr *BTSearch(BTNodePtr RootPtr, char *searchedID) {
    //TODO: rekurzivne
    int compare;    // pomocna promenna pro uchovani hodnoty porovnani dvou stringu (identifikatoru)
    // compare = 0          ~ identifikatory jsou stejne
    // compare < 0          ~ hledany identifikator je mensi (pruchod do leveho syna)
    // compare > 0          ~ hledany identifikator je vetsi (pruchod do praveho syna)

    if(RootPtr == NULL)
        return NULL;

    BTNodePtr tmp = RootPtr;
    while(tmp != NULL){
        compare = strcmp(searchedID, tmp->item->itemID);
        if(compare == 0)
            return RootPtr->item;
        else if(compare < 0){
            if(tmp->LPtr != NULL)
                tmp = tmp->LPtr;
            else
                return NULL;
        }
        else{
            if(tmp->RPtr != NULL)
                tmp = tmp->RPtr;
            else
                return NULL;
        }
    }


    return NULL;
}

// vlozeni promenne typu integer do BVS
int BTInsertVarInt(funcDataPtr RootPtr, char *id, int value) {
    BTItemPtr *newData = (struct BTItem *) malloc(sizeof(struct BTItem));
    newData->itemID = id;
    newData->itemType = item_type_variable;
    newData->varData = malloc(sizeof(struct varData));
    if (newData->varData == NULL) {
        return 99;
    }
    newData->varData->type = var_integer;
    newData->varData->data = (int *) malloc(sizeof(int));
    if (newData->varData->data == NULL)
        return 99;
    *(int *) (newData->varData->data) = value;

    if(RootPtr->ParamRootPtr == NULL){
        BTNodePtr newItem = (struct BTNode *) malloc(sizeof(struct BTNode));
        if(newItem == NULL)
            return 99;
        newItem->item = newData;
        RootPtr->ParamRootPtr = newItem;
    }
    else
        BTInsert(RootPtr->ParamRootPtr, newData);

    return 0;
}


// vlozeni promenne typu double do BVS
int BTInsertVarDouble(funcDataPtr  RootPtr, char *id, double value) {
    BTItemPtr *newData = (struct BTItem *) malloc(sizeof(struct BTItem));
    newData->itemID = id;
    newData->itemType = item_type_variable;
    newData->varData = malloc(sizeof(struct varData));
    if (newData->varData == NULL) {
        return 99;
    }
    newData->varData->type = var_double;
    newData->varData->data = (double *) malloc(sizeof(double));
    if (newData->varData->data == NULL)
        return 99;
    *(double *) (newData->varData->data) = value;

    if(RootPtr->ParamRootPtr == NULL){
        BTNodePtr newItem = (struct BTNode *) malloc(sizeof(struct BTNode));
        if(newItem == NULL)
            return 99;
        newItem->item = newData;
        RootPtr->ParamRootPtr = newItem;
    }
    else
        BTInsert(RootPtr->ParamRootPtr, newData);

    return 0;
}


// vlozeni promenne typu string do BVS
int BTInsertVarString(funcDataPtr RootPtr, char *id, char *value) {
    BTItemPtr *newData = (struct BTItem *) malloc(sizeof(struct BTItem));
    newData->itemID = id;
    newData->itemType = item_type_variable;
    newData->varData = malloc(sizeof(struct varData));
    if (newData->varData == NULL) {
        return 99;
    }
    newData->varData->type = var_string;
    int len = strlen(value);
    newData->varData->data = (char *) malloc(sizeof(char) * len);
    if (newData->varData->data == NULL)
        return 99;

    for (int i = 0; i < len; i++)
        ((char *) newData->varData->data)[i] = value[i];

    if(RootPtr->ParamRootPtr == NULL){
        BTNodePtr newItem = (struct BTNode *) malloc(sizeof(struct BTNode));
        if(newItem == NULL)
            return 99;
        newItem->item = newData;
        RootPtr->ParamRootPtr = newItem;
    }
    else
        BTInsert(RootPtr->ParamRootPtr, newData);

    return 0;
}

// vlozeni funkce do BVS (rootPtr), navratovy typ (returnType), identifikator (id),
// koren BVS parametru (parameters)
int BTInsertFunc(BTNodePtr RootPtr, varDataType returnType, char *id) {
    BTItemPtr *newItem = (struct BTItem *) malloc(sizeof(struct BTItem));
    if(newItem == NULL)
        return 99;
    newItem->itemID = id;
    newItem->itemType = item_type_function;
    newItem->funcData = (struct funcData *) malloc(sizeof(struct funcData));
    if (newItem->funcData == NULL) {
        return 99;
    }
    newItem->funcData->returnType = returnType;
    newItem->funcData->ParamRootPtr = NULL;
    newItem->declared = 0;
    newItem->defined = 0;
    newItem->paramCount = 0;

    if(RootPtr->item == NULL)
        RootPtr->item = newItem;
    else{
        int result = BTInsert(RootPtr, newItem);
        if(result != 0)
            return result;
    }

    return 0;
}


//TODO: nazev funkce se neuvolnuje, je ulozen ukazatel z tokenu, ktery se uvolni jinde
// zrusi cely BVS a korektne uvolni alokovanou pamet
//void BTDispose(BTNodePtr *BTRoot) {
//    BTStack stack;        // pomocny zasobnik ukazatelu pro nerekurzivni pruchod BVS
//    BTStackInit(&stack);    // inicializace zasobniku
//
//    do {
//        if ((*BTRoot) == NULL)        // pokud se dojde na konec BVS (list stromu)
//        {
//            if (stack.top != 0)    // zasobnik ukazatelu neni prazdny
//            {
//                (*BTRoot) = BTStackPop(&stack);
//            }
//        } else {
//            if ((*BTRoot)->RPtr != NULL)    // existuje pravy podstrom
//            {
//                BTStackPush(&stack, (*BTRoot)->RPtr);
//            }
//            BTNodePtr deletedNode = (*BTRoot);
//            (*BTRoot) = (*BTRoot)->LPtr;    // pruchod do leveho podstromu
//
//            if ((*deletedNode->item)->itemType == item_type_variable) // jedna se o promennou
//            {
//                free((*deletedNode->item)->varData->data);
//                free((*deletedNode->item)->varData);
//            } else {
//                free((*deletedNode->item)->funcData);
//            }
//
//            free((*deletedNode->item)->itemID);    // uvolneni alokovane pameti pro itemID
//            free(deletedNode);        // zruseni uzlu
//        }
//    } while ((*BTRoot) != NULL || (stack.top != 0));    // nejsme v listu nebo zasobnik neni prazdny
//    free(BTRoot);    // zruseni ukazatele na koren BVS
//}


////TODO: check return codes and frees, implement dispose
//int main(){
//    BTNodePtr strom = (struct BTNode *) malloc(sizeof(struct BTNode));
//    BTInit(strom);
//    char *f1 = (char*) malloc(sizeof(char)*20);
//    char *f2 = (char*) malloc(sizeof(char)*20);
//    char *f3 = (char*) malloc(sizeof(char)*20);
//    char *f4 = (char*) malloc(sizeof(char)*20);
//    char *f5 = (char*) malloc(sizeof(char)*20);
//    strcpy(f1, "foo");
//    BTInsertFunc(strom, var_integer, f1);
//    strcpy(f2, "xyz");
//    BTInsertFunc(strom, var_string, f2);
//    strcpy(f3, "abc");
//    BTInsertFunc(strom, var_string, f3);
//    strcpy(f4, "def");
//    BTInsertFunc(strom, var_string, f4);
//    strcpy(f5, "khl");
//    BTInsertFunc(strom, var_string, f5);
//
//
//    char *v1 = (char*) malloc(sizeof(char)*20);
//    char *v2 = (char*) malloc(sizeof(char)*20);
//    char *v3 = (char*) malloc(sizeof(char)*20);
//    char *v4 = (char*) malloc(sizeof(char)*20);
//    char *v5 = (char*) malloc(sizeof(char)*20);
//    strcpy(v4, "prom_c");
//    strcpy(v1, "prom_x");
//    strcpy(v2, "prom_a");
//    strcpy(v3, "prom_b");
//    strcpy(v5, "prom_e");
//    BTInsertVarInt(strom->item->funcData, v4, 100);
//    BTInsertVarInt(strom->item->funcData, v1, 10);
//    BTInsertVarInt(strom->item->funcData, v2, 1);
//    BTInsertVarInt(strom->item->funcData, v3, 19);
//    BTInsertVarInt(strom->item->funcData, v5, 189);
//    int *data;
//    if(BTSearch(strom, f1) != NULL)
//        if(BTSearch(strom, f3)!= NULL)
//            if(BTSearch(strom, f2)!= NULL)
//                if(BTSearch(strom, f4)!= NULL)
//                    *&data = strom->item->funcData->ParamRootPtr->item->varData->data;
//
//    return 0;
//}