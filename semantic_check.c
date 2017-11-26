
#include "parser.h"
#include "strings.h"

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
                actualFunction->declared += 1;
                break;
            case Function:
                name = C->last->lineData->next->token->myString;
                if((actualFunction = BTSearch(symBTree, name)) != NULL){ // function has been declared
                    if(actualFunction->defined != 0){
                        return 3;
                    }
                    actualFunction->defined += 1;
                }
                else{ // definition is also declaration
                    result = BTInsertFunc(symBTree, idReturnType, name);
                    if(result != 0)
                        return result;
                    actualFunction = BTSearch(symBTree, name);
                    actualFunction->defined += 1;
                    if(actualFunction->declared == 0)
                        actualFunction->declared = 1;

                }
                break;
        }
        //insert parameters from function
        tmp = C->last->lineData;
        while(tmp->tokenID != LeftParenthes || tmp == NULL)
            tmp = tmp->next;
        //tmp is on ID or ')' position
        i = 0;
        TString **tokenArr = (TString **) malloc(sizeof(TString*)*5);
        //TODO: 99
        while(tmp->tokenID != RightParenthes){
            tmp = tmp->next;
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
                    for(int j = 0; j < allocation; j++)
                        tokenArr[j] = (TString *) malloc(sizeof(TString));
                    //TODO:check for 99
                }
                actualFunction->funcData->parameterTypes[i] = tmp->next->next->tokenID;
                i++;
                actualFunction->paramCount = i;
                if(i > 1){
                    for(int j = 0; j < i; j++)
                        if(strcmp(tokenArr[i]->myString, tmp->token->myString) == 0)
                            return 3;//TODO: asi 3, nwm co je to za error kdyz se 2 parametry jmenuji stejne + free
                    tokenArr[i-1] = tmp->token;
                }
                else
                    tokenArr[i-1] = tmp->token;
            }
            else{ // definition saves parameters as variables

                
//                name = tmp->token->myString;
//                switch (tmp->next->next->tokenID){
//                    case Double:
//                        idReturnType = var_double;
//                        break;
//                    case Integer:
//                        idReturnType = var_integer;
//                        break;
//                    case String:
//                        idReturnType = var_string;
//                        break;
//                    default:
//                        return 2;
//                }
//                //TODO: je potreba vkladat hodnoty???
//                switch (idReturnType){
//                    case var_integer:
//                        result = BTInsertVarInt(actualFunction->funcData, name, 0);
//                        if(result != 0)
//                            return result;
//                        break;
//                    case var_string:
//                        result = BTInsertVarString(actualFunction->funcData, name, name);
//                        if(result != 0)
//                            return result;
//                        break;
//                    case var_double:
//                        result = BTInsertVarDouble(actualFunction->funcData, name, 0.0);
//                        if(result != 0)
//                            return result;
//                        break;
//                }
//                actualFunction->paramCount += 1;//todo: takhle ne
            }
            tmp = tmp->next->next->next;
        }
        for(int j = 0; j < i; j++)
            free(tokenArr[j]);

    }
    else if(id == Scope){
        //TODO: zkontrolovat definice
    }
    else{ // insert variables from function to symBTree


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