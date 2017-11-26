#ifndef syntax_check_h
#define syntax_check_h


#include "parser.h"
#include "scanner.h"
#include "semantic_check.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK       10000000
#define STRING_SIZE     20

/* definitions of LL non-terminals */
#define S                    0     // 201    index in ll array
//      Head                 1     // 202    use index-201 to get
//      Parameters           2     // 203    position in array
//      ParameterNext        3     // 204
//      BodyFunction         4     // 205
//      BodyIF1              5     // 206
//      BodyIF2              6     // 207
//      BodyWhile            7     // 208
//      ScopeBody            8     // 209
//      ScopeWhile           9     // 210
//      ScopeIF1            10     // 211
//      ScopeIF2            11     // 212
//      Assignment          12     // 213
#define Assignment2         13     // 214
#define IdAs                14     // 215
//      ParameterFce        15     // 216
//      ParameterFceNext    16     // 217
#define Expr                17     // 218
#define ExprIF              18     // 219
//      DataType            19     // 220
#define PrintP              20     // 221

typedef struct {
    int value[MAX_STACK];
    int top;
}tStack;

/*
 * main function of syntax analysis
 */
int syntax_analysis(tCodeList *);
/*
 * free stack and struct with actual token
 * which has not been pushed to struct with code yet
 */
void freeThisCycle(TString *, tStack *);
/*
 * fill stack with selected rule from ll table
 * rule is filled from last number, zeros are ignored
 */
void applyRule(tStack *, int, int);



/* operations with stack */
void stackInit(tStack *);
int stackEmpty(tStack *);
void sPush(tStack *, int);
void sPop(tStack *);
int sTop(tStack *);
int sPopTop(tStack *);

/* function to process expression */
int process_expr(int, tCodeList *, int, TString *, tStack *);


#endif