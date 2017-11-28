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
    int compare;    // pomocna promenna pro uchovani hodnoty porovnani dvou stringu (identifikatoru)
    // compare = 0          ~ identifikatory jsou stejne
    // compare < 0          ~ hledany identifikator je mensi (pruchod do leveho syna)
    // compare > 0          ~ hledany identifikator je vetsi (pruchod do praveho syna)

    if(RootPtr == NULL)
        return NULL;

    BTItemPtr *var;
    BTNodePtr tmp = RootPtr;
    while(tmp != NULL){
        if(tmp->item != NULL) {
            if(tmp->item->ParamRootPtr != NULL)
                if((var = BTSearch(tmp->item->ParamRootPtr, searchedID)) != NULL)
                    return var;
//            if(tmp->item->funcData != NULL)
//                if((var = BTSearch(tmp->item->funcData->ParamRootPtr, searchedID)) != NULL)
//                    return var;
        }
        else
            break;
        compare = strcmp(searchedID, tmp->item->itemID);
        if(compare == 0)
            return tmp->item;
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
int BTInsertVarInt(BTNodePtr RootPtr, char *id, int value) {
    BTItemPtr *newData = (struct BTItem *) malloc(sizeof(struct BTItem));
    char *newid = (char *) malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(newid, id);
    newData->itemID = newid;
    newData->itemType = item_type_variable;
//    newData->funcData = NULL;
    newData->varData = malloc(sizeof(struct varData));
    if (newData->varData == NULL) {
        return 99;
    }
    newData->varData->type = var_integer;
    newData->varData->data = (int *) malloc(sizeof(int));
    if (newData->varData->data == NULL) {
        return 99;
    }
    *(int *) (newData->varData->data) = value;

    if(RootPtr->item == NULL){
        RootPtr->item = newData;
    }
    else
    BTInsert(RootPtr, newData);

    return 0;
}


// vlozeni promenne typu double do BVS
int BTInsertVarDouble(BTNodePtr RootPtr, char *id, double value) {
    BTItemPtr *newData = (struct BTItem *) malloc(sizeof(struct BTItem));
    char *newid = (char *) malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(newid, id);
    newData->itemID = newid;
    newData->itemType = item_type_variable;
//    newData->funcData = NULL;
    newData->varData = malloc(sizeof(struct varData));
    if (newData->varData == NULL) {
        return 99;
    }
    newData->varData->type = var_double;
    newData->varData->data = (double *) malloc(sizeof(double));
    if (newData->varData->data == NULL)
        return 99;
    *(double *) (newData->varData->data) = value;

    if(RootPtr->item == NULL){
        RootPtr->item = newData;
    }
    else
        BTInsert(RootPtr, newData);

    return 0;
}


// vlozeni promenne typu string do BVS
int BTInsertVarString(BTNodePtr RootPtr, char *id, char *value) {
    BTItemPtr *newData = (struct BTItem *) malloc(sizeof(struct BTItem));
    char *newid = (char *) malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(newid, id);
    newData->itemID = newid;
    newData->itemType = item_type_variable;
//    newData->funcData = NULL;
    newData->varData = malloc(sizeof(struct varData));
    if (newData->varData == NULL)
        return 99;

    newData->varData->type = var_string;
    int len = strlen(value);
    newData->varData->data = (char *) malloc(sizeof(char) * len);
    if (newData->varData->data == NULL)
        return 99;

    for (int i = 0; i < len; i++)
        ((char *) newData->varData->data)[i] = value[i];

    if(RootPtr->item == NULL){
        RootPtr->item = newData;
    }
        BTInsert(RootPtr, newData);

    return 0;
}

// vlozeni funkce do BVS (rootPtr), navratovy typ (returnType), identifikator (id),
// koren BVS parametru (parameters)
int BTInsertFunc(BTNodePtr RootPtr, varDataType returnType, char *id) {
    BTItemPtr *newItem = (struct BTItem *) malloc(sizeof(struct BTItem));
    if(newItem == NULL)
        return 99;
    char *newID = (char *) malloc(sizeof(char) * (strlen(id) + 1));
    if(newID == NULL) {
        free(newItem);
        return 99;
    }

    strcpy(newID, id);
    newItem->itemID = newID;
    newItem->declared = 0;
    newItem->defined = 0;
    newItem->paramCount = 0;
    newItem->itemType = item_type_function;
    newItem->returnType = returnType;
    newItem->ParamRootPtr = NULL;
    newItem->parameterTypes = NULL;

    newItem->ParamRootPtr = (struct BTNode *) malloc(sizeof(struct BTNode));
    newItem->ParamRootPtr->item = NULL;
    newItem->ParamRootPtr->RPtr = NULL;
    newItem->ParamRootPtr->LPtr = NULL;

    if(RootPtr->item == NULL)
        RootPtr->item = newItem;
    else{
        int result = BTInsert(RootPtr, newItem);
        if(result != 0){
//            free(newItem->funcData);
            free(newItem);
            return result;
        }
    }
    return 0;

//    BTItemPtr *newItem = (struct BTItem *) malloc(sizeof(struct BTItem));
//    if(newItem == NULL)
//        return 99;
//    char *newid = (char *) malloc(sizeof(char) * (strlen(id) + 1));//TODO:99
//    strcpy(newid, id);
//    newItem->itemID = newid;
//    newItem->itemType = item_type_function;
//    newItem->varData = NULL;
//    funcDataPtr funcData = (struct funcData *) malloc(sizeof(struct funcData));
//    if (funcData == NULL)
//        return 99;
//    newItem->funcData = funcData;
//    newItem->funcData->returnType = returnType;
//    newItem->funcData->ParamRootPtr = NULL;
//    newItem->funcData->parameterTypes = NULL;
//    newItem->declared = 0;
//    newItem->defined = 0;
//    newItem->paramCount = 0;
//
//    if(RootPtr->item == NULL)
//        RootPtr->item = newItem;
//    else{
//        int result = BTInsert(RootPtr, newItem);
//        if(result != 0)
//            return result;

    return 0;
}


void BTDispose(BTNodePtr RootPtr){
    if(RootPtr == NULL)
        return;

    if(RootPtr->RPtr != NULL){
        BTDispose(RootPtr->RPtr);
        RootPtr->RPtr = NULL;
    }
    if(RootPtr->LPtr != NULL){
        BTDispose(RootPtr->LPtr);
        RootPtr->LPtr = NULL;
    }

    if(RootPtr->item != NULL){
        if(RootPtr->item->ParamRootPtr != NULL){
            BTDispose(RootPtr->item->ParamRootPtr);
            RootPtr->item->ParamRootPtr = NULL;
        }
        if(RootPtr->item->parameterTypes != NULL){
            free(RootPtr->item->parameterTypes);
            RootPtr->item->parameterTypes = NULL;
        }
//        if(RootPtr->item->funcData != NULL){
//            if(RootPtr->item->funcData->parameterTypes != NULL){
//                free(RootPtr->item->funcData->parameterTypes);
//                RootPtr->item->funcData->parameterTypes = NULL;
//            }
//            if(RootPtr->item->funcData->ParamRootPtr != NULL){
//                BTDispose(RootPtr->item->funcData->ParamRootPtr);
//                RootPtr->item->funcData->ParamRootPtr = NULL;
//            }
//            free(RootPtr->item->funcData);
//            RootPtr->item->funcData = NULL;
//        }
        if(RootPtr->item->itemID != NULL){
            free(RootPtr->item->itemID);
            RootPtr->item->itemID = NULL;
        }
        if(RootPtr->item->varData != NULL){
            if(RootPtr->item->varData->data != NULL){
                free(RootPtr->item->varData->data);
                RootPtr->item->varData->data = NULL;
            }
            free(RootPtr->item->varData);
            RootPtr->item->varData = NULL;
        }
        free(RootPtr->item);
        RootPtr->item = NULL;
    }
    free(RootPtr);

//    if(RootPtr->item != NULL){
//        BTItemPtr *tmp = RootPtr->item;
//        if(tmp->funcData != NULL){
//            if(tmp->funcData->parameterTypes != NULL){
//                if(tmp->funcData->ParamRootPtr != NULL){
//                    BTDispose(tmp->funcData->ParamRootPtr);
//                    tmp->funcData->ParamRootPtr = NULL;
//                }
//                free(tmp->funcData->parameterTypes);
//                tmp->funcData->parameterTypes = NULL;
//            }
//            if(tmp->itemID != NULL){
//                free(tmp->itemID);
//                tmp->itemID = NULL;
//            }
//            free(tmp->funcData);
//            tmp->funcData = NULL;
//        }
//        if(tmp->itemID != NULL){
//            free(tmp->itemID);
//            tmp->itemID = NULL;
//        }
//        if(tmp->varData != NULL){
//            if(tmp->varData->data != NULL){
//                free(tmp->varData->data);
//                tmp->varData->data = NULL;
//            }
//            free(tmp->varData);
//            tmp->varData = NULL;
//        }
//        free(RootPtr->item);
//        RootPtr->item = NULL;
//    }
//    free(RootPtr);
}



//TODO: check return codes and frees, implement dispose
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