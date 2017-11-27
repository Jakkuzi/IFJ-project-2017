#include "semantic_check.h"


static BTItemPtr *actualFunction;

int semantic_check(tCodeList *C, BTNodePtr symBTree){
    /* SEMANTIC CHECK
     * filling symtable from here
     *TODO: než se vloží nový řádek
     * načíst aktuální data do symtable --- z C->last
     * zkontrolovat symtable, jestli je zatím správně
     * symBTree - strom plny ID funkci
     *
     * */
    int result, i;
    int id = C->last->lineData->tokenID;
    char *name = NULL; // name of function or variable
    varDataType idReturnType;
    tLinePtr tmp = C->last->lineData; // iterator to get exact data
    // create function in symtable
    if(id == Declare || id == Function){
        while(tmp->tokenID != RightParenthes)
            tmp = tmp->next;
        // first next is As, second is data type, so tmp contains data type now
        tmp = tmp->next->next;
        switch (tmp->tokenID){
            case Double:
                idReturnType = var_double;
                break;
            case Integer:
                idReturnType = var_integer;
                break;
            case String:
                idReturnType = var_string;
                break;
            default:
                return 2;
        }
        switch (id){
            case Declare:
                name = C->last->lineData->next->next->token->myString;
                if(BTSearch(symBTree, name) != NULL){
                    return 3;//TODO: multiple declaration is what code?
                }
                result = BTInsertFunc(symBTree, idReturnType, name);
                if(result != 0)//TODO: free some shits
                    return result;
                actualFunction = BTSearch(symBTree, name);
                // second declaration or declaration of defined function
                if(actualFunction->declared != 0 && actualFunction->defined != 0)
                    return 3;
                actualFunction->declared++;
                break;
            case Function:
                name = C->last->lineData->next->token->myString;
                if((actualFunction = BTSearch(symBTree, name)) != NULL){ // function has been declared
                    if(actualFunction->defined != 0) // redefinition
                        return 3;
                }
                else{
                    result = BTInsertFunc(symBTree, idReturnType, name);
                    if(result != 0)
                        return result;
                    actualFunction = BTSearch(symBTree, name);
                }
                actualFunction->defined = 1;
                if(actualFunction->declared){
                    if(actualFunction->funcData->returnType != idReturnType)
                        return 3;
                }
                break;
        }
        // check parameters from function
        tmp = C->last->lineData;
        while(tmp->tokenID != LeftParenthes)
            tmp = tmp->next;
        i = 0;
        TString **tokenArr = NULL;
        while(tmp->tokenID != RightParenthes){
            tmp = tmp->next; // tmp is on ID or ')' position
            if(i == 0 && tmp->tokenID == RightParenthes)
                break;
            if(id == Declare){ // declaration saves only data types
                if(actualFunction->funcData->parameterTypes == NULL){ //
                    int allocation = 0;
                    tLinePtr counter = tmp;
                    while(counter->tokenID != RightParenthes){ // get count of parameters for allocation
                        allocation++;
                        if(allocation == 1) // first token is already id
                            counter = counter->next->next->next;
                        else
                            counter = counter->next->next->next->next;
                    }
                    actualFunction->funcData->parameterTypes = (int *) malloc(sizeof(int) * allocation);
                    if(actualFunction->funcData->parameterTypes == NULL)
                        return 99;
                    tokenArr = (TString **) malloc(sizeof(TString*) * allocation);
                    if(tokenArr == NULL)
                        return 99;
                }
                actualFunction->funcData->parameterTypes[i] = tmp->next->next->tokenID;
                i++;
                actualFunction->paramCount = i;
                if(i > 1){
                    for(int j = 0; j < i-1; j++)
                        if(strcmp(tokenArr[j]->myString, tmp->token->myString) == 0){
                            if(tokenArr != NULL)
                                free(tokenArr);
                            return 3;//TODO: asi 3, nwm co je to za error kdyz se 2 parametry jmenuji stejne + free
                        }
                    tokenArr[i-1] = tmp->token;
                }
                else
                    tokenArr[i-1] = tmp->token;
            }
            else{ // definition saves parameters as variables
                name = tmp->token->myString;

                // search for parameter duplication or parameter named as function
                if(BTSearch(symBTree, name) != NULL)
                    return 3; //TODO:free

                // before inserting first variable tree is not allocated
                if(actualFunction->funcData->ParamRootPtr == NULL){
                    int params = 0;
                    tLinePtr counter = tmp;
                    while(counter->tokenID != RightParenthes){ // get count of parameters for allocation
                        params++;
                        if(params == 1) // first token is already id
                            counter = counter->next->next->next;
                        else
                            counter = counter->next->next->next->next;
                    }
                    if(actualFunction->declared)
                        if(params != actualFunction->paramCount)
                            return 3;
                }

                switch (tmp->next->next->tokenID){
                    case Double:
                        idReturnType = var_double;
                        break;
                    case Integer:
                        idReturnType = var_integer;
                        break;
                    case String:
                        idReturnType = var_string;
                        break;
                    default:
                        return 2;
                }

                if(actualFunction->declared) // check if parameters from declaration are identical
                    if(actualFunction->funcData->parameterTypes[i] != tmp->next->next->tokenID)
                        return 3;

                switch (idReturnType){
                    case var_integer:
                        result = BTInsertVarInt(actualFunction->funcData, name, 0);
                        if(result != 0)
                            return result;
                        break;
                    case var_string:
                        result = BTInsertVarString(actualFunction->funcData, name, name);
                        if(result != 0)
                            return result;
                        break;
                    case var_double:
                        result = BTInsertVarDouble(actualFunction->funcData, name, 0.0);
                        if(result != 0)
                            return result;
                        break;
                }
                i++;
            }
            tmp = tmp->next->next->next;
        }
        if(tokenArr != NULL)
            free(tokenArr);

    }
    else if(id == Scope){
        //TODO: zkontrolovat definice
    }
    else{ // insert variables from function to symBTree
        if(id == End && C->last->lineData->next->tokenID == Function){
            BTDispose(actualFunction->funcData->ParamRootPtr);
            actualFunction->funcData->ParamRootPtr = NULL;
            free(actualFunction->funcData);
            actualFunction->varData = NULL;
        }

    }




    /* end of SEMANTIC CHECK */

    //TODO: smazat promenne aktualni funkce
    return 0;
}

int addBuiltInFunctions(BTNodePtr symBTree){
    char *f1 = (char *) malloc(sizeof(char) * 10); // lenght
    if(f1 == NULL)
        return 99;
    char *f2 = (char *) malloc(sizeof(char) * 10); // substr
    if(f2 == NULL){
        free(f1);
        return 99;
    }
    char *f3 = (char *) malloc(sizeof(char) * 10); // asc
    if(f3 == NULL){
        multiFree(f1, f2, NULL, NULL);
        return 99;
    }
    char *f4 = (char *) malloc(sizeof(char) * 10); // chr
    if(f4 == NULL){
        multiFree(f1, f2, f3, NULL);
        return 99;
    }
    strcpy(f1, "length");
    strcpy(f2, "substr");
    strcpy(f3, "asc");
    strcpy(f4, "chr");

    int result;
    BTItemPtr *actualF;

    //f1
    result = BTInsertFunc(symBTree, var_integer, f1);
    if(result != 0){
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    actualF->funcData->parameterTypes = (int *) malloc(sizeof(int));
    if(actualF->funcData->parameterTypes == NULL){
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    actualF->funcData->parameterTypes[0] = String;
    actualF->paramCount = 1;
    actualF->defined = 1;
    actualF->declared = 1;

    //f2
    result = BTInsertFunc(symBTree, var_string, f2);
    if(result != 0){
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    actualF->funcData->parameterTypes = (int *) malloc(sizeof(int) * 3);
    if(actualF->funcData->parameterTypes == NULL){
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    actualF->funcData->parameterTypes[0] = String;
    actualF->funcData->parameterTypes[1] = Integer;
    actualF->funcData->parameterTypes[2] = Integer;
    actualF->paramCount = 3;
    actualF->defined = 1;
    actualF->declared = 1;

    //f3
    result = BTInsertFunc(symBTree, var_integer, f3);
    if(result != 0){
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    actualF->funcData->parameterTypes = (int *) malloc(sizeof(int) * 2);
    if(actualF->funcData->parameterTypes == NULL){
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    actualF->funcData->parameterTypes[0] = String;
    actualF->funcData->parameterTypes[1] = Integer;
    actualF->paramCount = 2;
    actualF->defined = 1;
    actualF->declared = 1;

    //f4
    result = BTInsertFunc(symBTree, var_string, f4);
    if(result != 0){
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    actualF->funcData->parameterTypes = (int *) malloc(sizeof(int));
    if(actualF->funcData->parameterTypes == NULL){
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    actualF->funcData->parameterTypes[0] = Integer;
    actualF->paramCount = 1;
    actualF->defined = 1;
    actualF->declared = 1;

    return 0;
}

void multiFree(char *s1, char *s2, char *s3, char *s4){
    if(s1 != NULL)
        free(s1);
    if(s2 != NULL)
        free(s2);
    if(s3 != NULL)
        free(s3);
    if(s4 != NULL)
        free(s4);
}