#include "syntax_check.h"
#include "strings.h"
#include "parser.h"

/* LL table converted to id of tokens */
int ll[21][8][8] = {
    //S
    {{23, 	30,		202,	122, 	201,	0,		0,		0},     // declare
    {30, 	202,	122,	205, 	30 ,	122,	201,	0},     // function
    {38, 	122,	209,	38,     0,		0,		0,		0},},   // scope
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
    {122,   0,      0,      0,      0,      0,      0,      0},
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
    {{122,  0,      0,      0,      0,      0,      0,      0},
    {297,  73,     221,    0,      0,      0,      0,      0},},
};

/* syntax analysis top down */
int syntax_analysis(tCodeList *C){
    tStack *s;
    s = (tStack *) malloc(sizeof(tStack));
    if(s == NULL)
        return 99;
    stackInit(s);
    int i, found, top = 0, result; // iteration and auxiliary variables
    int t = 0; // token id
    int first_token = 1; // check start of code

    do{
        TString *token = (TString *) malloc(sizeof(TString));
        if(token == NULL){
            free(s);
            return 99;
        }
        stringInit(token);

        if(t == 122){ // last token was EOL
            t = getNextToken(token);
            if(t == 99 || t == 1)
                return t;
            while(t == 122){
                stringFree(token);
                stringInit(token);
                t = getNextToken(token);
                if(t == 99 || t == 1)
                    return t;
            }

        }
        else{ // read first token
            t = getNextToken(token);
            if(t == 99 || t == 1)
                return t;
        }
        //TODO: RM if below, check this on another place
        if(t == 121)
            return 0;
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
                applyRule(s, S, i-1);
                sPop(s); // first token already match rule
            }
            else{
                freeThisCycle(token, s);
                return 2;
            }
            first_token = 0;
        }
        else{
            if(sTop(s) > 200 && sTop(s) < 250){ // non-terminal to process, insert new rules to stack
                found = 0;
                for(i = 0; i < 8 && !found; i++){ // looking for rule based on next token
                    if(t == ll[sTop(s)-201][i][0])
                        found = 1;
                }

                if(found){
                    applyRule(s, sPopTop(s)-201, i-1);
                    sPop(s);
                }
                else if((top = (sTop(s))) == 221 || top == 219 || // exception for some expressions
                        top == 218 || top == 215 || top == 214){
                    int result;
                    switch(sTop(s)){
                        case 221:
                            result = process_expr(0, 104, PrintP, 297, C);//297
                            break;
                        case 219:
                            result = process_expr(0, 41, ExprIF, 296, C);//296
                            break;
                        case 218:
                            result = process_expr(1, 122, Expr, 298, C);//298
                            break;
                        case 215:
                            result = process_expr(1, 122,  IdAs, 298, C);//298
                            break;
                        case 214:
                            result = process_expr(0, 122, Assignment2, 299, C);//299
                            break;
                        default:
                            return 2;
                    }
                    if(result != 0)
                        return result;
                    sPopTop(s);
                }
                else{
                    freeThisCycle(token, s);
                    return 2;
                }

            }
            else{ // terminal to process
                if(sTop(s) == t)
                    sPop(s);
                else{
                    freeThisCycle(token, s);
                    return 2;
                }
            }

        }

        //TODO: save token to struct from here
        if(t == 122){
            result = tCodeCreateNewLine(C);
            if(result != 0)
                return result;
        }
        else
            tCodeInsertToken(C, token, t);
    }while(!stackEmpty(s));


    free(s);
    return 0;
}
/* END of main function */

/* functions to process expression */

/* function to process expression */
int process_expr(int id_processed, int separator, int rule_num, int first, tCodeList *C){
    printf("neni vyreseno...\n");

    char *prec_str = (char *) malloc(sizeof(char) * STRING_SIZE);
    if(prec_str == NULL)
        return 99;

    int len = 0;
    if(id_processed){
        int id = C->last->lineData->tokenID;
        //if(id )
    }


    return 0;
}

/* END of section */

/* auxiliary functions */
void applyRule(tStack *s,int rule, int section){
    for(int i = 7; i >= 0; i--){
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
    //TODO: smazat tu blbost pro urychleni
    s->value[s->top] = 0;
    if(!stackEmpty(s))
        s->top--;
}

int sTop(tStack *s){
    return s->value[s->top];
}

int sPopTop(tStack *s){
    return s->value[s->top--];
}