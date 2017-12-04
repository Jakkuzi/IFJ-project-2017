#include "semantic_check.h"
#include "strings.h"

static BTItemPtr *actualFunction;
static BTItemPtr *actualFunction2;

int semantic_check(tCodeList *C, BTNodePtr symBTree) {
    int pom = 0, pom2 = 0;
    int result, i;
    int id = C->last->lineData->tokenID;
    char *name = NULL; // name of function or variable
    varDataType idReturnType;
    tLinePtr tmp = C->last->lineData; // iterator to get exact data
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
                        if (params == 1) { // first token is already id
                            if(actualFunction->parameterTypes[params-1] != counter->next->next->tokenID)
                                return 3;
                            counter = counter->next->next->next;
                        }
                        else{
                            if(actualFunction->parameterTypes[params-1] != counter->next->next->next->tokenID)
                                return 3;
                            counter = counter->next->next->next->next;
                        }
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
                else if(C->last->lineData->next->tokenID == Scope){
                    result = check_func_for_definition(symBTree);
                    if (result != 0)
                        return result;
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

                var = BTSearch(symBTree, name);

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
                            case valueOfDouble:
                            case valueOfDoubleWithExp:
                                if(actualFunction2->parameterTypes[i] == String)
                                    return 4;
                                break;
                            case ID:
                                if((var = BTSearch(actualFunction->ParamRootPtr, tmp->token->myString)) == NULL)
                                    return 3; // promenna neni deklarovana v aktualni funkci
                                if(var->varData->type != getType(actualFunction2->parameterTypes[i]))
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
                                if(idReturnType == var_string && tmp->tokenID != Plus)
                                    return 6; // operace nad stringem
                                if(idReturnType != var_string && (tmp->tokenID < Plus || tmp->tokenID > IntDiv))
                                    return 6; // operace nad int a double
                        }
                        tmp = tmp->next;
                    }
                }

                break;
            case Print:
                tmp = tmp->next;
                pom2 = -1; // operace s datovymi typy 1 int/double, 0 string, -1 undefined
                pom = 0; // pocet relacnich operatoru
                while(tmp != NULL){
                    switch (tmp->tokenID){
                        case ID:
                            var = BTSearch(symBTree, tmp->token->myString);
                            if(var == NULL)
                                return 3;
                            if(var->itemType == item_type_function)
                                return 6; //funexp - neni podporovano

                            if(pom2 == -1)
                                pom2 = var->varData->type == var_string ? 0 : 1;
                            else{
                                if(pom2 == 0 && var->varData->type != var_string)
                                    return 4;
                                else if(pom2 == 1 && var->varData->type == var_string)
                                    return 4;
                            }

                            break;
                        case valueOfString:
                            if(pom2 == -1)
                                pom2 = 0;
                            else if(pom2 == 1)
                                return 4;
                            break;
                        case valueOfInteger:
                        case valueOfDouble:
                        case valueOfDoubleWithExp:
                            if(pom2 == -1)
                                pom2 = 1;
                            else if(pom2 == 0)
                                return 4;
                            break;
                        case Semicolon:
                            pom2 = -1;
                            pom = 0;
                            break;
                        default:// operator
                            if(tmp->tokenID >= Equal && tmp->tokenID <= GreaterOrEqual)
                                pom++;
                    }
                    if(pom > 1)
                        return 2;
                    tmp = tmp->next;
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
            case ID:
                name = C->last->lineData->token->myString;
                if(BTSearch(symBTree, name) == NULL)
                    return 3;
                var = BTSearch(actualFunction->ParamRootPtr, name);
                if(var == NULL) //nedefinovana funkce
                    return 3;
                idReturnType = var->varData->type;
                tmp = tmp->next->next;
                if(tmp->tokenID == ID && tmp->next != NULL && tmp->next->tokenID == LeftParenthes){ // prirazeni funkce
                    actualFunction2 = BTSearch(symBTree, tmp->token->myString);
                    if(actualFunction2 == NULL) //nedefinovana funkce
                        return 3;

                    //neni deklarovana ani definovana
                    if(!(actualFunction2->declared || actualFunction2->defined))
                        return 3;

                    if(actualFunction2->returnType == var_double || actualFunction2->returnType == var_integer){
                        if(idReturnType == var_string)
                            return 4;
                    }
                    else
                        if(idReturnType != var_string)
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
                            case valueOfDouble:
                            case valueOfDoubleWithExp:
                                if(actualFunction2->parameterTypes[i] == String)
                                    return 4;
                                break;
                            case ID:
                                if((var = BTSearch(actualFunction->ParamRootPtr, tmp->token->myString)) == NULL)
                                    return 3; // promenna neni deklarovana v aktualni funkci
                                if(var->varData->type == var_double || var->varData->type == var_integer) {
                                    if (getType(actualFunction2->parameterTypes[i]) == var_string)
                                        return 4;
                                }
                                else
                                    if(getType(actualFunction2->parameterTypes[i]) != var_string)
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
                                var = BTSearch(symBTree, tmp->token->myString);
                                if(var->itemType != item_type_variable)
                                    return 4;
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
                                if(idReturnType == var_string && tmp->tokenID != Plus)
                                    return 6; // operace nad stringem
                                if(idReturnType != var_string && (tmp->tokenID < Plus || tmp->tokenID > IntDiv))
                                    return 6; // operace nad int a double
                        }
                        tmp = tmp->next;
                    }
                }

                break;
            case If:
                pom = 0; // 0 = int/double, 1 = string
                tmp = tmp->next; // if x<--
                pom2 = 0; // je to vyraz?

                switch (tmp->tokenID){
                    case valueOfInteger:
                    case valueOfDouble:
                    case valueOfDoubleWithExp:
                        pom = 0;
                        break;
                    case valueOfString:
                        pom = 1;
                        break;
                    case ID:
                        var = BTSearch(actualFunction->ParamRootPtr, tmp->token->myString);
                        if(var == NULL)
                            return 3;
                        if(var->itemType == item_type_function)
                            return 6;

                        if(var->varData->type == var_string)
                            pom = 1;
                        else
                            pom = 0;
                        break;
                }

                pom2 = 0; // count of =
                while(tmp->tokenID != Then){
                    switch(tmp->tokenID){
                        case valueOfInteger:
                        case valueOfDouble:
                        case valueOfDoubleWithExp:
                            if(pom == 1)
                                return 4;
                            break;
                        case valueOfString:
                            if(pom == 0)
                                return 4;
                            break;
                        case ID:
                            var = BTSearch(actualFunction->ParamRootPtr, tmp->token->myString);
                            if(var == NULL)
                                return 3;
                            if(var->itemType == item_type_function)
                                return 6;

                            if(var->varData->type == var_string && pom != 1)
                                return 4;
                            else if(var->varData->type != var_string && pom == 1)
                                return 4;
                            break;
                        default:
                            if(tmp->tokenID >= Equal && tmp->tokenID <= GreaterOrEqual)
                                pom2++;
                    }
                    tmp = tmp->next;
                }
                if(pom2 != 1){
//                    if(pom2 > 1)
//                        return 4;
//                    else
                        return 2;
                }
                break;
            case Do:
                pom = 0; // 0 = int/double, 1 = string
                pom2 = 0; // je to vyraz?
                tmp = tmp->next->next; // do while x<--

                switch (tmp->tokenID){
                    case valueOfInteger:
                    case valueOfDouble:
                    case valueOfDoubleWithExp:
                        pom = 0;
                        break;
                    case valueOfString:
                        pom = 1;
                        break;
                    case ID:
                        var = BTSearch(actualFunction->ParamRootPtr, tmp->token->myString);
                        if(var == NULL)
                            return 3;
                        if(var->itemType == item_type_function)
                            return 6;

                        if(var->varData->type == var_string)
                            pom = 1;
                        else
                            pom = 0;
                        break;
                }

                pom2 = 0; // count of =
                while(tmp != NULL){
                    switch(tmp->tokenID){
                        case valueOfInteger:
                        case valueOfDouble:
                        case valueOfDoubleWithExp:
                            if(pom == 1)
                                return 4;
                            break;
                        case valueOfString:
                            if(pom == 0)
                                return 4;
                            break;
                        case ID:
                            var = BTSearch(actualFunction->ParamRootPtr, tmp->token->myString);
                            if(var == NULL)
                                return 3;
                            if(var->itemType == item_type_function)
                                return 6;

                            if(var->varData->type == var_string && pom != 1)
                                return 4;
                            else if(var->varData->type != var_string && pom == 1)
                                return 4;
                            break;
                        default:
                            if(tmp->tokenID >= Equal && tmp->tokenID <= GreaterOrEqual)
                                pom2++;
                    }

                    tmp = tmp->next;
                }
                if(pom2 != 1){
//                    if(pom2 > 1)
//                        return 4;
//                    else TODO co a jak s timhle??? stejne i if
                        return 2;
                }
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

    int result;
    BTItemPtr *actualF = NULL;

    //f1
    result = BTInsertFunc(symBTree, var_integer, f1);
    if (result != 0) {
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    actualF->parameterTypes = (int *) malloc(sizeof(int));
    if (actualF->parameterTypes == NULL) {
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    actualF->parameterTypes[0] = String;
    actualF->paramCount = 1;
    actualF->defined = 1;
    actualF->declared = 1;

    //f2
    strcpy(f2, "substr");
    result = BTInsertFunc(symBTree, var_string, f2);
    if (result != 0) {
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f2);
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
    strcpy(f3, "asc");
    result = BTInsertFunc(symBTree, var_integer, f3);
    if (result != 0) {
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f3);
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
    strcpy(f4, "chr");
    result = BTInsertFunc(symBTree, var_string, f4);
    if (result != 0) {
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f4);
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
        default:// nemuze nastat
            return var_integer;
    }
}



