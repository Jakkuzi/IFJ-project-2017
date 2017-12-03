#include "semantic_check.h"
#include "parser.h"
#include "strings.h"

static BTItemPtr *actualFunction;
static BTItemPtr *actualFunction2;

int semantic_check(tCodeList *C, BTNodePtr symBTree) {
    int pom = 0, pom2 = 0, pome = 0;
    int result, i;
    int id = C->last->lineData->tokenID;
    char *name = NULL; // name of function or variable
    char *name2 = NULL;
    char *name3 = NULL;
    varDataType idReturnType;
    tLinePtr tmp = C->last->lineData; // iterator to get exact data
    tLinePtr tmp2;
    BTItemPtr *var;

    // create function in symtable
    if (id == Declare || id == Function) {
        while (tmp->tokenID != RightParenthes)
            tmp = tmp->next;
        // first next is As, second is data type, so tmp contains data type now
        tmp = tmp->next->next;
        switch (tmp->tokenID) {
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
        switch (id) {
            case Declare:
                name = C->last->lineData->next->next->token->myString;
                if (BTSearch(symBTree, name) != NULL) {
                    return 3;
                }
                result = BTInsertFunc(symBTree, idReturnType, name);
                if (result != 0)//TODO: free some shits
                    return result;
                actualFunction = BTSearch(symBTree, name);
                // second declaration or declaration of defined function
                if (actualFunction->declared != 0 && actualFunction->defined != 0)
                    return 3;
                actualFunction->declared++;
                break;
            case Function:
                name = C->last->lineData->next->token->myString;
                if ((actualFunction = BTSearch(symBTree, name)) != NULL) { // function has been declared
                    if (actualFunction->defined != 0) // redefinition
                        return 3;
                } else {
                    result = BTInsertFunc(symBTree, idReturnType, name);
                    if (result != 0)
                        return result;
                    actualFunction = BTSearch(symBTree, name);
                }
                actualFunction->defined++;
                if (actualFunction->declared) {
                    if (actualFunction->returnType != idReturnType)
                        return 3;
                }
                break;
            default:
                return 99;
        }
        // check parameters from function
        tmp = C->last->lineData;
        while (tmp->tokenID != LeftParenthes)
            tmp = tmp->next;
        i = 0;
        TString **tokenArr = NULL;
        while (tmp->tokenID != RightParenthes) {
            tmp = tmp->next; // tmp is on ID or ')' position
            if (i == 0 && tmp->tokenID == RightParenthes)
                break;
            if (id == Declare) { // declaration saves only data types
                if (actualFunction->parameterTypes == NULL) { //
                    int allocation = 0;
                    tLinePtr counter = tmp;
                    while (counter->tokenID != RightParenthes) { // get count of parameters for allocation
                        allocation++;
                        if (allocation == 1) // first token is already id
                            counter = counter->next->next->next;
                        else
                            counter = counter->next->next->next->next;
                    }
                    actualFunction->parameterTypes = (int *) malloc(sizeof(int) * allocation);
                    if (actualFunction->parameterTypes == NULL)
                        return 99;
                    tokenArr = (TString **) malloc(sizeof(TString *) * allocation);
                    if (tokenArr == NULL)
                        return 99;
                }
                actualFunction->parameterTypes[i] = tmp->next->next->tokenID;
                i++;
                actualFunction->paramCount = i;
                if (i > 1) {
                    for (int j = 0; j < i - 1; j++)
                        if (strcmp(tokenArr[j]->myString, tmp->token->myString) == 0) {
                            return 3;
                        }
                    tokenArr[i - 1] = tmp->token;
                } else
                    tokenArr[i - 1] = tmp->token;
            } else { // definition saves parameters as variables
                name = tmp->token->myString;

                if (BTSearch(symBTree, name) != NULL)
                    return 3; //parameter name is already taken

                // check parameter count
                if (actualFunction->declared && actualFunction->ParamRootPtr->item == NULL) {
                    int params = 0;
                    tLinePtr counter = tmp;
                    while (counter->tokenID != RightParenthes) { // get count of parameters for allocation
                        params++;
                        if (params == 1) // first token is already id
                            counter = counter->next->next->next;
                        else
                            counter = counter->next->next->next->next;
                    }
                    if (params != actualFunction->paramCount)
                        return 3;
                } else {
                    if (i == 0) { //
                        int allocation = 0;
                        tLinePtr counter = tmp;
                        while (counter->tokenID != RightParenthes) { // get count of parameters for allocation
                            allocation++;
                            if (allocation == 1) // first token is already id
                                counter = counter->next->next->next;
                            else
                                counter = counter->next->next->next->next;
                        }
                        actualFunction->parameterTypes = (int *) malloc(sizeof(int) * allocation);
                        if (actualFunction->parameterTypes == NULL)
                            return 99;
                    }
                    actualFunction->parameterTypes[i] = tmp->next->next->tokenID;
                    actualFunction->paramCount = i + 1;
                }

                switch (tmp->next->next->tokenID) {
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
                if (actualFunction->declared) // check if parameters from declaration have identical type
                    if (actualFunction->parameterTypes[i] != tmp->next->next->tokenID)
                        return 3;

                switch (idReturnType) {
                    case var_integer:
                        result = BTInsertVarInt(actualFunction->ParamRootPtr, name, 0);
                        if (result != 0)
                            return result;
                        break;
                    case var_string:

                        result = BTInsertVarString(actualFunction->ParamRootPtr, name, name);
                        if (result != 0)
                            return result;
                        break;
                    case var_double:
                        result = BTInsertVarDouble(actualFunction->ParamRootPtr, name, 0.0);
                        if (result != 0)
                            return result;
                        break;
                }
                i++;
            }
            tmp = tmp->next->next->next;
        }
        if (tokenArr != NULL)
            free(tokenArr);

    } else if (id == Scope) {
        result = check_func_for_definition(symBTree);
        if (result != 0)
            return result;
        result = BTInsertFunc(symBTree, var_integer, "@Scope");
        if(result != 0)
            return result;
        actualFunction = BTSearch(symBTree, "@Scope");
        if(actualFunction == NULL)
            return 99;
        actualFunction->declared++;
        actualFunction->defined++;
    } else { // insert variables from function to symBTree
        switch (id) {
            case End:
                if (C->last->lineData->next->tokenID == Function) {
                    BTDispose(actualFunction->ParamRootPtr);
                    actualFunction->ParamRootPtr = NULL;
                    free(actualFunction->ParamRootPtr);
                    actualFunction->varData = NULL;
                }
                break;
            case Else:
                break;
            case Loop:
                break;
            case Return:
                pom = 1;
                int parametr = 0, type;
                tmp = C->last->lineData->next; // return x<-

                if (tmp->tokenID == ID) {
                    if ((var = BTSearch(symBTree, tmp->token->myString)) == NULL)
                        return 3; // nedefinovana promenna
                    if (var->itemType == item_type_function)
                        pom = 0;
                    if (pom == 0) {
                        if (actualFunction->returnType == var_integer || actualFunction->returnType == var_double) {
                            if (var->returnType == var_string)
                                return 4;
                        } else if (var->returnType != var_string)
                            return 4;
                        tmp = tmp->next;
                        if (tmp == NULL || tmp->tokenID != LeftParenthes)
                            return 4;
                    }
                }

                while (tmp != NULL) {
                    switch (pom) {
                        case 0://parametry funkce
                            type = actualFunction->parameterTypes[parametr]; // typ parametru, int atd
                            switch (tmp->tokenID) {
                                case RightParenthes:
                                    break;
                                case LeftParenthes:
                                    break;
                                case valueOfDouble:
                                    if (type == String)
                                        return 4;
                                    break;
                                case valueOfDoubleWithExp:
                                    if (type == String)
                                        return 4;
                                    break;
                                case valueOfInteger:
                                    if (type == String)
                                        return 4;
                                    break;
                                case valueOfString:
                                    if (type != String)
                                        return 4;
                                    break;
                                case ID:
                                    if ((var = BTSearch(actualFunction->ParamRootPtr, tmp->token->myString)) == NULL)
                                        return 3;
                                    if (type == var_string && var->varData->type != var_string)
                                        return 4;
                                    else if (type != var_string && var->varData->type == var_string)
                                        return 4;
                                    break;
                                default:
                                    return 6;
                            }
                            parametr++;

                            break;
                        case 1://promenna
                            switch (tmp->tokenID) {
                                case ID:
                                    if ((var = BTSearch(actualFunction->ParamRootPtr, tmp->token->myString)) == NULL)
                                        return 3;
                                    if (actualFunction->returnType == var_string && var->varData->type != var_string)
                                        return 4;
                                    else if (var->varData->type == var_string)
                                        return 4;
                                    break;
                                case valueOfString:
                                    if (actualFunction->returnType != var_string)
                                        return 4;
                                    break;
                                case valueOfInteger:
                                    if (actualFunction->returnType == var_string)
                                        return 4;
                                    break;
                                case valueOfDoubleWithExp:
                                    if (actualFunction->returnType == var_string)
                                        return 4;
                                    break;
                                case valueOfDouble:
                                    if (actualFunction->returnType == var_string)
                                        return 4;
                                    break;
                                default:
                                    break;
                            }
                            break;
                    }
                    tmp = tmp->next;
                }

                break;
            case Dim:
                name = tmp->next->token->myString;
                // redefinice promenne
                if(BTSearch(symBTree, name) != NULL)
                    return 3;

                tmp = tmp->next->next->next; // datovy typ
                switch (tmp->tokenID){
                    case Integer:
                        result = BTInsertVarInt(actualFunction->ParamRootPtr, name, 0);
                        break;
                    case Double:
                        result = BTInsertVarDouble(actualFunction->ParamRootPtr, name, 0.0);
                        break;
                    case String:
                        result = BTInsertVarString(actualFunction->ParamRootPtr, name, "");
                        break;
                    default:
                        return 2;
                }
                if(result != 0)
                    return result;

                //bez prirazeni
                if(tmp->next == NULL)
                    break;

                idReturnType = getType(tmp->tokenID); // typ deklarace
                tmp = tmp->next->next; // dim id as dataType = x<--

                if(tmp->tokenID == ID && tmp->next != NULL && tmp->next->tokenID == LeftParenthes){ // prirazeni funkce
                    actualFunction2 = BTSearch(symBTree, tmp->token->myString);
                    if(actualFunction2 == NULL) //nedefinovana funkce
                        return 3;

                    //neni deklarovana ani definovana
                    if(!(actualFunction2->declared || actualFunction2->defined))
                        return 3;

                    if(strcmp(actualFunction->itemID, "@Scope") == 0)
                        if(actualFunction2->defined == 0)
                            return 3;

                    if(actualFunction2->returnType != idReturnType)
                        return 4;

                    tmp = tmp->next->next;

                    i = 0; // parametr counter
                    while(tmp->tokenID != RightParenthes){
                        switch(tmp->tokenID){
                            case valueOfString:
                                if(actualFunction2->parameterTypes[i] != String)
                                    return 4;
                                break;
                            case valueOfInteger:
                                if(actualFunction2->parameterTypes[i] != Integer)
                                    return 4;
                                break;
                            case valueOfDouble:
                            case valueOfDoubleWithExp:
                                if(actualFunction2->parameterTypes[i] != Double)
                                    return 4;
                                break;
                            case ID:
                                if((var = BTSearch(actualFunction->ParamRootPtr, tmp->token->myString)) == NULL)
                                    return 3; // promenna neni deklarovana v aktualni funkci
                                if(var->varData->type != actualFunction2->parameterTypes[i])
                                    return 4;
                                break;
                        }
                        if(tmp->tokenID != Comma)
                            i++;
                        tmp = tmp->next;
                    }
                    if(actualFunction2->paramCount != i)
                        return 4;
                }
                else{ //prirazeni promenne
                    while(tmp != NULL){
                        switch(tmp->tokenID){
                            case valueOfInteger:
                            case valueOfDouble:
                            case valueOfDoubleWithExp:
                                if(idReturnType == var_string)
                                    return 4;
                                break;
                            case valueOfString:
                                if(idReturnType != var_string)
                                    return 4;
                                break;
                            case ID:
                                var = BTSearch(actualFunction->ParamRootPtr, tmp->token->myString);
                                if(var == NULL)
                                    return 3;

                                switch(var->varData->type){ // kontrola typu
                                    case var_string:
                                        if(idReturnType != var_string)
                                            return 4;
                                        break;
                                    case var_integer:
                                    case var_double:
                                        if(idReturnType == var_string)
                                            return 4;
                                        break;
                                }
                                break;
                            default:
                                if(idReturnType == var_string && tmp->tokenID > 81 && tmp->tokenID < 95)
                                    return 6; // operace se stringem
                        }
                        tmp = tmp->next;
                    }
                }

                break;
            case Print:
                while (tmp->tokenID != Semicolon)
                    tmp = tmp->next;
                tmp = tmp->next;
                if(tmp != NULL) {
                    name = tmp->token->myString;
                    actualFunction = BTSearch(symBTree, name);
                    if (actualFunction == NULL)
                        return 3;
                    else if (actualFunction->defined != 1 || actualFunction->declared != 1)
                        return 3;
                    else if (actualFunction->varData == NULL)
                        return 3;
                }
                break;
            case Input:
                tmp = tmp->next; // input id<--
                name = tmp->token->myString;
                if((var = BTSearch(symBTree, name)) == NULL) // hleda v celem stromu identifikator
                    return 3; //promenna neni deklarovana
                if(var->itemType != item_type_variable) // pokud je identifikator funkce, je to spatne
                    return 6;
                // tohle je zbytecne, ale mas to jako priklad, ze promenna je v aktualni funkci definovana
                if(var != BTSearch(actualFunction->ParamRootPtr, name))
                    return 6;
                break;

//                tmp = tmp->next;
//                name = tmp->token->myString;
//                actualFunction = BTSearch(symBTree, name);
//                if (actualFunction == NULL)
//                    return 3;
//                else if (actualFunction->defined != 1 || actualFunction->declared != 1)
//                    return 3;
//                else if (actualFunction->varData == NULL)
//                    return 3;
//                break;
            case ID:
                name = C->last->lineData->token->myString;
                actualFunction = BTSearch(symBTree, name);
                if (actualFunction == NULL)
                    return 3;
                else if (actualFunction->declared != 1)
                    return 3;
                actualFunction->defined++;
                if(actualFunction->defined != 1)
                    return 3;
                idReturnType = actualFunction->varData->type;
                tmp = tmp->next->next;
                while (tmp != NULL) {
                    switch (tmp->tokenID) {
                        case valueOfInteger:
                            if (idReturnType == var_string)
                                return 4;
                            else if (idReturnType == var_integer || idReturnType == var_double) {
                                tmp = tmp->next;
                            }
                            break;
                        case valueOfDouble:
                            if (idReturnType == var_string)
                                return 4;
                            else if (idReturnType == var_integer) {
                                actualFunction->varData->type = var_double;
                                tmp = tmp->next;
                            }
                            else { //double
                                tmp = tmp->next;
                            }
                            break;
                        case valueOfString:
                            if (idReturnType == var_integer || idReturnType == var_double)
                                return 4;
                            else {
                                tmp = tmp->next;
                            }
                            break;
                        case ID:
                            name2 = tmp->token->myString;
                            tmp = tmp->next;
                            actualFunction = BTSearch(symBTree, name2);
                            if (actualFunction == NULL)
                                return 3;
                            else if (actualFunction->declared != 1 || actualFunction->defined != 1)
                                return 3;
                            if (tmp == NULL) {
                                switch (actualFunction->varData->type) {
                                    case var_integer:
                                        if (idReturnType == var_string)
                                            return 4;
                                        else if (idReturnType == var_integer ||
                                                 idReturnType == var_double) {
                                        }
                                        break;
                                    case var_double:
                                        if (idReturnType == var_string)
                                            return 4;
                                        else if (idReturnType == var_integer) {
                                            actualFunction->varData->type = var_double;
                                        }
                                        break;
                                    case var_string:
                                        if (idReturnType == var_integer || idReturnType == var_double)
                                            return 4;
                                        break;
                                    default:
                                        return 2;
                                }//konec switch
                            }//konec if
                            else if (tmp->tokenID == LeftParenthes) {
                                switch (actualFunction->returnType) {
                                    case var_integer:
                                        if (idReturnType == var_string)
                                            return 4;
                                        pom = 0;
                                        tmp2 = tmp->next;
                                        tmp = tmp->next->next;
                                        if (tmp->tokenID == RightParenthes) { //1 parametr
                                            name3 = tmp2->token->myString;
                                            actualFunction2 = BTSearch(symBTree, name3);
                                            if (actualFunction2 == NULL)
                                                return 3;
                                            else if (actualFunction2->declared != 1 ||
                                                     actualFunction2->defined != 1)
                                                return 3;
                                            else if (
                                                    actualFunction->parameterTypes[pom] == valueOfInteger &&
                                                    actualFunction->varData->type != var_integer ||
                                                    actualFunction->parameterTypes[pom] == valueOfDouble &&
                                                    actualFunction->varData->type != var_double ||
                                                    actualFunction->parameterTypes[pom] == valueOfString &&
                                                    actualFunction->varData->type != var_string)
                                                return 4;
                                            else
                                                tmp = tmp->next;
                                        }
                                        else if(tmp->tokenID == Comma){
                                            while (tmp->tokenID == Comma) { //2 a vice parametru
                                                name3 = tmp2->token->myString;
                                                actualFunction = BTSearch(symBTree, name3);
                                                if (actualFunction == NULL)
                                                    return 3;
                                                else if (actualFunction->declared != 1 ||
                                                         actualFunction->defined != 1)
                                                    return 3;
                                                else if (actualFunction->parameterTypes[pom] ==
                                                         valueOfInteger &&
                                                         actualFunction->varData->type != var_integer ||
                                                         actualFunction->parameterTypes[pom] ==
                                                         valueOfDouble &&
                                                         actualFunction->varData->type != var_double ||
                                                         actualFunction->parameterTypes[pom] ==
                                                         valueOfString &&
                                                         actualFunction->varData->type != var_string)
                                                    return 4;
                                                else {
                                                    tmp = tmp->next->next;
                                                    tmp2 = tmp2->next->next;
                                                    pom++;
                                                }
                                            }
                                            if (pom != (actualFunction->paramCount - 1))
                                                return 4;
                                            tmp = tmp->next;
                                        }
                                        //pro moznost bez parametru nemusime delat nic
                                        break;
                                    case var_double:
                                        if (idReturnType == var_string)
                                            return 4;
                                        else { //double, int
                                            if (idReturnType == var_integer)
                                                actualFunction->varData->type = var_double;
                                            pom = 0;
                                            tmp2 = tmp->next;
                                            tmp = tmp->next->next;
                                            if (tmp->tokenID == RightParenthes) { //1 parametr
                                                name3 = tmp2->token->myString;
                                                actualFunction2 = BTSearch(symBTree, name3);
                                                if (actualFunction2 == NULL)
                                                    return 3;
                                                else if (actualFunction2->declared != 1 ||
                                                         actualFunction2->defined != 1)
                                                    return 3;
                                                else if (
                                                        actualFunction->parameterTypes[pom] ==
                                                        valueOfInteger &&
                                                        actualFunction->varData->type != var_integer ||
                                                        actualFunction->parameterTypes[pom] ==
                                                        valueOfDouble &&
                                                        actualFunction->varData->type != var_double ||
                                                        actualFunction->parameterTypes[pom] ==
                                                        valueOfString &&
                                                        actualFunction->varData->type != var_string)
                                                    return 4;
                                                else
                                                    tmp = tmp->next;
                                            }
                                            else if (tmp->tokenID == Comma) {
                                                while (tmp->tokenID == Comma) { //2 a vice parametru
                                                    name3 = tmp2->token->myString;
                                                    actualFunction = BTSearch(symBTree, name3);
                                                    if (actualFunction == NULL)
                                                        return 3;
                                                    else if (actualFunction->declared != 1 ||
                                                             actualFunction->defined != 1)
                                                        return 3;
                                                    else if (actualFunction->parameterTypes[pom] ==
                                                             valueOfInteger &&
                                                             actualFunction->varData->type != var_integer ||
                                                             actualFunction->parameterTypes[pom] ==
                                                             valueOfDouble &&
                                                             actualFunction->varData->type != var_double ||
                                                             actualFunction->parameterTypes[pom] ==
                                                             valueOfString &&
                                                             actualFunction->varData->type != var_string)
                                                        return 4;
                                                    else {
                                                        tmp = tmp->next->next;
                                                        tmp2 = tmp2->next->next;
                                                        pom++;
                                                    }
                                                }//konec while
                                                if (pom != (actualFunction->paramCount - 1))
                                                    return 4;
                                                tmp = tmp->next;
                                            }//konec else if
                                        }//konec else
                                        break;
                                    case var_string:
                                        if (idReturnType == var_integer || idReturnType == var_double)
                                            return 4;
                                        pom = 0;
                                        tmp2 = tmp->next;
                                        tmp = tmp->next->next;
                                        if (tmp->tokenID == RightParenthes) { //1 parametr
                                            name3 = tmp2->token->myString;
                                            actualFunction2 = BTSearch(symBTree, name3);
                                            if (actualFunction2 == NULL)
                                                return 3;
                                            else if (actualFunction2->declared != 1 ||
                                                     actualFunction2->defined != 1)
                                                return 3;
                                            else if (
                                                    actualFunction->parameterTypes[pom] == valueOfInteger &&
                                                    actualFunction->varData->type != var_integer ||
                                                    actualFunction->parameterTypes[pom] == valueOfDouble &&
                                                    actualFunction->varData->type != var_double ||
                                                    actualFunction->parameterTypes[pom] == valueOfString &&
                                                    actualFunction->varData->type != var_string)
                                                return 4;
                                            else
                                                tmp = tmp->next;
                                        }
                                        else if(tmp->tokenID == Comma){
                                            while (tmp->tokenID == Comma) { //2 a vice parametru
                                                name3 = tmp2->token->myString;
                                                actualFunction = BTSearch(symBTree, name3);
                                                if (actualFunction == NULL)
                                                    return 3;
                                                else if (actualFunction->declared != 1 ||
                                                         actualFunction->defined != 1)
                                                    return 3;
                                                else if (actualFunction->parameterTypes[pom] ==
                                                         valueOfInteger &&
                                                         actualFunction->varData->type != var_integer ||
                                                         actualFunction->parameterTypes[pom] ==
                                                         valueOfDouble &&
                                                         actualFunction->varData->type != var_double ||
                                                         actualFunction->parameterTypes[pom] ==
                                                         valueOfString &&
                                                         actualFunction->varData->type != var_string)
                                                    return 4;
                                                else {
                                                    tmp = tmp->next->next;
                                                    tmp2 = tmp2->next->next;
                                                    pom++;
                                                }
                                            }
                                            if (pom != (actualFunction->paramCount - 1))
                                                return 4;
                                            tmp = tmp->next;
                                        }
                                        //pro moznost bez parametru nemusime delat nic
                                        break;
                                    default:
                                        return 2;
                                }//konec switch
                            }//konec else if
                            else{//za ID2 je znamenko
                                if(tmp->tokenID == Equal || tmp->tokenID == LowerGreater ||
                                   tmp->tokenID == Lower || tmp->tokenID == Greater ||
                                   tmp->tokenID == LowerOrEqual || tmp->tokenID == GreaterOrEqual)
                                    return 2;
                                else
                                    tmp = tmp->next;
                            }//konec else
                            break;
                        default://znamenko
                            if(tmp->tokenID == Equal || tmp->tokenID == LowerGreater ||
                                    tmp->tokenID == Lower || tmp->tokenID == Greater ||
                                    tmp->tokenID == LowerOrEqual || tmp->tokenID == GreaterOrEqual)
                                return 2;
                            else
                                tmp = tmp->next;
                            break;
                    }//konec switch
                }//konec while
                break;
            case If:
                pom = 0;
                tmp = tmp->next;
                while (tmp != NULL) {
                    if (tmp->tokenID == ID) {
                        name = tmp->token->myString;
                        actualFunction = BTSearch(symBTree, name);
                        if (actualFunction == NULL)
                            return 3;
                        else if (actualFunction->declared != 1 || actualFunction->defined != 1)
                            return 3;
                        else {
                            if (pom == 0) {
                                switch (actualFunction->varData->type) {
                                    case var_integer:
                                        pom = valueOfInteger;
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        pom = valueOfDouble;
                                        tmp = tmp->next;
                                        break;
                                    case var_string:
                                        pom = valueOfString;
                                        tmp = tmp->next;
                                        break;
                                    default:
                                        return 4;
                                }
                            }
                            else if (pom == valueOfInteger) {
                                switch (actualFunction->varData->type) {
                                    case var_integer:
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        pom = valueOfDouble;
                                        tmp = tmp->next;
                                        break;
                                    case var_string:
                                        return 4;
                                    default:
                                        return 4;
                                }
                            }
                            else if (pom == valueOfDouble) {
                                switch (actualFunction->varData->type) {
                                    case var_integer:
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        tmp = tmp->next;
                                        break;
                                    case var_string:
                                        return 4;
                                    default:
                                        return 4;
                                }
                            }
                            else if (pom == valueOfString) {
                                switch (actualFunction->varData->type) {
                                    case var_integer:
                                        return 4;
                                    case var_double:
                                        return 4;
                                    case var_string:
                                        tmp = tmp->next;
                                        break;
                                    default:
                                        return 4;
                                }
                            }
                        }//konec else
                    }//konec if (tmp->tokenID == ID)
                    else if (tmp->tokenID == valueOfInteger || tmp->tokenID == valueOfDouble) {
                        if (pom == 0) {
                            pom = tmp->tokenID;
                            tmp = tmp->next;
                        }
                        if (pom != valueOfString) {
                            tmp = tmp->next;
                        }
                        else
                            return 4;
                    }
                    else if (tmp->tokenID == valueOfString) {
                        if (pom == 0) {
                            pom = tmp->tokenID;
                            tmp = tmp->next;
                        }
                        if (pom == valueOfString) {
                            tmp = tmp->next;
                        }
                        else
                            return 4;
                    }
                    else {
                        if(tmp->tokenID == Equal || tmp->tokenID == LowerGreater ||
                           tmp->tokenID == Lower || tmp->tokenID == Greater ||
                           tmp->tokenID == LowerOrEqual || tmp->tokenID == GreaterOrEqual){
                            pome++;
                            if(pome>1)
                                return 2;
                            tmp = tmp->next;
                        }
                        else
                            tmp = tmp->next;
                    }
                }//konec while
                break;
            case Do:
                pom = 0;
                tmp = tmp->next;
                while (tmp != NULL) {
                    if (tmp->tokenID == ID) {
                        name = tmp->token->myString;
                        actualFunction = BTSearch(symBTree, name);
                        if (actualFunction == NULL)
                            return 3;
                        else if (actualFunction->declared != 1 || actualFunction->defined != 1)
                            return 3;
                        else {
                            if (pom == 0) {
                                switch (actualFunction->varData->type) {
                                    case var_integer:
                                        pom = valueOfInteger;
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        pom = valueOfDouble;
                                        tmp = tmp->next;
                                        break;
                                    case var_string:
                                        pom = valueOfString;
                                        tmp = tmp->next;
                                        break;
                                    default:
                                        return 4;
                                }
                            }
                            else if (pom == valueOfInteger) {
                                switch (actualFunction->varData->type) {
                                    case var_integer:
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        pom = valueOfDouble;
                                        tmp = tmp->next;
                                        break;
                                    case var_string:
                                        return 4;
                                    default:
                                        return 4;
                                }
                            }
                            else if (pom == valueOfDouble) {
                                switch (actualFunction->varData->type) {
                                    case var_integer:
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        tmp = tmp->next;
                                        break;
                                    case var_string:
                                        return 4;
                                    default:
                                        return 4;
                                }
                            }
                            else if (pom == valueOfString) {
                                switch (actualFunction->varData->type) {
                                    case var_integer:
                                        return 4;
                                    case var_double:
                                        return 4;
                                    case var_string:
                                        tmp = tmp->next;
                                        break;
                                    default:
                                        return 4;
                                }
                            }
                        }//konec else
                    }//konec if (tmp->tokenID == ID)
                    else if (tmp->tokenID == valueOfInteger || tmp->tokenID == valueOfDouble) {
                        if (pom == 0) {
                            pom = tmp->tokenID;
                            tmp = tmp->next;
                        }
                        if (pom != valueOfString) {
                            tmp = tmp->next;
                        }
                        else
                            return 4;
                    }
                    else if (tmp->tokenID == valueOfString) {
                        if (pom == 0) {
                            pom = tmp->tokenID;
                            tmp = tmp->next;
                        }
                        if (pom == valueOfString) {
                            tmp = tmp->next;
                        }
                        else
                            return 4;
                    }
                    else {
                        if(tmp->tokenID == Equal || tmp->tokenID == LowerGreater ||
                           tmp->tokenID == Lower || tmp->tokenID == Greater ||
                           tmp->tokenID == LowerOrEqual || tmp->tokenID == GreaterOrEqual){
                            pome++;
                            if(pome>1)
                                return 2;
                            tmp = tmp->next;
                        }
                        else
                            tmp = tmp->next;
                    }
                }//konec while
                break;
            default:
                return 2;
        }//konec hlavniho switch
    }//konec else
    return 0;
}


int addBuiltInFunctions(BTNodePtr symBTree) {
    char *f1 = (char *) malloc(sizeof(char) * 7); // lenght
    char *f2 = (char *) malloc(sizeof(char) * 7); // substr
    char *f3 = (char *) malloc(sizeof(char) * 4); // asc
    char *f4 = (char *) malloc(sizeof(char) * 4); // chr
    if (f4 == NULL || f3 == NULL || f2 == NULL || f1 == NULL) {
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    strcpy(f1, "length");
    strcpy(f2, "substr");
    strcpy(f3, "asc");
    strcpy(f4, "chr");

    int result;
    BTItemPtr *actualF = NULL;

    //f1
    result = BTInsertFunc(symBTree, var_integer, f1);
    if (result != 0) {
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    actualF->parameterTypes = (int *) malloc(sizeof(int) * 1);
    if (actualF->parameterTypes == NULL) {
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    actualF->parameterTypes[0] = String;
    actualF->paramCount = 1;
    actualF->defined = 1;
    actualF->declared = 1;

    //f2
    result = BTInsertFunc(symBTree, var_string, f2);
    if (result != 0) {
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    if (actualF == NULL) {
        multiFree(f1, f2, f3, f4);
        return 3; //TODO: asi trojka?
    }
    if (actualF->parameterTypes == NULL)
        actualF->parameterTypes = (int *) malloc(sizeof(int) * 3);

    if (actualF->parameterTypes == NULL) {
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    actualF->parameterTypes[0] = String;
    actualF->parameterTypes[1] = Integer;
    actualF->parameterTypes[2] = Integer;
    actualF->paramCount = 3;
    actualF->defined = 1;
    actualF->declared = 1;

    //f3
    result = BTInsertFunc(symBTree, var_integer, f3);
    if (result != 0) {
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    if (actualF == NULL) {
        multiFree(f1, f2, f3, f4);
        return 3; //TODO: asi trojka?
    }
    if (actualF->parameterTypes == NULL)
        actualF->parameterTypes = (int *) malloc(sizeof(int) * 2);
    if (actualF->parameterTypes == NULL) {
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    actualF->parameterTypes[0] = String;
    actualF->parameterTypes[1] = Integer;
    actualF->paramCount = 2;
    actualF->defined = 1;
    actualF->declared = 1;

    //f4
    result = BTInsertFunc(symBTree, var_string, f4);
    if (result != 0) {
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    if (actualF == NULL) {
        multiFree(f1, f2, f3, f4);
        return 3; //TODO: asi trojka?
    }
    if (actualF->parameterTypes == NULL)
        actualF->parameterTypes = (int *) malloc(sizeof(int) * 1);
    if (actualF->parameterTypes == NULL) {
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    actualF->parameterTypes[0] = Integer;
    actualF->paramCount = 1;
    actualF->defined = 1;
    actualF->declared = 1;

    multiFree(f1, f2, f3, f4);
    return 0;
}

void multiFree(char *s1, char *s2, char *s3, char *s4) {
    if (s1 != NULL)
        free(s1);
    if (s2 != NULL)
        free(s2);
    if (s3 != NULL)
        free(s3);
    if (s4 != NULL)
        free(s4);
}

int check_func_for_definition(BTNodePtr symBTree){
    if(symBTree->LPtr != NULL)
        if(check_func_for_definition(symBTree->LPtr) == 3)
            return 3;
    if(symBTree->RPtr != NULL)
        if(check_func_for_definition(symBTree->RPtr) == 3)
            return 3;
    if(symBTree->item != NULL){
        if(symBTree->item->declared > 1)
            return 3;
        if(symBTree->item->defined != 1)
            return 3;
    }
    return 0;
}

varDataType getType(int type){
    switch (type){
        case Integer:
            return var_integer;
        case Double:
            return var_double;
        case String:
            return var_string;
    }
}


/*name = C->last->lineData->next->token->myString;
                if (BTSearch(symBTree, name) != NULL) {
                    return 3;
                }
                else {
                    tmp = tmp->next->next->next;
                    switch (tmp->tokenID) {
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
                    switch (idReturnType) {
                        case var_integer:
                            result = BTInsertVarInt(symBTree, name, 0);
                            break;
                        case var_double:
                            result = BTInsertVarDouble(symBTree, name, 0.0);
                            break;
                        case var_string:
                            result = BTInsertVarString(symBTree, name, '\0');
                            break;
                        default:
                            return 99;
                    }
                    if (result != 0)
                        return result;
                    actualFunction = BTSearch(symBTree, name);
                    if (actualFunction == NULL)
                        return 3;
                    actualFunction->itemType = item_type_variable;
                    actualFunction->varData->type = idReturnType;
                    actualFunction->declared++;
                    if ((actualFunction->declared) > 1)
                        return 3;
                    tmp = tmp->next;
                    if (tmp == NULL)
                        return 0;
                    switch (tmp->tokenID) {
                        case Equal:
                            tmp = tmp->next;
                            actualFunction->defined++;
                            if ((actualFunction->defined) > 1)
                                return 3;
                            while (tmp != NULL) {
                                switch (tmp->tokenID) {
                                    case valueOfInteger:
                                        if (idReturnType == var_string)
                                            return 4;
                                        else if (idReturnType == var_integer || idReturnType == var_double) {
                                            tmp = tmp->next;
                                        }
                                        break;
                                    case valueOfDouble:
                                        if (idReturnType == var_string)
                                            return 4;
                                        else if (idReturnType == var_integer) {
                                            actualFunction->varData->type = var_double;
                                            tmp = tmp->next;
                                        }
                                        else { //double
                                            tmp = tmp->next;
                                        }
                                        break;
                                    case valueOfString:
                                        if (idReturnType == var_integer || idReturnType == var_double)
                                            return 4;
                                        else {
                                            tmp = tmp->next;
                                        }
                                        break;
                                    case ID:
                                        name2 = tmp->token->myString;
                                        tmp = tmp->next;
                                        actualFunction = BTSearch(symBTree, name2);
                                        if (actualFunction == NULL)
                                            return 3;
                                        else if (actualFunction->declared != 1 || actualFunction->defined != 1)
                                            return 3;
                                        if (tmp == NULL) {
                                            switch (actualFunction->varData->type) {
                                                case var_integer:
                                                    if (idReturnType == var_string)
                                                        return 4;
                                                    else if (idReturnType == var_integer ||
                                                             idReturnType == var_double) {
                                                    }
                                                    break;
                                                case var_double:
                                                    if (idReturnType == var_string)
                                                        return 4;
                                                    else if (idReturnType == var_integer) {
                                                        actualFunction->varData->type = var_double;
                                                    }
                                                    break;
                                                case var_string:
                                                    if (idReturnType == var_integer || idReturnType == var_double)
                                                        return 4;
                                                    break;
                                                default:
                                                    return 2;
                                            }//konec switch
                                        }//konec if
                                        else if (tmp->tokenID == LeftParenthes) {
                                            switch (actualFunction->returnType) {
                                                case var_integer:
                                                    if (idReturnType == var_string)
                                                        return 4;
                                                    pom = 0;
                                                    tmp2 = tmp->next;
                                                    tmp = tmp->next->next;
                                                    if (tmp->tokenID == RightParenthes) { //1 parametr
                                                        name3 = tmp2->token->myString;
                                                        actualFunction2 = BTSearch(symBTree, name3);
                                                        if (actualFunction2 == NULL)
                                                            return 3;
                                                        else if (actualFunction2->declared != 1 ||
                                                                 actualFunction2->defined != 1)
                                                            return 3;
                                                        else if (
                                                                actualFunction->parameterTypes[pom] == valueOfInteger &&
                                                                actualFunction->varData->type != var_integer ||
                                                                actualFunction->parameterTypes[pom] == valueOfDouble &&
                                                                actualFunction->varData->type != var_double ||
                                                                actualFunction->parameterTypes[pom] == valueOfString &&
                                                                actualFunction->varData->type != var_string)
                                                            return 4;
                                                        else
                                                            tmp = tmp->next;
                                                    }
                                                    else if(tmp->tokenID == Comma){
                                                        while (tmp->tokenID == Comma) { //2 a vice parametru
                                                            name3 = tmp2->token->myString;
                                                            actualFunction = BTSearch(symBTree, name3);
                                                            if (actualFunction == NULL)
                                                                return 3;
                                                            else if (actualFunction->declared != 1 ||
                                                                     actualFunction->defined != 1)
                                                                return 3;
                                                            else if (actualFunction->parameterTypes[pom] ==
                                                                     valueOfInteger &&
                                                                     actualFunction->varData->type != var_integer ||
                                                                     actualFunction->parameterTypes[pom] ==
                                                                     valueOfDouble &&
                                                                     actualFunction->varData->type != var_double ||
                                                                     actualFunction->parameterTypes[pom] ==
                                                                     valueOfString &&
                                                                     actualFunction->varData->type != var_string)
                                                                return 4;
                                                            else {
                                                                tmp = tmp->next->next;
                                                                tmp2 = tmp2->next->next;
                                                                pom++;
                                                            }
                                                        }
                                                        if (pom != (actualFunction->paramCount - 1))
                                                            return 4;
                                                        tmp = tmp->next;
                                                    }
                                                    //pro moznost bez parametru nemusime delat nic
                                                    break;
                                                case var_double:
                                                    if (idReturnType == var_string)
                                                        return 4;
                                                    else { //double, int
                                                        if (idReturnType == var_integer)
                                                            actualFunction->varData->type = var_double;
                                                        pom = 0;
                                                        tmp2 = tmp->next;
                                                        tmp = tmp->next->next;
                                                        if (tmp->tokenID == RightParenthes) { //1 parametr
                                                            name3 = tmp2->token->myString;
                                                            actualFunction2 = BTSearch(symBTree, name3);
                                                            if (actualFunction2 == NULL)
                                                                return 3;
                                                            else if (actualFunction2->declared != 1 ||
                                                                     actualFunction2->defined != 1)
                                                                return 3;
                                                            else if (
                                                                    actualFunction->parameterTypes[pom] ==
                                                                    valueOfInteger &&
                                                                    actualFunction->varData->type != var_integer ||
                                                                    actualFunction->parameterTypes[pom] ==
                                                                    valueOfDouble &&
                                                                    actualFunction->varData->type != var_double ||
                                                                    actualFunction->parameterTypes[pom] ==
                                                                    valueOfString &&
                                                                    actualFunction->varData->type != var_string)
                                                                return 4;
                                                            else
                                                                tmp = tmp->next;
                                                        }
                                                        else if (tmp->tokenID == Comma) {
                                                            while (tmp->tokenID == Comma) { //2 a vice parametru
                                                                name3 = tmp2->token->myString;
                                                                actualFunction = BTSearch(symBTree, name3);
                                                                if (actualFunction == NULL)
                                                                    return 3;
                                                                else if (actualFunction->declared != 1 ||
                                                                         actualFunction->defined != 1)
                                                                    return 3;
                                                                else if (actualFunction->parameterTypes[pom] ==
                                                                         valueOfInteger &&
                                                                         actualFunction->varData->type != var_integer ||
                                                                         actualFunction->parameterTypes[pom] ==
                                                                         valueOfDouble &&
                                                                         actualFunction->varData->type != var_double ||
                                                                         actualFunction->parameterTypes[pom] ==
                                                                         valueOfString &&
                                                                         actualFunction->varData->type != var_string)
                                                                    return 4;
                                                                else {
                                                                    tmp = tmp->next->next;
                                                                    tmp2 = tmp2->next->next;
                                                                    pom++;
                                                                }
                                                            }//konec while
                                                            if (pom != (actualFunction->paramCount - 1))
                                                                return 4;
                                                            tmp = tmp->next;
                                                        }//konec else if
                                                    }
                                                    break;
                                                case var_string:
                                                    if (idReturnType == var_integer || idReturnType == var_double)
                                                        return 4;
                                                    pom = 0;
                                                    tmp2 = tmp->next;
                                                    tmp = tmp->next->next;
                                                    if (tmp->tokenID == RightParenthes) { //1 parametr
                                                        name3 = tmp2->token->myString;
                                                        actualFunction2 = BTSearch(symBTree, name3);
                                                        if (actualFunction2 == NULL)
                                                            return 3;
                                                        else if (actualFunction2->declared != 1 ||
                                                                 actualFunction2->defined != 1)
                                                            return 3;
                                                        else if (
                                                                actualFunction->parameterTypes[pom] == valueOfInteger &&
                                                                actualFunction->varData->type != var_integer ||
                                                                actualFunction->parameterTypes[pom] == valueOfDouble &&
                                                                actualFunction->varData->type != var_double ||
                                                                actualFunction->parameterTypes[pom] == valueOfString &&
                                                                actualFunction->varData->type != var_string)
                                                            return 4;
                                                        else
                                                            tmp = tmp->next;
                                                    }
                                                    else if(tmp->tokenID == Comma){
                                                        while (tmp->tokenID == Comma) { //2 a vice parametru
                                                            name3 = tmp2->token->myString;
                                                            actualFunction = BTSearch(symBTree, name3);
                                                            if (actualFunction == NULL)
                                                                return 3;
                                                            else if (actualFunction->declared != 1 ||
                                                                     actualFunction->defined != 1)
                                                                return 3;
                                                            else if (actualFunction->parameterTypes[pom] ==
                                                                     valueOfInteger &&
                                                                     actualFunction->varData->type != var_integer ||
                                                                     actualFunction->parameterTypes[pom] ==
                                                                     valueOfDouble &&
                                                                     actualFunction->varData->type != var_double ||
                                                                     actualFunction->parameterTypes[pom] ==
                                                                     valueOfString &&
                                                                     actualFunction->varData->type != var_string)
                                                                return 4;
                                                            else {
                                                                tmp = tmp->next->next;
                                                                tmp2 = tmp2->next->next;
                                                                pom++;
                                                            }
                                                        }
                                                        if (pom != (actualFunction->paramCount - 1))
                                                            return 4;
                                                        tmp = tmp->next;
                                                    }
                                                    //pro moznost bez parametru nemusime delat nic
                                                    break;
                                                default:
                                                    return 2;
                                            }//konec switch
                                        }//konec else if
                                        else{//za ID2 je znamenko
                                            if(tmp->tokenID == Equal || tmp->tokenID == LowerGreater ||
                                               tmp->tokenID == Lower || tmp->tokenID == Greater ||
                                               tmp->tokenID == LowerOrEqual || tmp->tokenID == GreaterOrEqual)
                                                return 2;
                                            else
                                                tmp = tmp->next;
                                        }//konec else
                                        break;
                                    default://znamenko
                                        if(tmp->tokenID == Equal || tmp->tokenID == LowerGreater ||
                                           tmp->tokenID == Lower || tmp->tokenID == Greater ||
                                           tmp->tokenID == LowerOrEqual || tmp->tokenID == GreaterOrEqual)
                                            return 2;
                                        else
                                            tmp = tmp->next;
                                        break;
                                }//konec switch
                            }//konec while
                            break;//case Equal
                        default:
                            return 2;
                    }//konec switch
                }//konec else
                break;*/