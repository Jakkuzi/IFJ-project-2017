#include "parser.h"

int main(){
    freopen("test_program","r",stdin);
    tCodeList sCode;
    tCodeInit(&sCode);
    tCodeCreateNewLine(&sCode);

    int syntax = syntax_analysis(&sCode);
    if(syntax != 0){
        tCodeDispose(&sCode);
        return syntax;
    }
    tCodeDispose(&sCode);

    printf("Obe analyzy OK\n");//TODO: smazat

    return 0;
}

// inicializace kódu na prázdnou strukturu
void tCodeInit(tCodeList *sCode){
    sCode->first = NULL;
    sCode->last = NULL;
}

// přidá další řádek kódu
int tCodeCreateNewLine(tCodeList *sCode){
    tCodePtr line = (struct tCode *) malloc(sizeof(struct tCode));
    if(line == NULL)
        return 99;
    tLinePtr data = (struct tLine *) malloc(sizeof(struct tLine));
    if(data == NULL){
        free(line);
        return 99;
    }
    data->next = NULL;
    data->token = NULL;
    data->tokenID = 0;
    line->next = NULL;
    line->lineData = data;

    if(sCode->first == NULL){
        sCode->first = line;
        sCode->last = sCode->first;
    }
    else{
        sCode->last->next = line;
        sCode->last = line;
    }
    return 0;
}

// přidá další položku na řádku kódu
int tCodeInsertToken(tCodeList *sCode, TString *token, int id){
    tLinePtr tmp = sCode->last->lineData;
    while(tmp->next != NULL)
        tmp = tmp->next;
    if(tmp->token == NULL){
        tmp->token = token;
        tmp->tokenID = id;
    }
    else{
        tLinePtr data = (struct tLine *) malloc(sizeof(struct tLine));
        if(data == NULL)
            return 99;

        data->token = token;
        data->next = NULL;
        data->tokenID = id;
        tmp->next = data;
    }
    return 0;
}

// smaže kód a uvolní alokovanou paměť
void tCodeDispose(tCodeList *sCode)
{
    tCodePtr tmp;
    tLinePtr data;

    while(sCode->first != NULL){
        tmp = sCode->first;
        while(tmp->lineData != NULL){
            if(tmp->lineData->token != NULL){
                if(tmp->lineData->token->myString != NULL){
                    free(tmp->lineData->token->myString);
                    tmp->lineData->token->myString = NULL;
                }
                free(tmp->lineData->token);
                tmp->lineData->token = NULL;
            }
            data = tmp->lineData;
            tmp->lineData = tmp->lineData->next;
            free(data);
            data = NULL;

        }

        sCode->first = tmp->next;
        free(tmp);
        tmp = NULL;
    }
    sCode->last = NULL;

//    tCodeList *tmp = sCode;
//    while(tmp->first != NULL)
//	{
//		tLinePtr tmpLine = tmp->first->lineData;
//		while(tmpLine != NULL)
//		{
//			free(tmpLine->token->myString);
//			tLinePtr deleteLine = tmpLine;
//			tmpLine = tmpLine->next;
//			free(deleteLine);
//		}
//
//		tCodePtr deleteList = tmp->first;
//		tmp->first = tmp->first->next;
//		free(deleteList);
//	}
}
