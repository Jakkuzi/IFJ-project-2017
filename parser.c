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

// inicializace kódu na prázdnou strukturu
void tCodeInit(tCodeList *sCode){
    sCode->first = NULL;
    sCode->last = NULL;
}

// přidá další řádek kódu
int tCodeCreateNewLine(tCodeList *sCode){
    tCodePtr line = (struct tCode *) malloc(sizeof(struct tCode));
    tLinePtr data = (struct tLine *) malloc(sizeof(struct tLine));
    if(line == NULL)
        return 99;
    if(data == NULL){
        free(line);
        return 99;
    }
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
        tmp->next = data;

    }

}

// smaže kód a uvolní alokovanou paměť
void tCodeDispose(tCodeList *sCode)
{
	tCodeList *tmp = sCode;
	while(tmp->first != NULL)
	{
		tLinePtr tmpLine = tmp->first->lineData;
		while(tmpLine != NULL)
		{
			free(tmpLine->token->myString);
			free(tmpLine->token);
			tLinePtr deleteLine = tmpLine;
			tmpLine = tmpLine->next;
			free(deleteLine);
		}

		tCodePtr deleteList = tmp->first;
		tmp->first = tmp->first->next;
		free(deleteList);
	}
}
