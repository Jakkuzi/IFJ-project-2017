#include "semantic_check.h"


static BTItemPtr *actualFunction;
static BTItemPtr *actualFunction2;

int semantic_check(tCodeList *C, BTNodePtr symBTree){
    /* SEMANTIC CHECK
     * filling symtable from here
     *TODO: než se vloží nový řádek
     * načíst aktuální data do symtable --- z C->last
     * zkontrolovat symtable, jestli je zatím správně
     * symBTree - strom plny ID funkci
     *
     * */
    int result, i, pomint, pomint2;
    double pomd, pomd2, pomd3, pomd4;
    int id = C->last->lineData->tokenID;
    char *name = NULL; // name of function or variable
    char *name2 = NULL;
    char *name3 = NULL;
    varDataType idReturnType;
    tLinePtr tmp = C->last->lineData; // iterator to get exact data
    tLinePtr tmp2 = C->last->lineData->next->next->next->next->next->next->next;
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
                    if(actualFunction->returnType != idReturnType)
                        return 3;
                }
                break;
            default:
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
                if(actualFunction->parameterTypes == NULL){ //
                    int allocation = 0;
                    tLinePtr counter = tmp;
                    while(counter->tokenID != RightParenthes){ // get count of parameters for allocation
                        allocation++;
                        if(allocation == 1) // first token is already id
                            counter = counter->next->next->next;
                        else
                            counter = counter->next->next->next->next;
                    }
                    actualFunction->parameterTypes = (int *) malloc(sizeof(int) * allocation);
                    if(actualFunction->parameterTypes == NULL)
                        return 99;
                    tokenArr = (TString **) malloc(sizeof(TString*) * allocation);
                    if(tokenArr == NULL)
                        return 99;
                }
                actualFunction->parameterTypes[i] = tmp->next->next->tokenID;
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
                if(actualFunction->ParamRootPtr == NULL){
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
                    if(actualFunction->parameterTypes[i] != tmp->next->next->tokenID)
                        return 3;

                switch (idReturnType){
                    case var_integer:
                        result = BTInsertVarInt(actualFunction, name, 0);
                        if(result != 0)
                            return result;
                        break;
                    case var_string:
                        result = BTInsertVarString(actualFunction, name, name);
                        if(result != 0)
                            return result;
                        break;
                    case var_double:
                        result = BTInsertVarDouble(actualFunction, name, 0.0);
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
        switch(id){
            case End:
                if(C->last->lineData->next->tokenID == Function){
                    BTDispose(actualFunction->ParamRootPtr);
                    actualFunction->ParamRootPtr = NULL;
                    free(actualFunction->ParamRootPtr);
                    actualFunction->varData = NULL;
                    break;
                }
                break;
            case Else:
                break;
            case Loop:
                break;
            case Dim:
                name = C->last->lineData->next->token->myString;
                if(BTSearch(symBTree, name) != NULL){
                    return 3;
                }
                else{
                    tmp = tmp->next->next->next;
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
                    actualFunction->itemID = name;
                    actualFunction->itemType = item_type_variable;
                    actualFunction->declared++;
                    if((actualFunction->declared) > 1 || (actualFunction->defined) > 0)
                        return 3;
                    actualFunction->varData->type = idReturnType;
                    result = BTInsert(symBTree, actualFunction);
                    if(result != 0)
                        return result;
                    tmp = tmp->next;
                    switch (tmp->tokenID){
                        case EOL:
                            break;
                        case Equal:
                            tmp = tmp->next;
                            switch (tmp->tokenID){
                                case valueOfInteger:
                                    if(idReturnType == var_string)
                                        return 4;
                                    else if(idReturnType == var_integer){
                                        name2 = C->last->lineData->next->next->next->next->next->token->myString;
                                        pomint = atoi(name2);
                                        actualFunction = BTSearch(symBTree, name);
                                        BTInsertVarInt(symBTree, name, pomint);
                                        break;
                                    }
                                    else{ //double
                                        name2 = C->last->lineData->next->next->next->next->next->token->myString;
                                        sscanf(name2, "%lf", &pomd);
                                        actualFunction = BTSearch(symBTree, name);
                                        BTInsertVarDouble(symBTree, name, pomd);
                                        break;
                                    }
                                case valueOfDouble:
                                    if(idReturnType == var_string)
                                        return 4;
                                    else if(idReturnType == var_integer){
                                        actualFunction = BTSearch(symBTree, name);
                                        actualFunction->varData->type = var_double;
                                        name2 = C->last->lineData->next->next->next->next->next->token->myString;
                                        sscanf(name2, "%lf", &pomd2);
                                        BTInsertVarDouble(symBTree, name, pomd2);
                                        break;
                                    }
                                    else{ //double
                                        name2 = C->last->lineData->next->next->next->next->next->token->myString;
                                        sscanf(name2, "%lf", &pomd);
                                        BTInsertVarDouble(symBTree, name, pomd2);
                                        break;
                                    }
                                case valueOfString:
                                    if(idReturnType == var_integer || idReturnType == var_double)
                                        return 4;
                                    else{
                                        name2 = C->last->lineData->next->next->next->next->next->token->myString;
                                        BTInsertVarString(symBTree, name, name2);
                                        break;
                                    }
                                case ID:
                                    tmp = tmp->next;
                                    name2 = C->last->lineData->next->next->next->next->next->token->myString;
                                    actualFunction = BTSearch(symBTree, name2);
                                    if(actualFunction == NULL)
                                        return 3;
                                    else if(actualFunction->declared != 1 || actualFunction->defined != 1)
                                        return 3;
                                    switch(tmp->tokenID){
                                        case EOL:
                                            //TODO: udelat neco s warningy
                                            switch(actualFunction->varData->type){
                                                case var_integer:
                                                    if(idReturnType == var_string)
                                                        return 4;
                                                    else if(idReturnType == var_integer){
                                                        BTInsertVarInt(symBTree, name, actualFunction->varData->data);
                                                        break;
                                                    }
                                                    else{ //double
                                                        BTInsertVarDouble(symBTree, name, actualFunction->varData->data);
                                                        break;
                                                    }

                                                case var_double:
                                                    if(idReturnType == var_string)
                                                        return 4;
                                                    else if(idReturnType == var_integer){
                                                        BTInsertVarDouble(symBTree, name, actualFunction->varData->data);
                                                        actualFunction = BTSearch(symBTree, name);
                                                        actualFunction->varData->type = var_double;
                                                        break;
                                                    }
                                                    else{ //double
                                                        BTInsertVarDouble(symBTree, name, actualFunction->varData->data);
                                                        break;
                                                    }
                                                case var_string:
                                                    if(idReturnType == var_integer || idReturnType == var_double)
                                                        return 4;
                                                    else{
                                                        BTInsertVarString(symBTree, name, actualFunction->varData->data);
                                                        break;
                                                    }
                                                default:
                                                    return 2;
                                            }
                                        case LeftParenthes:
                                            //TODO: muze se navratovy typ taky pretypovat?
                                            switch(actualFunction->returnType){
                                                case var_integer:
                                                    if(idReturnType == var_string)
                                                        return 4;
                                                    /*else if(idReturnType == var_integer){
                                                        break;
                                                    }*/
                                                    else{ //double, int
                                                        int pom = 0;
                                                        tmp = tmp->next->next;
                                                        if(tmp->tokenID == EOL) //bez parametru
                                                            break;
                                                        else if(tmp->tokenID == RightParenthes){ //1 parametr
                                                            name3 = tmp2->token->myString;
                                                            actualFunction2 = BTSearch(symBTree, name3);
                                                            if(actualFunction2 == NULL)
                                                                return 3;
                                                            else if(actualFunction2->declared != 1 || actualFunction2->defined != 1)
                                                                return 3;
                                                            else if(actualFunction->parameterTypes[pom] != tmp2->tokenID)
                                                                return 4;
                                                            else if(actualFunction->parameterTypes[pom] == tmp2->tokenID)
                                                                break;
                                                        }

                                                        while(tmp->tokenID == Comma){ //2 a vice parametru
                                                            name3 = tmp2->token->myString;
                                                            actualFunction = BTSearch(symBTree, name3);
                                                            if(actualFunction == NULL)
                                                                return 3;
                                                            else if(actualFunction->declared != 1 || actualFunction->defined != 1)
                                                                return 3;
                                                            else{
                                                                if(actualFunction->parameterTypes[pom] == tmp2->tokenID){
                                                                    tmp = tmp->next->next;
                                                                    tmp2 = tmp2->next->next;
                                                                    pom++;
                                                                }
                                                                else
                                                                    return 4;
                                                            }
                                                        }
                                                        if(pom != actualFunction->paramCount-1)
                                                            return 4;
                                                    }

                                                case var_double:
                                                    if(idReturnType == var_string)
                                                        return 4;
                                                        /*else if(idReturnType == var_integer){
                                                            break;
                                                        }*/
                                                    else{ //double, int
                                                        if(idReturnType == var_integer)
                                                            actualFunction->varData->type = var_double;
                                                        int pom = 0;
                                                        tmp = tmp->next->next;
                                                        if(tmp->tokenID == EOL) //bez parametru
                                                            break;
                                                        else if(tmp->tokenID == RightParenthes){ //1 parametr
                                                            name3 = tmp2->token->myString;
                                                            actualFunction2 = BTSearch(symBTree, name3);
                                                            if(actualFunction2 == NULL)
                                                                return 3;
                                                            else if(actualFunction2->declared != 1 || actualFunction2->defined != 1)
                                                                return 3;
                                                            else if(actualFunction->parameterTypes[pom] != tmp2->tokenID)
                                                                return 4;
                                                            else if(actualFunction->parameterTypes[pom] == tmp2->tokenID)
                                                                break;
                                                        }

                                                        while(tmp->tokenID == Comma){ // 2 a vice parametru
                                                            name3 = tmp2->token->myString;
                                                            actualFunction = BTSearch(symBTree, name3);
                                                            if(actualFunction == NULL)
                                                                return 3;
                                                            else if(actualFunction->declared != 1 || actualFunction->defined != 1)
                                                                return 3;
                                                            else{
                                                                if(actualFunction->parameterTypes[pom] == tmp2->tokenID){
                                                                    tmp = tmp->next->next;
                                                                    tmp2 = tmp2->next->next;
                                                                    pom++;
                                                                }
                                                                else
                                                                    return 4;
                                                            }
                                                        }
                                                        if(pom != actualFunction->paramCount-1)
                                                            return 4;
                                                    }
                                                case var_string:
                                                    if(idReturnType == var_integer || idReturnType == var_double)
                                                        return 4;
                                                    else{ //string
                                                        int pom = 0;
                                                        tmp = tmp->next->next;
                                                        if(tmp->tokenID == EOL) //bez parametru
                                                            break;
                                                        else if(tmp->tokenID == RightParenthes){ //1 parametr
                                                            name3 = tmp2->token->myString;
                                                            actualFunction2 = BTSearch(symBTree, name3);
                                                            if(actualFunction2 == NULL)
                                                                return 3;
                                                            else if(actualFunction2->declared != 1 || actualFunction2->defined != 1)
                                                                return 3;
                                                            else if(actualFunction->parameterTypes[pom] != tmp2->tokenID)
                                                                return 4;
                                                            else if(actualFunction->parameterTypes[pom] == tmp2->tokenID)
                                                                break;
                                                        }

                                                        while(tmp->tokenID == Comma){ //2 a vice parametru
                                                            name3 = tmp2->token->myString;
                                                            actualFunction = BTSearch(symBTree, name3);
                                                            if(actualFunction == NULL)
                                                                return 3;
                                                            else if(actualFunction->declared != 1 || actualFunction->defined != 1)
                                                                return 3;
                                                            else{
                                                                if(actualFunction->parameterTypes[pom] == tmp2->tokenID){
                                                                    tmp = tmp->next->next;
                                                                    tmp2 = tmp2->next->next;
                                                                    pom++;
                                                                }
                                                                else
                                                                    return 4;
                                                            }
                                                        }
                                                        if(pom != actualFunction->paramCount-1)
                                                            return 4;
                                                    }
                                                default:
                                                    return 2;
                                            }
                                        default:
                                            return 2;
                                    }
                                default:
                                    return 2;

                            }
                        default:
                            return 2;

                    }

                }

            case Print:
                while(tmp->tokenID != Semicolon)
                    tmp = tmp->next;
                if(tmp->next->tokenID == EOL)
                    break;
                else{
                    tmp = tmp->next;
                    name = tmp->token->myString;
                    actualFunction = BTSearch(symBTree, name);
                    if(actualFunction == NULL)
                        return 3;
                    else if(actualFunction->defined != 1 || actualFunction->declared != 1)
                        return 3;
                    else if(actualFunction->varData == NULL)
                        return 3;
                    else
                        break;
                }
            case Input:
                tmp = tmp->next;
                name = tmp->token->myString;
                actualFunction = BTSearch(symBTree, name);
                if(actualFunction == NULL)
                    return 3;
                else if(actualFunction->defined != 1 || actualFunction->declared != 1)
                    return 3;
                else if(actualFunction->varData == NULL)
                    return 3;
                else
                    break;
            case ID:
                name = C->last->lineData->token->myString;
                actualFunction = BTSearch(symBTree, name);
                if(actualFunction == NULL)
                    return 3;
                else if(actualFunction->defined != 1 || actualFunction->declared != 1)
                    return 3;
                else if(actualFunction->varData == NULL)
                    return 3;
                idReturnType = actualFunction->varData->type;
                tmp = tmp->next->next;
                switch(tmp->tokenID){
                    case valueOfInteger:
                        if(idReturnType == var_string)
                            return 4;
                        else if(idReturnType == var_integer){
                            name2 = C->last->lineData->next->next->token->myString;
                            pomint2 = atoi(name2);
                            actualFunction = BTSearch(symBTree, name);
                            BTInsertVarInt(symBTree, name, pomint2);
                            break;
                        }
                        else{ //double
                            name2 = C->last->lineData->next->next->token->myString;
                            sscanf(name2, "%lf", &pomd3);
                            actualFunction = BTSearch(symBTree, name);
                            BTInsertVarDouble(symBTree, name, pomd3);
                            break;
                        }
                    case valueOfDouble:
                        if(idReturnType == var_string)
                            return 4;
                        else if(idReturnType == var_integer){
                            actualFunction = BTSearch(symBTree, name);
                            actualFunction->varData->type = var_double;
                            name2 = C->last->lineData->next->next->token->myString;
                            sscanf(name2, "%lf", &pomd3);
                            BTInsertVarDouble(symBTree, name, pomd3);
                            break;
                        }
                        else{ //double
                            name2 = C->last->lineData->next->next->token->myString;
                            sscanf(name2, "%lf", &pomd3);
                            BTInsertVarDouble(symBTree, name, pomd3);
                            break;
                        }
                    case valueOfString:
                        if(idReturnType == var_integer || idReturnType == var_double)
                            return 4;
                        else{
                            name2 = C->last->lineData->next->next->token->myString;
                            BTInsertVarString(symBTree, name, name2);
                            break;
                        }
                    case ID:
                        tmp = tmp->next;
                        name2 = C->last->lineData->next->next->token->myString;
                        actualFunction = BTSearch(symBTree, name2);
                        if(actualFunction == NULL)
                            return 3;
                        else if(actualFunction->declared != 1 || actualFunction->defined != 1)
                            return 3;
                        switch(tmp->tokenID) {
                            case EOL:
                                //TODO: udelat neco s warningy
                                switch (actualFunction->varData->type) {
                                    case var_integer:
                                        if (idReturnType == var_string)
                                            return 4;
                                        else if (idReturnType == var_integer) {
                                            BTInsertVarInt(symBTree, name, actualFunction->varData->data);
                                            break;
                                        } else { //double
                                            BTInsertVarDouble(symBTree, name, actualFunction->varData->data);
                                            break;
                                        }

                                    case var_double:
                                        if (idReturnType == var_string)
                                            return 4;
                                        else if (idReturnType == var_integer) {
                                            BTInsertVarDouble(symBTree, name, actualFunction->varData->data);
                                            actualFunction = BTSearch(symBTree, name);
                                            actualFunction->varData->type = var_double;
                                            break;
                                        } else { //double
                                            BTInsertVarDouble(symBTree, name, actualFunction->varData->data);
                                            break;
                                        }
                                    case var_string:
                                        if (idReturnType == var_integer || idReturnType == var_double)
                                            return 4;
                                        else {
                                            BTInsertVarString(symBTree, name, actualFunction->varData->data);
                                            break;
                                        }
                                    default:
                                        return 2;
                                }
                            case LeftParenthes:
                                //TODO: muze se navratovy typ taky pretypovat?
                                switch (actualFunction->returnType) {
                                    case var_integer:
                                        if (idReturnType == var_string)
                                            return 4;
                                            /*else if(idReturnType == var_integer){
                                                break;
                                            }*/
                                        else { //double, int
                                            int pom = 0;
                                            tmp = tmp->next->next;
                                            tmp2 = C->last->lineData->next->next->next->next;
                                            if (tmp->tokenID == EOL) //bez parametru
                                                break;
                                            else if (tmp->tokenID == RightParenthes) { //1 parametr
                                                name3 = tmp2->token->myString;
                                                actualFunction2 = BTSearch(symBTree, name3);
                                                if (actualFunction2 == NULL)
                                                    return 3;
                                                else if (actualFunction2->declared != 1 ||
                                                         actualFunction2->defined != 1)
                                                    return 3;
                                                else if (actualFunction->parameterTypes[pom] != tmp2->tokenID)
                                                    return 4;
                                                else if (actualFunction->parameterTypes[pom] == tmp2->tokenID)
                                                    break;
                                            }

                                            while (tmp->tokenID == Comma) { //2 a vice parametru
                                                name3 = tmp2->token->myString;
                                                actualFunction = BTSearch(symBTree, name3);
                                                if (actualFunction == NULL)
                                                    return 3;
                                                else if (actualFunction->declared != 1 || actualFunction->defined != 1)
                                                    return 3;
                                                else {
                                                    if (actualFunction->parameterTypes[pom] == tmp2->tokenID) {
                                                        tmp = tmp->next->next;
                                                        tmp2 = tmp2->next->next;
                                                        pom++;
                                                    } else
                                                        return 4;
                                                }
                                            }
                                            if (pom != actualFunction->paramCount - 1)
                                                return 4;
                                        }

                                    case var_double:
                                        if (idReturnType == var_string)
                                            return 4;
                                            /*else if(idReturnType == var_integer){
                                                break;
                                            }*/
                                        else { //double, int
                                            if (idReturnType == var_integer)
                                                actualFunction->varData->type = var_double;
                                            int pom = 0;
                                            tmp = tmp->next->next;
                                            tmp2 = C->last->lineData->next->next->next->next;
                                            if (tmp->tokenID == EOL) //bez parametru
                                                break;
                                            else if (tmp->tokenID == RightParenthes) { //1 parametr
                                                name3 = tmp2->token->myString;
                                                actualFunction2 = BTSearch(symBTree, name3);
                                                if (actualFunction2 == NULL)
                                                    return 3;
                                                else if (actualFunction2->declared != 1 ||
                                                         actualFunction2->defined != 1)
                                                    return 3;
                                                else if (actualFunction->parameterTypes[pom] != tmp2->tokenID)
                                                    return 4;
                                                else if (actualFunction->parameterTypes[pom] == tmp2->tokenID)
                                                    break;
                                            }

                                            while (tmp->tokenID == Comma) { // 2 a vice parametru
                                                name3 = tmp2->token->myString;
                                                actualFunction = BTSearch(symBTree, name3);
                                                if (actualFunction == NULL)
                                                    return 3;
                                                else if (actualFunction->declared != 1 || actualFunction->defined != 1)
                                                    return 3;
                                                else {
                                                    if (actualFunction->parameterTypes[pom] == tmp2->tokenID) {
                                                        tmp = tmp->next->next;
                                                        tmp2 = tmp2->next->next;
                                                        pom++;
                                                    } else
                                                        return 4;
                                                }
                                            }
                                            if (pom != actualFunction->paramCount - 1)
                                                return 4;
                                        }
                                    case var_string:
                                        if (idReturnType == var_integer || idReturnType == var_double)
                                            return 4;
                                        else { //string
                                            int pom = 0;
                                            tmp = tmp->next->next;
                                            tmp2 = C->last->lineData->next->next->next->next;
                                            if (tmp->tokenID == EOL) //bez parametru
                                                break;
                                            else if (tmp->tokenID == RightParenthes) { //1 parametr
                                                name3 = tmp2->token->myString;
                                                actualFunction2 = BTSearch(symBTree, name3);
                                                if (actualFunction2 == NULL)
                                                    return 3;
                                                else if (actualFunction2->declared != 1 ||
                                                         actualFunction2->defined != 1)
                                                    return 3;
                                                else if (actualFunction->parameterTypes[pom] != tmp2->tokenID)
                                                    return 4;
                                                else if (actualFunction->parameterTypes[pom] == tmp2->tokenID)
                                                    break;
                                            }

                                            while (tmp->tokenID == Comma) { //2 a vice parametru
                                                name3 = tmp2->token->myString;
                                                actualFunction = BTSearch(symBTree, name3);
                                                if (actualFunction == NULL)
                                                    return 3;
                                                else if (actualFunction->declared != 1 || actualFunction->defined != 1)
                                                    return 3;
                                                else {
                                                    if (actualFunction->parameterTypes[pom] == tmp2->tokenID) {
                                                        tmp = tmp->next->next;
                                                        tmp2 = tmp2->next->next;
                                                        pom++;
                                                    }
                                                    else
                                                        return 4;
                                                }
                                            }
                                            if (pom != actualFunction->paramCount - 1)
                                                return 4;
                                        }
                                    default:
                                        return 2;
                                }
                            default:
                                return 2;
                        }
                    default:
                        return 2;
                }
            case If:
                int pom = 0;
                while(tmp->tokenID != EOL){
                    if(tmp->tokenID == ID){
                        name = tmp->token->myString;
                        actualFunction = BTSearch(symBTree, name);
                        if (actualFunction == NULL)
                            return 3;
                        else if (actualFunction->declared != 1 || actualFunction->defined != 1)
                            return 3;
                        else {
                            if(pom == 0){
                                switch(actualFunction->varData->type){
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
                            else if(pom == valueOfInteger){
                                switch(actualFunction->varData->type){
                                    case var_integer:
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        pom = valueOfDouble;
                                        tmp = tmp->next;
                                        //TODO:mozna se bude pretypovavat
                                        break;
                                    case var_string:
                                        return 4;
                                    default:
                                        return 4;
                                }
                            }
                            else if(pom == valueOfDouble){
                                switch(actualFunction->varData->type){
                                    case var_integer:
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        tmp = tmp->next;
                                        //TODO:mozna se bude pretypovavat
                                        break;
                                    case var_string:
                                        return 4;
                                    default:
                                        return 4;
                                }
                            }
                            else if(pom == valueOfString){
                                switch(actualFunction->varData->type){
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
                        }
                    }
                    else if(tmp->tokenID == valueOfInteger || tmp->tokenID == valueOfDouble){
                        if(pom == 0){
                            pom = tmp->tokenID;
                            tmp = tmp->next;
                            break;
                        }
                        if(pom != valueOfString){
                            tmp = tmp->next;
                            break;
                        }
                        else
                            return 4;
                    }
                    else if(tmp->tokenID == valueOfString){
                        if(pom == 0){
                            pom = tmp->tokenID;
                            tmp = tmp->next;
                            break;
                        }
                        if(pom == valueOfString){
                            tmp = tmp->next;
                            break;
                        }
                        else
                            return 4;
                    }
                }
            case Do:
                int pom2 = 0;
                while(tmp->tokenID != EOL){
                    if(tmp->tokenID == ID){
                        name = tmp->token->myString;
                        actualFunction = BTSearch(symBTree, name);
                        if (actualFunction == NULL)
                            return 3;
                        else if (actualFunction->declared != 1 || actualFunction->defined != 1)
                            return 3;
                        else {
                            if(pom2 == 0){
                                switch(actualFunction->varData->type){
                                    case var_integer:
                                        pom2 = valueOfInteger;
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        pom2 = valueOfDouble;
                                        tmp = tmp->next;
                                        break;
                                    case var_string:
                                        pom2 = valueOfString;
                                        tmp = tmp->next;
                                        break;
                                    default:
                                        return 4;
                                }
                            }
                            else if(pom2 == valueOfInteger){
                                switch(actualFunction->varData->type){
                                    case var_integer:
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        pom2 = valueOfDouble;
                                        tmp = tmp->next;
                                        //TODO:mozna se bude pretypovavat
                                        break;
                                    case var_string:
                                        return 4;
                                    default:
                                        return 4;
                                }
                            }
                            else if(pom2 == valueOfDouble){
                                switch(actualFunction->varData->type){
                                    case var_integer:
                                        tmp = tmp->next;
                                        break;
                                    case var_double:
                                        tmp = tmp->next;
                                        //TODO:mozna se bude pretypovavat
                                        break;
                                    case var_string:
                                        return 4;
                                    default:
                                        return 4;
                                }
                            }
                            else if(pom2 == valueOfString){
                                switch(actualFunction->varData->type){
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
                        }
                    }
                    else if(tmp->tokenID == valueOfInteger || tmp->tokenID == valueOfDouble){
                        if(pom2 == 0){
                            pom2 = tmp->tokenID;
                            tmp = tmp->next;
                            break;
                        }
                        if(pom2 != valueOfString){
                            tmp = tmp->next;
                            break;
                        }
                        else
                            return 4;
                    }
                    else if(tmp->tokenID == valueOfString){
                        if(pom2 == 0){
                            pom2 = tmp->tokenID;
                            tmp = tmp->next;
                            break;
                        }
                        if(pom2 == valueOfString){
                            tmp = tmp->next;
                            break;
                        }
                        else
                            return 4;
                    }
                }
            default:
                return 2;
        }
    }

    /* symtable filled */

    //TODO: zkontroluj kazdy radek, jestli je zatim spravne
    // symTree je ukazatel na root
    // actualFunction je funkce, ve ktere preva pracujeme
    // search je vhodne aplikovat prvne na funkci pak prekontrolovat,
    // jeslti se promenna nejmenuje jako funkce
    // GL :-)


    /* end of SEMANTIC CHECK */

    //TODO: smazat promenne aktualni funkce
    return 0;
}

int addBuiltInFunctions(BTNodePtr symBTree){
    char *f1 = (char *) malloc(sizeof(char) * 7); // lenght
    char *f2 = (char *) malloc(sizeof(char) * 7); // substr
    char *f3 = (char *) malloc(sizeof(char) * 4); // asc
    char *f4 = (char *) malloc(sizeof(char) * 4); // chr
    if(f4 == NULL || f3 == NULL || f2 == NULL || f1 == NULL){
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
    if(result != 0){
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    actualF->parameterTypes = (int *) malloc(sizeof(int) * 1);
    if(actualF->parameterTypes == NULL){
        multiFree(f1, f2, f3, f4);
        return 99;
    }
    actualF->parameterTypes[0] = String;
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
    if(actualF == NULL){
        multiFree(f1, f2, f3, f4);
        return 3; //TODO: asi trojka?
    }
    if(actualF->parameterTypes == NULL)
        actualF->parameterTypes = (int *) malloc(sizeof(int) * 3);
    if(actualF->parameterTypes == NULL){
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
    if(result != 0){
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    if(actualF == NULL){
        multiFree(f1, f2, f3, f4);
        return 3; //TODO: asi trojka?
    }
    if(actualF->parameterTypes == NULL)
        actualF->parameterTypes = (int *) malloc(sizeof(int) * 2);
    if(actualF->parameterTypes == NULL){
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
    if(result != 0){
        multiFree(f1, f2, f3, f4);
        return result;
    }
    actualF = BTSearch(symBTree, f1);
    if(actualF == NULL){
        multiFree(f1, f2, f3, f4);
        return 3; //TODO: asi trojka?
    }
    if(actualF->parameterTypes == NULL)
        actualF->parameterTypes = (int *) malloc(sizeof(int)*1);
    if(actualF->parameterTypes == NULL){
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