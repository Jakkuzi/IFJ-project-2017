#include "syntax_check.h"

/* LL table converted to id of tokens */
int ll[21][8][8] = {
    //S
    {{23, 	30,		202,	122, 	201,	0,		0,		0},
    {30, 	202,	122,	205, 	30 ,	122,	201,	0},
    {38, 	122,	209,	38, 	0,		0,		0,		0},},
    //Head
    {{11, 	100,	203,	21,		220, 	0,		0,		0},},
    //Parameters
    {{11, 	21,		220,	204,	0,		0,		0,		0},
    {101,	0,		0,		0,		0,		0,		0,		0},},
    //ParameterNext
    {{105,	203,	0,		0,		0,		0,		0,		0},
    {101,	0,		0,		0,		0,		0,		0,		0},},
    //BodyFunction
    {{24, 	11,		21,		220, 	213, 	205,	0,		0},
    {31, 	219,	41,		122, 	206, 	31 ,	122,	205},
    {25, 	42,		218,	122, 	208, 	122,	205,	0},
    {11, 	86,		214,	122, 	205,	0,		0,		0},
    {32, 	11,		122,	205,	0,		0,		0,		0},
    {36, 	221,	205,	0,		0,		0,		0,		0},
    {37, 	214,	122,	205,	0,		0,		0,		0},
    {28,	0,		0,		0,		0,		0,		0,		0},},
    //BodyIF1
    {{24,   11 ,    21 ,    220,    213,    206,    0,      0},
    {31,    219,    41 ,    122,    206,    31 ,    122,    206},
    {25,    42,     218,    122,    208,    122,    206,    0},
    {11,    86,     214,    122,    206,    0,      0,      0},
    {32,    11,     122,    206,    0,      0,      0,      0},
    {36,    221,    206,    0,      0,      0,      0,      0},
    {37,    214,    122,    206,    0,      0,      0,      0},
    {27,    122,    206,    0,      0,      0,      0,      0},},
    //BodyIF2
    {{24,   11 ,    21 ,    220,    213,    207,    0,      0},
    {31,    219,    41 ,    122,    206,    31 ,    122,    207},
    {25,    42 ,    218,    122,    208,    122,    207,    0},
    {11,    86 ,    214,    122,    207,    0,      0,      0},
    {32,    11 ,    122,    207,    0,      0,      0,      0},
    {36,    221,    207,    0,      0,      0,      0,      0},
    {37,    214,    122,    207,    0,      0,      0,      0},
    {28,    0,      0,      0,      0,      0,      0,      },},
    //BodyWhile
    {{24,   11 ,    21 ,    220,    213,    208,    0,      0},
    {31,    219,    41 ,    122,    206,    31 ,    122,    208},
    {25,    42 ,    218,    122,    208,    122,    208,    0},
    {11,    86 ,    214,    122,    208,    0,      0,      0},
    {32,    11 ,    122,    208,    0,      0,      0,      0},
    {36,    221,    208,    0,      0,      0,      0,      0},
    {37,    214,    122,    208,    0,      0,      0,      0},
    {35,    0,      0,      0,      0,      0,      0,      0},},
    //ScopeBody
    {{24,   11 ,    21 ,    220,    213,    209,    0,      0},
    {31,    219,    41 ,    122,    211,    31 ,    122,    209},
    {25,    42 ,    218,    122,    210,    122,    209,    0},
    {11,    86 ,    214,    122,    209,    0,      0,      0},
    {32,    11 ,    122,    209,    0,      0,      0,      0},
    {36,    221,    209,    0,      0,      0,      0,      0},
    {28,    0,      0,      0,      0,      0,      0,      0},},
    //ScopeWhile
    {{24,   11 ,    21 ,    220,    213,    210,    0,      0},
    {31,    219,    41 ,    122,    211,    31 ,    122,    210},
    {25,    42 ,    218,    122,    210,    122,    210,    0},
    {11,    86 ,    214,    122,    210,    0,      0,      0},
    {32,    11 ,    122,    210,    0,      0,      0,      0},
    {36,    221,    210,    0,      0,      0,      0,      0},
    {35,    0,      0,      0,      0,      0,      0,      0},},
    //ScopeIF1
    {{24,   11 ,    21 ,    220,    213,    211,    0,      0},
    {31,    219,    41 ,    122,    211,    31 ,    122,    211},
    {25,    42 ,    218,    122,    210,    122,    211,    0},
    {11,    86 ,    214,    122,    211,    0,      0,      0},
    {32,    11 ,    122,    211,    0,      0,      0,      0},
    {36,    221,    211,    0,      0,      0,      0,      0},
    {27,    122,    212,    0,      0,      0,      0,      0},},
    //ScopeIF2
    {{24,   11 ,    21 ,    220,    213,    212,    0,      0},
    {31,    219,    41 ,    122,    211,    31 ,    122,    212},
    {25,    42 ,    218,    122,    210,    122,    212,    0},
    {11,    86 ,    214,    122,    212,    0,      0,      0},
    {32,    11 ,    122,    212,    0,      0,      0,      0},
    {36,    221,    212,    0,      0,      0,      0,      0},
    {28,    0,      0,      0,      0,      0,      0,      0},},
    //Assignment
    {{86,   214,    122,    0,      0,      0,      0,      0},
    {122,   0,      0,      0,      0,      0,      0,      0},},
    //Assignment2
    {{11,   215,    0,      0,      0,      0,      0,      0},
    {299,   0,      0,      0,      0,      0,      0,      0},},
    //IdAs
    {{100,  216,    0,      0,      0,      0,      0,      0},
    {298,   0,      0,      0,      0,      0,      0,      0},},
    //ParameterFce
    {{11,   217,    0,      0,      0,      0,      0,      0},},
    //ParameterFceNext
    {{105,  216,    0,      0,      0,      0,      0,      0},
    {101,   0,      0,      0,      0,      0,      0,      0},},
    //Expr
    {{11,   215,    0,      0,      0,      0,      0,      0},
    {298,   0,      0,      0,      0,      0,      0,      0},},
    //ExprIF
    {{11,   215,    0,      0,      0,      0,      0,      0},
    {296,   0,      0,      0,      0,      0,      0,      0},},
    //DataType
    {{33,   0,      0,      0,      0,      0,      0,      0},
    {39,    0,      0,      0,      0,      0,      0,      0},
    {26,    0,      0,      0,      0,      0,      0,      0},},
    //PrintP
    {{297,  73,     221,    0,      0,      0,      0,      0},
    {122,   0,      0,      0,      0,      0,      0,      0},},
};

/* syntax analysis top down */
int syntax_analysis(tCodeList *C){
    C->first = NULL;
    tStack *s;
    s = (tStack *) malloc(sizeof(tStack));
    if(s == NULL)
        return 99;
    stackInit(s);
    int i, found; // iteration and auxiliary variables
    int t; // token id
    int first_token = 1; // check start of code
    int eol_loaded = 0;

    do{
        TString *token = malloc(sizeof(TString));
        if(token == NULL)
            return 99;
        stringInit(token);
        t = getNextToken(token);
        if(t == 122)
            eol_loaded = 1;
        if(eol_loaded && sTop(s) != 122){ // remove all empty lines unless they are on stack
            stringFree(token);
            stringInit(token);
            t = getNextToken(token);
            eol_loaded = 0;
        }
        if(first_token){ // code must start with exact commands from set of 3 words
            while(t == 122){ // ignore empty lines on start
                stringFree(token);
                stringInit(token);
                t = getNextToken(token);
            }

            found = 0;
            for(i = 0; i < 3 && !found; i++){ // S state have only 3 rules to go to
                if(t == ll[S][i][0])
                    found = 1;
            }

            if(found){
                applyRule(s, S, i);
                sPop(s); // first token already match rule
            }
            else{
                freeThisCycle(token, s);
                return 2;
            }
            first_token = 0;
            eol_loaded = 0;
        }
        else{
            if(sTop(s) > 201 && sTop(s) < 250){ // non-terminal to process
                found = 0;
                for(i = 0; i < 8 && !found; i++){
                    if(t == ll[sTop(s)-201][i][0])
                        found = 1;
                }

                if(found)
                    applyRule(s, sPopTop(s)-201, i);
                else{
                    freeThisCycle(token, s);
                    return 2;
                }
            } // insert new rules to stack

            //TODO: pošéfovat id

            //TODO: tady bude asi switch??


        }

    //TODO: save token to struct from here
    }while(!stackEmpty(s));


    free(s);
    return 0;
}
/* END of main function */

/* auxiliary functions */
void applyRule(tStack *s,int rule, int section){
    for(int i = 7; i <= 0; i--){
        if(ll[rule][section][i] == 0)
            continue;
        sPush(s, ll[rule][section][i]);
    }
}

void freeThisCycle(TString *t, tStack *s){
    free(t);
    free(s);
}

void stackInit(tStack *s){
    s->top = -1;
}

int stackEmpty(tStack *s){
    return (s->top == -1) ? 1 : 0;
}

void sPush(tStack *s, int num){
    s->value[++s->top] = num;
}

void sPop(tStack *s){
    if(!stackEmpty(s))
        s->top--;
}

int sTop(tStack *s){
    return s->value[s->top];
}

int sPopTop(tStack *s){
    return s->value[s->top--];
}