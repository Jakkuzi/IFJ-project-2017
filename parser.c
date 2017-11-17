#include "parser.h"

int parser(){
    tCodePtr sCode;

    tCodeInit(&sCode);

    if(syntax_analysis(&sCode) != 0)
        return 2;

}

void tCodeInit(tCodePtr *sCode){
    sCode = NULL;
}

void tCodeInsert(tCodePtr *sCode){

}

void tCodeDispose(tCodePtr *sCode){

}
