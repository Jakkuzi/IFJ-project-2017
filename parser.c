#include "parser.h"



int main(){
    tCodeList sCode;

    tCodeInit(&sCode);
    int syntax = syntax_analysis(&sCode);
    if(syntax == 2)
        return 2;
    if(syntax == 99)
        return 99;


    
    printf("Syntaktická analýza OK\n");

    return 0;
}

void tCodeInit(tCodeList *sCode){
    sCode->first = NULL;
    sCode->last = NULL;
}

int tCodeCreateNewLine(tCodeList *sCode){
    tCodePtr line = (struct tCode *) malloc(sizeof(struct tCode)); //new line of code
    tLinePtr data = (struct tLine *) malloc(sizeof(struct tLine)); //new place tokens in line
    if(line == NULL || data == NULL)
        return 99;
    if(sCode->first == NULL)
        sCode->first = line;
    else{
        sCode->last->next = line;
    }
    sCode->last = line;

    line->lineData = data;
    return 0;
}

void tCodeInsertToken(tCodeList *sCode, TString token){
    if(sCode->last->lineData == NULL)
        *sCode->last->lineData->token = token;
    else{
        tLinePtr tmp = sCode->last->lineData;
        while(tmp->next != NULL)
            tmp = tmp->next;
        *tmp->next->token = token;
    }
}

