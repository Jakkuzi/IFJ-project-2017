#include "parser.h"

int main(){
    freopen("program.txt","r",stdin);
    tCodeList sCode;

    tCodeInit(&sCode);
    int syntax = syntax_analysis(&sCode);
    if(syntax != 0){
        // TODO: uvolnit struktury
        return syntax;
    }

    printf("Syntaktická analýza OK\n");//TODO: smazat

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
    line->lineData = NULL;
    line->next = NULL;

    if(sCode->first == NULL)
    {
        sCode->first = line;
	sCode->last = sCode->first;
    }
    else{
        sCode->last->next = line;
	sCode->last = sCode->last->next;
    }
    return 0;
}

// přidá další položku na řádku kódu
void tCodeInsertToken(tCodeList *sCode, TString inToken, int id){

	TString *token = (TString *) malloc (sizeof(TString));
	// TODO: needs malloc error return value
	token->myString = (char *) malloc (sizeof(char) * strlen(inToken.myString));
	token->myString = strcpy(token->myString, inToken.myString);
	token->length = inToken.length;
	token->sizeOfAllocation = inToken.sizeOfAllocation;

    if(sCode->last->lineData == NULL){
	sCode->last->lineData = (struct tLine *) malloc (sizeof(struct tLine));
	// TODO: needs malloc error return value
        sCode->last->lineData->tokenID = id;
        sCode->last->lineData->token = token;
	sCode->last->lineData->next = NULL;
    }
    else{

        tLinePtr tmp = sCode->last->lineData;
        while(tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = (tLinePtr) malloc (sizeof(struct tLine));
	// TODO: needs malloc error return value
	tmp->next->next = NULL;
	tmp->next->tokenID = id;
	tmp->next->token = token;
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
