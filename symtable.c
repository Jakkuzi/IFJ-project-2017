/*
 * Implementace prekladace imperativního jazyka IFJ17.
 * xwilla00 - Tomas Willaschek
 * xmarek67 - Ondrej Marek
 */

#include "symtable.h"


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
    newNode->LPtr = NULL;
    newNode->RPtr = NULL;


    if(compare < 0)
        activeNodeParent->LPtr = newNode;
    else
        activeNodeParent->RPtr = newNode;

    return 0;
}

// najde polozku v BVS (RootPtr) podle identifikatoru (itemID) a vysledek vraci prostrednictvim ukazetele (output)
// navratova hodnota udava uspech (true) nebo neuspech (false)
BTItemPtr *BTSearch(BTNodePtr RootPtr, char *searchedID) {
    int compare = 0;    // pomocna promenna pro uchovani hodnoty porovnani dvou stringu (identifikatoru)
    BTItemPtr *var = NULL;
    BTNodePtr tmp = RootPtr;
    // compare = 0          ~ identifikatory jsou stejne
    // compare < 0          ~ hledany identifikator je mensi (pruchod do leveho syna)
    // compare > 0          ~ hledany identifikator je vetsi (pruchod do praveho syna)

    if(RootPtr == NULL) {
        return NULL;
    }

    if(RootPtr->RPtr != NULL) {
        var = BTSearch(RootPtr->RPtr, searchedID);
        if(var != NULL)
            return var;
    }
    if(RootPtr->LPtr != NULL){
        var = BTSearch(RootPtr->LPtr, searchedID);
        if(var != NULL)
            return var;
    }

    while(tmp != NULL){
        if(tmp->item != NULL) {
            if(tmp->item->ParamRootPtr != NULL)
                if((var = BTSearch(tmp->item->ParamRootPtr, searchedID)) != NULL)
                    return var;
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
    if(newData == NULL)
        return 99;
    char *newid = (char *) malloc(sizeof(char) * (strlen(id) + 1));
    if(newid == NULL){
        free(newData);
        return 99;
    }
    newData->varData = malloc(sizeof(struct varData));
    if (newData->varData == NULL) {
        free(newData);
        free(newid);
        return 99;
    }
    newData->varData->data = (int *) malloc(sizeof(int));
    if(newData->varData->data == NULL){
        free(newData->varData);
        free(newData);
        free(newid);
        return 99;
    }

    strcpy(newid, id);
    newData->ParamRootPtr = NULL;
    newData->parameterTypes = NULL;
    newData->itemID = newid;
    newData->itemType = item_type_variable;
    newData->varData->type = var_integer;
    *(int *) (newData->varData->data) = value;

    if(RootPtr->item == NULL)
        RootPtr->item = newData;
    else{
        int result = BTInsert(RootPtr, newData);
        if(result != 0){
            free(newData->varData);
            free(newData);
            free(newid);
            return result;
        }
    }
    return 0;
}


// vlozeni promenne typu double do BVS
int BTInsertVarDouble(BTNodePtr RootPtr, char *id, double value) {
    BTItemPtr *newData = (struct BTItem *) malloc(sizeof(struct BTItem));
    if(newData == NULL)
        return 99;
    char *newid = (char *) malloc(sizeof(char) * (strlen(id) + 1));
    if(newid == NULL){
        free(newData);
        return 99;
    }
    newData->varData = malloc(sizeof(struct varData));
    if (newData->varData == NULL) {
        free(newData);
        free(newid);
        return 99;
    }
    newData->varData->data = (double *) malloc(sizeof(double));
    if(newData->varData->data == NULL){
        free(newData->varData);
        free(newData);
        free(newid);
        return 99;
    }

    strcpy(newid, id);
    newData->ParamRootPtr = NULL;
    newData->parameterTypes = NULL;
    newData->itemID = newid;
    newData->itemType = item_type_variable;
    newData->varData->type = var_double;
    *(double *) (newData->varData->data) = value;

    if(RootPtr->item == NULL)
        RootPtr->item = newData;
    else{
        int result = BTInsert(RootPtr, newData);
        if(result != 0){
            free(newData->varData);
            free(newData);
            free(newid);
            return result;
        }
    }
    return 0;
}


// vlozeni promenne typu string do BVS
int BTInsertVarString(BTNodePtr RootPtr, char *id, char *value) {
    BTItemPtr *newData = (struct BTItem *) malloc(sizeof(struct BTItem));
    if(newData == NULL)
        return 99;
    char *newid = (char *) malloc(sizeof(char) * (strlen(id) + 1));
    if(newid == NULL){
        free(newData);
        return 99;
    }
    newData->varData = malloc(sizeof(struct varData));
    if (newData->varData == NULL) {
        free(newData);
        free(newid);
        return 99;
    }
    int len = strlen(value) + 1;
    newData->varData->data = (char *) malloc(sizeof(char) * len);
    if(newData->varData->data == NULL){
        free(newData->varData);
        free(newData);
        free(newid);
        return 99;
    }


    strcpy(newid, id);
    newData->ParamRootPtr = NULL;
    newData->parameterTypes = NULL;
    newData->itemID = newid;
    newData->itemType = item_type_variable;
    newData->varData->type = var_string;
    strcpy(newData->varData->data, value);

    if(RootPtr->item == NULL)
        RootPtr->item = newData;
    else{
        int result = BTInsert(RootPtr, newData);
        if(result != 0){
            free(newData->varData);
            free(newData);
            free(newid);
            return result;
        }
    }
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
    newItem->ParamRootPtr = (struct BTNode *) malloc(sizeof(struct BTNode));
    if(newItem->ParamRootPtr == NULL){
        free(newID);
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
    newItem->varData = NULL;
    newItem->parameterTypes = NULL;
    newItem->ParamRootPtr->item = NULL;
    newItem->ParamRootPtr->RPtr = NULL;
    newItem->ParamRootPtr->LPtr = NULL;

    if(RootPtr->item == NULL)
        RootPtr->item = newItem;
    else{
        int result = BTInsert(RootPtr, newItem);
        if(result != 0){
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