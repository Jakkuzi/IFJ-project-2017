#include "parser.h"

int main(){
    freopen("program.txt","r",stdin);
    tCodeList sCode;
    tCodeInit(&sCode);
    tCodeCreateNewLine(&sCode);

    int syntax = syntax_analysis(&sCode);
    if(syntax != 0){
        // TODO: uvolnit struktury
        return syntax;
    }

    printf("Syntakticka analyza OK\n");//TODO: smazat

    return 0;
}

void tCodeInit(tCodeList *sCode){
    sCode->first = NULL;
    sCode->last = NULL;
}

int tCodeCreateNewLine(tCodeList *sCode){
    tCodePtr line = (struct tCode *) malloc(sizeof(struct tCode));
    tLinePtr data = (struct tLine *) malloc(sizeof(struct tLine));
    if(line == NULL || data == NULL)
        return 99;
    data->next = NULL;
    data->token = NULL;
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
        tmp->next = data;
    }

}

