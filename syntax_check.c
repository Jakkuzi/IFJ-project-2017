#include "syntax_check.h"

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
    {37, 	214,	205,	0,	    0,		0,		0,		0},
    {28,	0,		0,		0,		0,		0,		0,		0},},
    //BodyIF1
    {{24,   11 ,    21 ,    220,    213,    206,    0,      0},
    {31,    219,    41 ,    122,    206,    31 ,    122,    206},
    {25,    42,     218,    122,    208,    122,    206,    0},
    {11,    86,     214,    122,    206,    0,      0,      0},
    {32,    11,     122,    206,    0,      0,      0,      0},
    {36,    221,    206,    0,      0,      0,      0,      0},
    {37,    214,    206,    0,      0,      0,      0,      0},
    {27,    122,    207,    0,      0,      0,      0,      0},},
    //BodyIF2
    {{24,   11 ,    21 ,    220,    213,    207,    0,      0},
    {31,    219,    41 ,    122,    206,    31 ,    122,    207},
    {25,    42 ,    218,    122,    208,    122,    207,    0},
    {11,    86 ,    214,    122,    207,    0,      0,      0},
    {32,    11 ,    122,    207,    0,      0,      0,      0},
    {36,    221,    207,    0,      0,      0,      0,      0},
    {37,    214,    207,    0,      0,      0,      0,      0},
    {28,    0,      0,      0,      0,      0,      0,      },},
    //BodyWhile
    {{24,   11 ,    21 ,    220,    213,    208,    0,      0},
    {31,    219,    41 ,    122,    206,    31 ,    122,    208},
    {25,    42 ,    218,    122,    208,    122,    208,    0},
    {11,    86 ,    214,    122,    208,    0,      0,      0},
    {32,    11 ,    122,    208,    0,      0,      0,      0},
    {36,    221,    208,    0,      0,      0,      0,      0},
    {37,    214,    208,    0,      0,      0,      0,      0},
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
    {{11,   217,    0,      0,      0,      0,      0,      0},
    {12,    217,    0,      0,      0,      0,      0,      0},
    {13,    217,    0,      0,      0,      0,      0,      0},
    {14,    217,    0,      0,      0,      0,      0,      0},
    {15,    217,    0,      0,      0,      0,      0,      0},
    {101,	0,		0,		0,		0,		0,		0,		0},},
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
    {297,   104,    221,    0,      0,      0,      0,      0},},
};

/* syntax analysis top down */
int syntax_analysis(tCodeList *C){
    tStack *s;
    s = (tStack *) malloc(sizeof(tStack));
    if(s == NULL) {
        return 99;
    }
    BTNodePtr symBTree = (struct BTNode *) malloc(sizeof(struct BTNode));
    if(symBTree == NULL){
        free(s);
        return 99;
    }
    BTInit(symBTree);

    int i, found, top = 0, result, print_expanded = 0, skip_insert = 0; // iteration and auxiliary variables
    stackInit(s);
    int t = 0; // token id
    int first_token = 1; // check start of code

    result = addBuiltInFunctions(symBTree);
    if(result != 0){
        free(s);
        BTDispose(symBTree);
        return result;
    }

    do{
        TString *token = (TString *) malloc(sizeof(TString));
        if(token == NULL){
            free(s);
            BTDispose(symBTree);
            return 99;
        }
        result = stringInit(token);
        if(result != 0){
            freeThisCycle(token, s);
            BTDispose(symBTree);
            return result;
        }
        if(t == EndOfLine){ // last token was EOL
            t = getNextToken(token);
            if(t == 99 || t == 1){
                freeThisCycle(token, s);
                BTDispose(symBTree);
                return t;
            }
            while(t == EndOfLine){
                stringFree(token);
                result = stringInit(token);
                if(result != 0){
                    freeThisCycle(token, s);
                    BTDispose(symBTree);
                    return result;
                }
                t = getNextToken(token);
                if(t == 99 || t == 1){
                    freeThisCycle(token, s);
                    BTDispose(symBTree);
                    return t;
                }
            }

        }
        else if(t == Print || print_expanded){
            t = getNextToken(token);
            if(t == 1 || t == 99){
                freeThisCycle(token, s);
                BTDispose(symBTree);
                return t;
            }
            if(!print_expanded){
                sPop(s);
                applyRule(s, PrintP, 1);
                print_expanded = 1;
            }
            else if(t != EndOfLine){
                sPop(s);
                applyRule(s, PrintP, 1);
            }
            else
                print_expanded = 0;
        }
        else{ // read first token
            t = getNextToken(token);
            if(t == 99 || t == 1){
                freeThisCycle(token, s);
                BTDispose(symBTree);
                return t;
            }
        }

        if(first_token){ // code must start with exact commands from set of 3 words
            while(t == EndOfLine){ // ignore empty lines on start
                stringFree(token);
                result = stringInit(token);
                if(result != 0){
                    freeThisCycle(token, s);
                    BTDispose(symBTree);
                    return result;
                }
                t = getNextToken(token);
                if(t == 1 || t == 99){
                    freeThisCycle(token, s);
                    BTDispose(symBTree);
                    return t;
                }
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
                BTDispose(symBTree);
                return 2;
            }
            first_token = 0;
        }
        else{

            if(sTop(s) > 200 && (sTop(s) < 250 || sTop(s) == 297)){ // non-terminal to process, insert new rules to stack
                found = 0;
                // looking for rule based on next token
                // if print was loaded in previous step, sTop() expand will be skipped, print have another function
                for(i = 0; i < 8 && !found && !print_expanded; i++){
                    if(t == ll[sTop(s)-201][i][0])
                        found = 1;
                }

                if(found){
                    applyRule(s, sPopTop(s)-201, i-1);
                    sPop(s);
                }
                else if((top = (sTop(s))) == PrintP+201 || top == ExprIF+201 || // exception for some expressions
                        top == Expr+201 || top == IdAs+201 || top == Assignment2+201
                        || top == 297){ // 297 is special case for print
                    switch(sTop(s)){
                        case 221:
                            result = process_expr(0, C, t, token, s);//297
                            break;
                        case 219:
                            result = process_expr(0,  C, t, token, s);//296
                            break;
                        case 218:
                            result = process_expr(1, C, t, token, s);//298
                            break;
                        case 215:
                            result = process_expr(1, C, t, token, s);//298
                            break;
                        case 214:
                            result = process_expr(0, C, t, token, s);//299
                            break;
                        case 297:
                            result = process_expr(0, C, t, token, s);//special case for print
                            break;
                        default:
                            freeThisCycle(token, s);
                            BTDispose(symBTree);
                            return 2;
                    }
                    skip_insert = 1;
                    sPop(s);
                    if(result == 2 || result == 99){
                        free(s);
                        BTDispose(symBTree);
                        return result;
                    }
                    if((t = result) == sTop(s))
                        sPop(s);
                }
                else{
                    freeThisCycle(token, s);
                    BTDispose(symBTree);
                    return 2;
                }

            }
            else{ // terminal to process
                if(sTop(s) == t)
                    sPop(s);
                else{
                    freeThisCycle(token, s);
                    BTDispose(symBTree);
                    return 2;
                }
            }
        }
        if(skip_insert){ // insert is skipped only if there is expression on line processed in another func
            skip_insert = 0;
        }
        else{
            if(t == EndOfLine){
                stringFree(token);
                free(token);
                result = semantic_check(C, symBTree);
                if(result != 0){
                    free(s);
                    BTDispose(symBTree);
                    return result;
                }
                result = tCodeCreateNewLine(C);
                if(result != 0){
                    freeThisCycle(token, s);
                    BTDispose(symBTree);
                    return result;
                }
            }
            else{
                result = tCodeInsertToken(C, token, t);
                if(result != 0){
                    freeThisCycle(token, s);
                    BTDispose(symBTree);
                    return result;
                }
            }
        }
    }while(!stackEmpty(s));

    TString *token = (TString *) malloc(sizeof(TString));
    if(token == NULL){
        free(s);
        BTDispose(symBTree);
        return 99;
    }
    do{ // check if there is any command after 'end scope'
        result = stringInit(token);
        if(result != 0){
            freeThisCycle(token, s);
            BTDispose(symBTree);
            return result;
        }
        t = getNextToken(token);
        if(t == 1 || t == 99){
            freeThisCycle(token, s);
            BTDispose(symBTree);
            return t;
        }
        stringFree(token);
    }while(t == EndOfLine);
    freeThisCycle(token, s);
    BTDispose(symBTree);
    symBTree = NULL;

    if(t != EndOfFile)
        return 2;
    return 0;
}
/* END of main function */

/* function to process expression */
int process_expr(int id_processed, tCodeList *C, int t, TString *token, tStack *s){
    int print_to_process = 0;
    if(sTop(s) == 297) // Special case for print
        print_to_process = 1;
    char *prec_str = (char *) malloc(sizeof(char) * STRING_SIZE);
    if(prec_str == NULL)
        return 99;
    strcpy(prec_str, "");
    int id, allocated = 1, skip = 0, result;

    int len = 0;
    if(id_processed){ // take last token from code and add it to prec_str to check expression
        tLinePtr last = C->last->lineData;
        while(last->next != NULL)
            last = last->next;

        if((id = last->tokenID) >= ID && id <= valueOfString){ // transform number or id to 'i'
            strcat(prec_str, "i");
            len++;
        }
        else{ // everything else should be operators {+, -, ...}
            len += last->token->length;
            while(len > STRING_SIZE * allocated){
                allocated++;
                prec_str = (char *) realloc(prec_str, STRING_SIZE * allocated * sizeof(char));
                if(prec_str == NULL)
                    return 99;
            }
            strcat(prec_str, last->token->myString);
        }
    }

    // save loaded token from main
    result = tCodeInsertToken(C, token, t);
    if(result != 0){
        free(prec_str);
        return result;
    }
    if(!print_to_process && (t == EndOfLine || t == Semicolon || t == Then))
        skip = 1;
    else if(print_to_process && t == Semicolon)
        skip = 1;
    len += token->length;
    while(len > STRING_SIZE * allocated){
        allocated++;
        prec_str = (char *) realloc(prec_str, STRING_SIZE * allocated * sizeof(char));
        if(prec_str == NULL)
            return 99;
    }
    if(t >= ID && t <= valueOfString)
        strcat(prec_str, "i");
    else
        strcat(prec_str, token->myString);

    while(!skip){
        token = (TString *) malloc(sizeof(TString));
        if(token == NULL){
            free(prec_str);
            return 99;
        }
        result = stringInit(token);
        if(result != 0){
            free(token);
            free(prec_str);
            return result;
        }
        t = getNextToken(token);
        if(t == 99 || t == 1){
            free(token);
            free(prec_str);
            return t;
        }
        if(t != EndOfLine){
            result = tCodeInsertToken(C, token, t);
            if(result != 0){
                free(token);
                free(prec_str);
                return result;
            }
        }
        else{
            result = tCodeCreateNewLine(C);
            if(result != 0){
                free(token);
                free(prec_str);
                return result;
            }
        }

        if(print_to_process && t == Semicolon)
            break;
        else if(!print_to_process && (t == EndOfLine || t == Semicolon || t == Then))
            break;
        if(t == EndOfFile){
            free(token);
            free(prec_str);
            return 2;
        }
        if(t >= ID && t <= valueOfString){ // transform number or id to 'i'
            strcat(prec_str, "i");
            len++;
        }
        else{ // everything else should be operators {+, -, ...}
            len += token->length;
            while(len > STRING_SIZE * allocated){
                allocated++;
                prec_str = (char *) realloc(prec_str, STRING_SIZE * allocated * sizeof(char));
                if(prec_str == NULL){
                    free(token);
                    return 99;
                }
            }
            strcat(prec_str, token->myString);
        }
    }

    result = precedencni(prec_str);
    free(prec_str);
    //free(token);
    if(result == 2 || result == 99)
        return result;
    else
        return t;
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
    if(!stackEmpty(s))
        s->top--;
}

int sTop(tStack *s){
    return s->value[s->top];
}

int sPopTop(tStack *s){
    return s->value[s->top--];
}
