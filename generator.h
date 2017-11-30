#ifndef GENERATOR_H
#define GENERATOR_H

#include "scanner.h"
#include "parser.c"
#include "strings.c"
#include "symtable.c"

// dodatecne operace pro jazyk IFJcode17 (implicitni konverze datovych typu)
#define Int2Float 200
#define Float2Int 201
#define Float2Int2Float 202 // pro IntDiv je potreba nejdrive oseknout, pote prevest na float pro deleni a zpet na int

// sLabel beginning

// navesti LABELIF
// Globalni pocitadlo pro navesti IF
unsigned int labelIf = 0;

// navesti LABELLOOP
// Globalni pocitadlo pro navesti LOOP
unsigned int labelLoop = 0;

#define sLabelMax 10000
// pomocny zasobnik pro navesti
typedef struct sLabel{
	int array[sLabelMax];
	int top;
} *sLabelPtr;

void sLabelInit(sLabelPtr stack)
{
        stack->top = -1;
}
void sLabelPush(sLabelPtr stack, int newLabel)
{
        stack->top += 1;
        stack->array[stack->top] = newLabel;
}
int sLabelTop(sLabelPtr stack)
{
        return stack->array[stack->top];
}
void sLabelPop(sLabelPtr stack)
{
	stack->top -= 1;
}

// sLabel end


// list beginning
// seznam znazornujici vyraz v postfixu (aby se s nim dalo dale pracovat a nebyl pouze vypsan)

typedef struct gList{
	int isOp;	// 1 = je oprator
	int isConst;	// 1 = je konstanta
	int tokenID;
	char *data;
	struct gList *next;
} *gListPtr;

void gListInit(gListPtr list)
{
	list->data = NULL;
	list->next = NULL;
}

void gListAdd(gListPtr *list, tLinePtr val)
{
	gListPtr newL = (gListPtr) malloc (sizeof(struct gList));
	// TODO: kontrola malloc

	if(val->tokenID == ID)
	{
		newL->isOp = 0;
		newL->isConst = 0;
	}
	else if(val->tokenID == valueOfInteger || val->tokenID == valueOfDouble ||
	val->tokenID == valueOfDoubleWithExp || val->tokenID == valueOfString)
	{
		newL->isOp = 0;
                newL->isConst = 1;
	}
	else
	{
		newL->isOp = 1;
                newL->isConst = 0;
	}
	newL->data = val->token->myString;
	newL->tokenID = val->tokenID;
	newL->next = NULL;
	if((*list)->data == NULL)
		*list = newL;
	else
	{
		gListPtr temp = *list;
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = newL;
	}
}


void gListPrint(gListPtr list)
{
	while(list != NULL)
	{
		if(list->tokenID != ID && list->tokenID != valueOfInteger && list->tokenID != valueOfDouble &&
		list->tokenID != valueOfDoubleWithExp && list->tokenID != valueOfString)
		{
			switch(list->tokenID)
			{
				case Plus: printf("+"); break;
				case Minus: printf("-"); break;
        	                case Div: printf("/"); break;
	                        case Mul: printf("*"); break;
                        	case IntDiv: printf("\\"); break;
                	        case Equal: printf("="); break;
        	                case LowerGreater: printf("<>"); break;
	                     	case Lower: printf("<"); break;
                        	case LowerOrEqual: printf("<="); break;
       				case Greater: printf(">"); break;
				case GreaterOrEqual: printf(">="); break;
				case Int2Float: printf("I2F"); break;
				case Float2Int: printf("F2I"); break;
				case Float2Int2Float: printf("F2I2F"); break;
				default: fprintf(stderr, "neznamy token[%d]", list->tokenID); break;
			}
			printf(" ");
		}
		else
		{
			printf("%s ", list->data);
		}
		list = list->next;
	}
}

// list end





// stack beginning
// Zasobnik pro postfixovou notaci (vyhodnoceni vyrazu)
#define gStackMax 100

typedef struct gOperator{
	int id;
	int priority;
} gOperator;

typedef struct gStack{
	gOperator array[gStackMax];
	int top;
} *gStackPtr;

int gStackPriority(int op)
{
        if(op == Mul || op == Div)
                return 1;
        else if(op == IntDiv)
                return 2;
        else if(op == Plus || op == Minus)
                return 3;
        else
                return 4;
}

void gStackInit(gStackPtr stack)
{
	stack->top = -1;
}
void gStackPush(gStackPtr stack, int op)
{
	stack->top += 1;
	stack->array[stack->top].id = op;
	stack->array[stack->top].priority = gStackPriority(op);
}
int gStackPop(gStackPtr stack)
{
	int temp = stack->array[stack->top].id;
	stack->top -= 1;
	return temp;
}

// vytvori strukturu tLine obsahujici token s prazdnym obsahem a tokenID vytazene z vrcholu
// zasobniku gStackPtr
struct tLine gListConv(gStackPtr stack)
{
        struct tLine newLine;
        newLine.token = (TString *) malloc (sizeof(TString));
        newLine.token->myString = (char *) malloc(sizeof(char));
        // TODO: should be edited
        newLine.tokenID = gStackPop(stack);
        return newLine;
}
// stack end

int generateLine(tLinePtr, BTItemPtr *);
void copyString(char *, char*);

#endif
