#include "generator.h"

#define IFJcode17_ID 0

// struktura pro docasne ukladani vystupniho kodu
typedef struct outputCode{
	char* str;
	int stream;
	struct outputCode *next;
} *outputCodePtr;

outputCodePtr IFJoutput;
outputCodePtr IFJoutputFirst;

void output_init()
{
	IFJoutput = (outputCodePtr) malloc (sizeof(struct outputCode));
	//TODO: malloc fail
	IFJoutput->next = NULL;
	IFJoutputFirst = IFJoutput;
}


void output_err(char *newStr)
{
        IFJoutput->str = (char *) malloc (sizeof(char)*(strlen(newStr)+1));
        strcpy(IFJoutput->str, newStr);
	IFJoutput->stream = 1;	// stderr
        IFJoutput->next = (outputCodePtr) malloc (sizeof (struct outputCode));
        //TODO: malloc fail
        IFJoutput = IFJoutput->next;
        IFJoutput->next = NULL;
}

void output_new(char *newStr)
{
	IFJoutput->str = (char *) malloc (sizeof(char)*(strlen(newStr)+1));
	if(IFJoutput->str != NULL)
		strcpy(IFJoutput->str, newStr);
	IFJoutput->stream = 0; // stdout
        IFJoutput->next = (outputCodePtr) malloc (sizeof (struct outputCode));
        //TODO: malloc fail
        IFJoutput = IFJoutput->next;
	IFJoutput->next = NULL;
}

void output_print()
{
	outputCodePtr assist = IFJoutputFirst;
	outputCodePtr deleted = assist;
	while(assist != NULL)
	{
		deleted = assist;
		if(assist->str != NULL)
		{
			if(assist->stream == 0)
			{
				fprintf(stdout, "%s", assist->str);
				free(assist->str);

			}
			else if(assist->stream == 1)
			{
				fprintf(stderr, "%s", assist->str);
				free(assist->str);
			}
		}

		assist = assist->next;
		if(deleted != NULL)
			free(deleted);
	}
}



// stack for fuctions identificators
#define fStackMax 500
typedef struct fStack{
	char *array[fStackMax];
	int top;
} *fStackPtr;
void fStackPush(fStackPtr stack, char *id)
{
	stack->top += 1;
	stack->array[stack->top] = id;
}
char *fStackTop(fStackPtr stack)
{
	return stack->array[stack->top];
}
void fStackPop(fStackPtr stack)
{
	stack->top -= 1;
}
// globalni zasobnik identifikatoru funkci
struct fStack funcStackS;
fStackPtr funcStack = &funcStackS;
// najde id promenne v aktualni funkci
BTItemPtr *fStackSearch(BTNodePtr BTree, char* id)
{
        BTItemPtr *item = BTSearch(BTree, fStackTop(funcStack));
        if(item == NULL)
	{
		return NULL;
	}
	if(item->itemType == item_type_function)
        {
                return BTSearch(item->ParamRootPtr, id);
        }
	return NULL;
}



// global variable for initial print
int hVarOnce = 0;

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
        int isOp;       // 1 = je oprator
        int isConst;    // 1 = je konstanta
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
                                case Plus: output_new("+"); break;
                                case Minus: output_new("-"); break;
                                case Div: output_new("/"); break;
                                case Mul: output_new("*"); break;
                                case IntDiv: output_new("\\"); break;
                                case Equal: output_new("="); break;
                                case LowerGreater: output_new("<>"); break;
                                case Lower: output_new("<"); break;
                                case LowerOrEqual: output_new("<="); break;
                                case Greater: output_new(">"); break;
                                case GreaterOrEqual: output_new(">="); break;
                                case Int2Float: output_new("I2F"); break;
                                case Float2Int: output_new("F2I"); break;
                                case Float2Int2Float: output_new("F2I2F"); break;
				case Float2IntOsek: output_new("F2IOs"); break;
				case NoDataConversion: output_new("NOCONV"); break;
				case StrConcate: output_new("STRCON"); break;
                                default:
					{
						char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "#neznamy token[%d]", list->tokenID) + 1));
						sprintf(oHelp, "#neznamy token[%d]", list->tokenID);
						output_err(oHelp);
						if(oHelp != NULL)free(oHelp);break;
					}
                        }
                        output_new(" ");
                }
                else if (list->tokenID != valueOfString)
                {
			char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "%s ", list->data) + 1));
			sprintf(oHelp, "%s ", list->data);
			output_new(&oHelp[0]);
			if(oHelp != NULL)free(oHelp);
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







// globaclni zasobniky navesti pro label if a label loop
struct sLabel sStrLabelIf;
sLabelPtr sLabelIf = &sStrLabelIf;
struct sLabel sStrLabelLoop;
sLabelPtr sLabelLoop = &sStrLabelLoop;


// prevedeni hodnoty typu double na integer s preferenci sude cislice
int IFJcode17_exprDouble2Int(double val)
{
	int valInt = val;
	double rest = val - valInt;
	if(rest >= 0.5)
		valInt += 1;
	return valInt;
}

// kalkulace pro optimalizaci vyrazu (predpocitani konstant)
void IFJcode17_exprOptimalizeExecute(gListPtr first, gListPtr second, int op)
{
	if(op >= Equal && op <= GreaterOrEqual)	// relacni operator - neoptimalizuje se
		return;

	if(first->tokenID == valueOfInteger || first->tokenID == valueOfDouble);
	else
		return;

        if(second->tokenID == valueOfInteger || second->tokenID == valueOfDouble);
        else
                return;


	double oFirst;
	double oSecond;
	double result;
	sscanf(first->data, "%lf", &oFirst);
	sscanf(second->data, "%lf", &oSecond);

	switch(op)
	{
		case Plus:
			if(first->tokenID == valueOfInteger && second->tokenID == valueOfInteger)
			{
				int result = oFirst + oSecond;
				sprintf(first->data, "%d", result);
			}
			else if(first->tokenID == valueOfDouble || second->tokenID == valueOfDouble)
			{
				result = oFirst + oSecond;
				first->tokenID = valueOfDouble;
			}
			break;
		case Minus:
                        if(first->tokenID == valueOfInteger && second->tokenID == valueOfInteger)
                        {
                                int result = oFirst - oSecond;
                                sprintf(first->data, "%d", result);
                        }
                        else if(first->tokenID == valueOfDouble || second->tokenID == valueOfDouble)
                        {
                                result = oFirst - oSecond;
                                first->tokenID = valueOfDouble;
                        }
			break;
		case Mul:
                        if(first->tokenID == valueOfInteger && second->tokenID == valueOfInteger)
                        {
                                int result = oFirst * oSecond;
                                sprintf(first->data, "%d", result);
                        }
                        else if(first->tokenID == valueOfDouble || second->tokenID == valueOfDouble)
                        {
                                result = oFirst * oSecond;
                                first->tokenID = valueOfDouble;
                        }
			break;
		case Div:
			result = oFirst / oSecond;
			first->tokenID = valueOfDouble;
			break;
		case IntDiv:
			{
			int result; 	// rozsah integer
			result = IFJcode17_exprDouble2Int(oFirst) / IFJcode17_exprDouble2Int(oSecond);
			//result = (int) result2;
			sprintf(first->data, "%d", result);
			first->tokenID = valueOfInteger;
			break;
			}

		default: output_err("Optimalizator - neznamy operand\n");
	}
	if(op == IntDiv || ((first->tokenID == valueOfInteger && second->tokenID == valueOfInteger) && (op == Plus || op == Minus || op == Mul)))
	;else
		sprintf(first->data, "%g", result);
	first->isOp = 0;
	first->isConst = 1;
}


// optimalizacni funkce, predpocita konstanty a negeneruje je do vysledneho kodu
// vraci 1 pokud se povedla jedna optimalizacni iterace
// vraci 0 pokud jiz neni co optimalizovat
int IFJcode17_exprOptimalize(gListPtr *list)
{
	gListPtr first = NULL;
	gListPtr second = NULL;
	gListPtr act = (*list);
	int res = 0;

	while(act != NULL)
	{
		first = second;	// prvni operand
		second = act;	// druhy operand
		act = act->next; // aktivni prvek - operator pri optimalizaci
		if(first != NULL && second != NULL)
		{
			if(first->isConst == 1 && second->isConst == 1 && (act->tokenID >= Plus && act->tokenID <= IntDiv))
			{
				IFJcode17_exprOptimalizeExecute(first, second, act->tokenID);
				first->next = act->next;
				res = 1;
			}
		}
	}
	return res;
}



void IFJcode17_exprConvTypesExecute(gListPtr *list, int *array, int finalType)
{
	gListPtr listAssist = *list;
	gListPtr listAssistPrev = listAssist;

	unsigned int counter = 0;
        while(listAssist != NULL)
        {
		if(array[counter] != 0)
		{
			if(listAssist->tokenID == valueOfString || listAssist->tokenID == ID || (listAssist->tokenID >= Plus && listAssist->tokenID <= IntDiv)) // ID nebo operand
			{
				gListPtr newValue = (gListPtr) malloc (sizeof(struct gList));
				// TODO: malloc fail
				newValue->tokenID = array[counter];
				newValue->next = listAssist->next;
				newValue->isOp = 1;
				listAssist->next = newValue;
				listAssist = listAssist->next;
			}
			else if(listAssist->tokenID == valueOfInteger && array[counter] == Int2Float)
			{
				listAssist->tokenID = valueOfDouble;
			}
			else if (listAssist->tokenID == valueOfDouble && array[counter] == Float2Int)
			{
				double assistDouble;
				sscanf(listAssist->data, "%lf", &assistDouble);
				int assistInt = IFJcode17_exprDouble2Int(assistDouble);
				sprintf(listAssist->data, "%d", assistInt);
				listAssist->tokenID = valueOfInteger;
			}
                        else if (listAssist->tokenID == valueOfDouble && array[counter] == Float2Int2Float)
                        {
                                double assistDouble;
                                sscanf(listAssist->data, "%lf", &assistDouble);
                                int assistInt = IFJcode17_exprDouble2Int(assistDouble);
                                sprintf(listAssist->data, "%d", assistInt);
                                listAssist->tokenID = valueOfDouble;
                        }
		}
		counter += 1;
		listAssistPrev = listAssist;
		listAssist = listAssist->next;
	}



//	if(finalType == Int2Float || Float2Int) // pretypovani vysledku pro ulozeni do promenne
//	{
		gListPtr newValue = (gListPtr) malloc (sizeof(struct gList));
        	// TODO: malloc fail
                newValue->tokenID = finalType;
                newValue->next = NULL;
                newValue->isOp = 1;
                listAssistPrev->next = newValue;
//	}
}


void IFJcode17_exprConvTypes(gListPtr *list, BTNodePtr BTree, int assignType)
{
	typedef struct gListConv{
		int order;
		int dataType;
		struct gListConv *next;
	} *gListConvPtr;
	gListConvPtr convListFirst = (gListConvPtr) malloc (sizeof(struct gListConv));
	convListFirst->next = NULL;

	BTItemPtr *searchResult = (BTItemPtr *) malloc (sizeof(struct BTItem));
	// TODO: malloc fail
	unsigned int index = 0;
	gListConvPtr convListAssist = convListFirst;
	gListPtr listAssist = *list;
	while(listAssist != NULL)
	{
		convListAssist->order = index;
		index += 1;
		if(listAssist->tokenID == ID)	// zpracovava se identifikator promenne
		{
			//searchResult = BTSearch(BTree, listAssist->data);
			searchResult = fStackSearch(BTree, listAssist->data);

			if(searchResult == NULL)
			{
				char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "Chyba generatoru. identifikator [%s] nenalezen v tabulce symbolu.\n", listAssist->data) + 1));
				sprintf(oHelp, "Chyba generatoru. identifikator [%s] nenalezen v tabulce symbolu.\n", listAssist->data);
				output_err(oHelp);
				if(oHelp != NULL)free(oHelp);
				return; // TODO: error codes
			}
			switch(searchResult->varData->type)
			{
				case var_integer: convListAssist->dataType = valueOfInteger; break;
				case var_double: convListAssist->dataType = valueOfDouble; break;
				case var_string: convListAssist->dataType = valueOfString; break;
				default: output_err("Chyba generatoru. Konverze datovych typu se nepovedla.\n");
			}
		}
		else	// zpracovava se konstanta nebo operator
		{
			convListAssist->dataType = listAssist->tokenID;
		}
		listAssist = listAssist->next;
		if(listAssist != NULL)
		{
			convListAssist->next = (gListConvPtr) malloc (sizeof(struct gListConv));
			// TODO: malloc fail
			convListAssist->next->next = NULL;
		}
		convListAssist = convListAssist->next;
	}

	int *array = (int *) malloc (sizeof(int)*(index+1));
	// TODO: malloc fail

	for (unsigned int y = 0; y < index; y++)
		array[y] = 0;

	gListConvPtr firstPtr;	// prvni operand
	gListConvPtr secondPtr;	// druhy operand

	int first;
	int second;
	int op;

	int converted = 1; // povedla se jedna iterace

	gListConvPtr convListAssist4;

	while(converted == 1)
	{
		converted = 0;
		first = 0;
		second = 0;
		op = 0;

		firstPtr = NULL;
		secondPtr = NULL;

		convListAssist = convListFirst;
		convListAssist4 = convListAssist;
	while(convListAssist != NULL)
	{
		first = second;
		second = op;
		op = convListAssist->dataType;

		convListAssist4 = firstPtr;
		firstPtr = secondPtr;
		secondPtr = convListAssist;


		if((first == valueOfInteger || first == valueOfDouble || first == valueOfString) &&
		(second == valueOfInteger || second == valueOfDouble || second == valueOfString) &&
		(op >= Plus && op <= GreaterOrEqual))	// lze vypocitat
		{
			converted = 1;
			switch(op)
			{
				case Div:
                                        if(first == valueOfInteger && second == valueOfDouble)
                                        {
                                                array[convListAssist->order] = Int2Float;
                                        }
                                        else if(first == valueOfDouble && second == valueOfInteger)
                                        {
                                                array[convListAssist4->next->order] = Int2Float;
                                        }
                                        else if(first == valueOfInteger && second == valueOfInteger)
                                        {
                                                array[convListAssist4->order] = Int2Float;
                                                array[convListAssist4->next->order] = Int2Float;
                                        }
                                        else // oba jsou double
					{
					}
                                        convListAssist4->dataType = valueOfDouble;
					break;
                                case IntDiv:
					if(first == valueOfInteger)
						array[convListAssist4->order] = Int2Float;
                                        else if(first == valueOfDouble)
                                                array[convListAssist4->order] = Float2Int2Float;
                                        if(second == valueOfInteger)
                                                array[convListAssist4->next->order] = Int2Float;
                                        else if(first == valueOfDouble)
                                                array[convListAssist4->next->order] = Float2Int2Float;

                                        convListAssist4->dataType = valueOfInteger;
                                        break;
				default:
                                        if(first == valueOfInteger && second == valueOfDouble)
                                        {
                                                convListAssist4->dataType = valueOfDouble;
                                                array[convListAssist4->order] = Int2Float;
                                        }
                                        else if(op == Plus && (first == valueOfString && second == valueOfString))
                                        {
                                                array[convListAssist4->next->order] = StrConcate;
                                        }

                                        else if(first == valueOfDouble && second == valueOfInteger)
                                        {
                                                convListAssist4->dataType = valueOfDouble;
                                                array[convListAssist4->next->order] = Int2Float;
                                        }
                                        else if(first == valueOfInteger && second == valueOfInteger) // oba jsou integer
                                        {
                                                if(op == Div)
                                                {
                                                        convListAssist4->dataType = valueOfDouble;
                                                }
                                                else
                                                {
                                                        convListAssist4->dataType = valueOfInteger;
                                                }
                                        }
                                        else // oba jsou double
                                                convListAssist4->dataType = valueOfDouble;
                                        break;

			}
			convListAssist4->order = convListAssist->order;
			convListAssist4->next = convListAssist->next; // preklenuti
			// TODO: uvolneni preklenuti (ztrata 2 ukazatelu)
			break;	// TODO
		}

		convListAssist = convListAssist->next;
	}
	}

	int finalDataType = convListFirst->dataType;
	if(finalDataType != assignType)	// nesouhlasi datovy typ vysledku a promenne do ktere se zapise
	{
		if(finalDataType == valueOfDouble && assignType == valueOfInteger)
		{
			IFJcode17_exprConvTypesExecute(list, array, Float2IntOsek);
		}
		else if(finalDataType == valueOfInteger && assignType == valueOfDouble)
		{
			IFJcode17_exprConvTypesExecute(list, array, Int2Float);
		}
		else// if(finalDataType == valueOfString)
		{
			IFJcode17_exprConvTypesExecute(list, array, NoDataConversion);
		}
	}
	else
	{
		IFJcode17_exprConvTypesExecute(list, array, NoDataConversion);	// typy souhlasi
	}
}


// prevede vyraz na postfixovou (polskou) notaci
gListPtr IFJcode17_toPostfix(tLinePtr *temp, BTNodePtr BTree, int assignType)
{
	struct gStack st;
	gStackPtr stack = &st;
	gStackInit(stack);

	//struct gList list;
	gListPtr gListFirst = (gListPtr) malloc (sizeof(struct gList));
	// TODO: malloc fail
	gListInit(gListFirst);

	while((*temp) != NULL)
	{
		if((*temp)->tokenID == Semicolon) // kvuli [Print (5 + 2); 1-1;]
			break;

		if((*temp)->tokenID == ID || (*temp)->tokenID == valueOfDouble ||
		(*temp)->tokenID == valueOfString || (*temp)->tokenID == valueOfInteger ||
		(*temp)->tokenID == valueOfDoubleWithExp) // zpracovava se operand
		{
			gListAdd(&gListFirst, *temp);
		}
		else if((*temp)->tokenID == LeftParenthes) // zpracovava se leva zavorka
		{
			gStackPush(stack, LeftParenthes);
		}
                else if((*temp)->tokenID == RightParenthes) // zpracovava se prava zavorka
		{
			while(stack->array[stack->top].id != LeftParenthes)
			{
				struct tLine newLine = gListConv(stack);
				gListAdd(&gListFirst, &newLine); // operator ze zas. do sezn.
			}
			gStackPop(stack); // zbaveni se leve zavorky na vrcholu zasobiku
		}
		else if((*temp)->tokenID >= Plus && (*temp)->tokenID <= GreaterOrEqual)	// zpracovava se operator
		{
			int success = 0;
			while(success == 0)
			{
				if(stack->top == -1 || stack->array[stack->top].id == LeftParenthes ||
				stack->array[stack->top].priority > gStackPriority((*temp)->tokenID))
				{
					gStackPush(stack, (*temp)->tokenID);
					success = 1;
				}
				else if(stack->array[stack->top].priority <= gStackPriority((*temp)->tokenID))
				{
					struct tLine newLine = gListConv(stack);
					gListAdd(&gListFirst, &newLine); // operator ze zas. do sezn.
				}
			}
		}
		else	// token neni soucasti vyrazu
		{
			break;	// preruseni cyklu while
		}
		*temp = (*temp)->next;
	}
	while(stack->top != -1)
	{
		struct tLine newLine = gListConv(stack);
		gListAdd(&gListFirst, &newLine); // operator ze zas. do sezn.
	}

	// TODO: pak smazat vypisy
	//output_new("\n# -------- TEST VYRAZU --------\n# Prevedeny na postfix: ");
	//gListPrint(gListFirst);
	while(IFJcode17_exprOptimalize(&gListFirst) == 1);	// optimalizace vyrazu
	//output_new("\n# Vyraz po optimalizaci (predpocitani konstant): ");
	//gListPrint(gListFirst);
	//output_new("\n# Vyraz po implicitni konverzi datovych typu: ");
	IFJcode17_exprConvTypes(&gListFirst, BTree, assignType);
	//gListPrint(gListFirst);
	//output_new("\n# -------- KONEC TESTU --------\n\n");

	return gListFirst;
}


// vyhodnoceni vyrazu bez pravdivostnich hodnot a relacnich opteratoru (true, false)
void IFJcode17_evalExpr(tLinePtr *temp, BTNodePtr BTree, int assignType)
{
        gListPtr list, assistList;
	list = IFJcode17_toPostfix(temp, BTree, assignType);	// ulozeni vytupniho seznamu vyrazu v postfixu
	assistList = list;
	while(assistList != NULL)
	{
		if(assistList->isOp == 1)
		{
			switch(assistList->tokenID)
			{
				case StrConcate:
							output_new("PUSHFRAME\n");
							output_new("CREATEFRAME\n");
							output_new("DEFVAR TF@Concate1\n");
                                                        output_new("DEFVAR TF@Concate2\n");
                                                        output_new("POPS TF@Concate2\n");
                                                        output_new("POPS TF@Concate1\n");
							output_new("CONCAT TF@Concate1 TF@Concate1 TF@Concate2\n");
							output_new("PUSHS TF@Concate1\n");
							output_new("POPFRAME\n");
							assistList = assistList->next; break;

				case Float2IntOsek:	output_new("FLOAT2R2EINTS\n"); break; // nakonec nejde o oseknuti (asi)
				case Float2Int2Float: 	output_new("FLOAT2R2EINTS\n");
							output_new("INT2FLOATS\n"); break;
				case Int2Float: output_new("INT2FLOATS\n"); break;
				case Float2Int:	output_new("FLOAT2R2EINTS\n"); break;
				case Plus: output_new("ADDS\n"); break;
                                case Minus: output_new("SUBS\n"); break;
                                case Mul: output_new("MULS\n"); break;
                                case Div: output_new("DIVS\n"); break;
                                case IntDiv: 	output_new("DIVS\n");
						output_new("FLOAT2INTS\n"); break;
				case Equal:	output_new("EQS\n"); break;
                                case Lower:     output_new("LTS\n"); break;
                                case Greater:     output_new("GTS\n"); break;
                                case LowerGreater:     output_new("EQS\n");
							output_new("NOTS\n"); break;
                                case LowerOrEqual: case GreaterOrEqual:
						output_new("\n# BEGINNING: <= or >= operator\n"); // navic kvuli prehlednosti
						output_new("CREATEFRAME\n");
						output_new("DEFVAR TF@assist1\n");
						output_new("DEFVAR TF@assist2\n");
						output_new("DEFVAR TF@result1\n");
						output_new("DEFVAR TF@result2\n");
						output_new("POPS TF@assist1\n");
						output_new("POPS TF@assist2\n");
						if(assistList->tokenID == LowerOrEqual)
							output_new("LT ");
						else
							output_new("GT ");
						output_new("TF@result1 TF@assist2 TF@assist1\n");
						output_new("EQ TF@result2 TF@assist2 TF@assist1\n");
						output_new("PUSHS TF@result1\n");
						output_new("PUSHS TF@result2\n");
						output_new("ORS\n");
						output_new("CREATEFRAME\n");
						output_new("# END: <= or >= operator\n\n"); // navic kvuli prehlednosti
							break;

				// TODO: relacni operatory
			}
		}
		else
		{
			output_new("PUSHS ");
			if(assistList->isConst == 1)
			{
				switch(assistList->tokenID)
                        	{
                                	case valueOfInteger: output_new("int@"); break;
                               		case valueOfDouble: output_new("float@"); break;
                        		case valueOfString:
								IFJcode17_writeString(assistList->data);
								output_new("\n");
								break;
					// TODO: relacni operatory
				}
			}
			else
			{
				output_new("LF@");
			}
			if(assistList->tokenID != valueOfString)
			{
				char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "%s\n", assistList->data) + 1));
        			sprintf(oHelp, "%s\n", assistList->data);
        			output_new(oHelp);
        			if(oHelp != NULL)free(oHelp);
			}
		}
		assistList = assistList->next;
	}
}



// zpracovani prikazu IF
void IFJcode17_if(tLinePtr *temp, BTNodePtr BTree)
{
	labelIf += 1;
	sLabelPush(sLabelIf, labelIf);
	if((*temp)->next != NULL)
		*temp = (*temp)->next;
	if((*temp)->tokenID != Then)
		IFJcode17_evalExpr(temp, BTree, 0);
	output_new("PUSHS bool@true\n");

	char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "JUMPIFNEQS LABELIF%d", sLabelTop(sLabelIf)) + 1));
        sprintf(oHelp, "JUMPIFNEQS LABELIF%d", sLabelTop(sLabelIf));
        output_new(oHelp);
        if(oHelp != NULL)free(oHelp);

	while((*temp)->tokenID != Then)
		*temp = (*temp)->next;

	*temp = (*temp)->next;	// zbaveni se tokenu Then
}

// zpracovani prikazu ELSE (pouziva navesti z predchoziho IF)
void IFJcode17_else(tLinePtr *temp)
{
        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "JUMP LABELIFEND%d\n", sLabelTop(sLabelIf)) + 1));
        sprintf(oHelp, "JUMP LABELIFEND%d\n", sLabelTop(sLabelIf));
        output_new(oHelp);
        if(oHelp != NULL)free(oHelp);

        char *pHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "LABEL LABELIF%d", sLabelTop(sLabelIf)) + 1));
        sprintf(pHelp, "LABEL LABELIF%d", sLabelTop(sLabelIf));
        output_new(pHelp);
        if(pHelp != NULL)free(pHelp);

	if((*temp)->next != NULL)
		*temp = (*temp)->next;	// zbaveni se tokenu Else
}

// zpracovani sekvence prikazu end if
void IFJcode_ifend(tLinePtr *temp)
{
        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "LABEL LABELIFEND%d\n", sLabelTop(sLabelIf)) + 1));
        sprintf(oHelp, "LABEL LABELIFEND%d\n", sLabelTop(sLabelIf));
        output_new(oHelp);
        if(oHelp != NULL)free(oHelp);

	sLabelPop(sLabelIf);

	while((*temp)->next != NULL)
		*temp = (*temp)->next;  // zbaveni se tokenu (end) if
}


// zpracovani sekvence prikazu DO WHILE
void IFJcode17_dowhile(tLinePtr *temp, BTNodePtr BTree)
{

	labelLoop += 1;
	sLabelPush(sLabelLoop, labelLoop);

        if((*temp)->next != NULL)
                *temp = (*temp)->next;  // zbaveni se tokenu DO
        if((*temp)->next != NULL)
                *temp = (*temp)->next;  // zbaveni se tokenu WHILE


        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "LABEL LABELLOOP%d\n", sLabelTop(sLabelLoop)) + 1));
        sprintf(oHelp, "LABEL LABELLOOP%d\n", sLabelTop(sLabelLoop));
        output_new(oHelp);
        if(oHelp != NULL)free(oHelp);



	if((*temp)->tokenID != While)
		IFJcode17_evalExpr(temp, BTree, 0);

	output_new("PUSHS bool@true\n");

        char *pHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "JUMPIFNEQS LABELLOOPEND%d\n", sLabelTop(sLabelLoop)) ));
        sprintf(pHelp, "JUMPIFNEQS LABELLOOPEND%d\n", sLabelTop(sLabelLoop));
        output_new(pHelp);
        if(pHelp!=NULL)free(pHelp);
}

// zpracovani ukoncovaciho prikazu cyklu LOOP
void IFJcode17_loop(tLinePtr *temp)
{
        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "JUMP LABELLOOP%d\n", sLabelTop(sLabelLoop)) + 1));
        sprintf(oHelp, "JUMP LABELLOOP%d\n", sLabelTop(sLabelLoop));
        output_new(oHelp);
        if(oHelp != NULL)free(oHelp);

        char *pHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "LABEL LABELLOOPEND%d", sLabelTop(sLabelLoop)) + 1));
        sprintf(pHelp, "LABEL LABELLOOPEND%d", sLabelTop(sLabelLoop));
        output_new(pHelp);
        if(pHelp!=NULL)free(pHelp);

	sLabelPop(sLabelLoop);

	while((*temp)->next != NULL)
		*temp = (*temp)->next;  // zbaveni se tokenu LOOP
}


// pomocna funkce pro generovani instrukce WRITE
// vytiskne na stdout lomitko a za nim tricifernou ascii hodnotu jednoho znaku
void IFJcode17_writechar(int character)
{

	if(character < 0)
	{
		character = 256 + character;
	}
	output_new("\\");
		if(character < 100)
			output_new("0");
		if(character < 10)
			output_new("0");

        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "%d", character) + 1));
        sprintf(oHelp, "%d", character);
        output_new(oHelp);
        if(oHelp != NULL)free(oHelp);
}

// vytisknuti retezce na stdout podle pravidel IFJcode17
void IFJcode17_writeString(char *myString)
{
	output_new("string@");
        for(int i = 0; myString[i] != '\0'; i++)
        {
        	IFJcode17_writechar(myString[i]);
	}
}

// generovani instrukce WRITE
void IFJcode17_write(tLinePtr *temp,  BTNodePtr BTree)
{
        *temp = (*temp)->next;
        while(*temp != NULL)
        {
                //if((*temp)->tokenID != Semicolon)
                //        printf("WRITE ");

                if((*temp)->tokenID == Semicolon)
                        *temp = (*temp)->next;
                else if((*temp)->tokenID == Exclamation)
                {
                        *temp = (*temp)->next;
			IFJcode17_writeString((*temp)->token->myString);
                        *temp = (*temp)->next;
                        if((*temp) != NULL)
				output_new("\n");
                }

		if(*temp != NULL)
		{
			IFJcode17_evalExpr(temp, BTree, 0);
			output_new("CREATEFRAME\n");
			output_new("DEFVAR TF@WriteHelp\n");
			output_new("POPS TF@WriteHelp\n");
			output_new("WRITE TF@WriteHelp\n");
		}
/*
		else if((*temp)->tokenID == valueOfString)
                {
                        IFJcode17_writeString((*temp)->token->myString);
                        *temp = (*temp)->next;
                        if((*temp) != NULL)
                                printf("\n");
                }
                else if((*temp)->tokenID == valueOfInteger)
                {
			printf("int@%s", (*temp)->token->myString);
                        *temp = (*temp)->next;
                        if((*temp) != NULL)
                                printf("\n");
                }
                else if((*temp)->tokenID == valueOfDouble)
                {
                        printf("float@%s", (*temp)->token->myString);
                        *temp = (*temp)->next;
                        if((*temp) != NULL)
                                printf("\n");
                }
                else if((*temp)->tokenID == ID)
                {
                        printf("LF@%s", (*temp)->token->myString);
                        *temp = (*temp)->next;
                	if((*temp) != NULL)
				printf("\n");
		}
*/
        }

}

// generovani inicializace promenne na 0
void IFJcode17_varInit(tLinePtr *temp, BTNodePtr BTree)
{
        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "DEFVAR LF@%s\n", (*temp)->next->token->myString) + 1));
        sprintf(oHelp, "DEFVAR LF@%s\n", (*temp)->next->token->myString);
        output_new(oHelp);
        if(oHelp != NULL)free(oHelp);

	BTItemPtr *item = fStackSearch(BTree, (*temp)->next->token->myString); // pokus o nelezeni id v aktualni funkci
        if(item != NULL)
        {
	        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "MOVE LF@%s ", (*temp)->next->token->myString) + 1));
       		sprintf(oHelp, "MOVE LF@%s ", (*temp)->next->token->myString);
        	output_new(oHelp);
        	if(oHelp != NULL)free(oHelp);

                if(item->varData->type == var_integer)
			output_new("int@0\n");
                else if(item->varData->type == var_double)
			output_new("float@0\n");
		else if(item->varData->type == var_string)
                        output_new("string@\n");
        }

}

// prirazeni vysledku funkce
void IFJcode17_funcAssign(tLinePtr *temp, int *parameterTypes, BTNodePtr BTree)
{
	if((*temp)->next != NULL)
		*temp = (*temp)->next;

	int counter = 0;
	while((*temp)->next != NULL)
	{
		if((*temp)->tokenID == ID)
		{

                        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "PUSHS LF@%s\n", (*temp)->token->myString) + 1));
                        sprintf(oHelp, "PUSHS LF@%s\n", (*temp)->token->myString);
                        output_new(oHelp);
                        if(oHelp != NULL)free(oHelp);

			BTItemPtr *item = fStackSearch(BTree, (*temp)->token->myString);
			if(item != NULL)
			{
				if(item->varData->type == var_integer && parameterTypes[counter] == Double)
					output_new("INT2FLOATS\n");
				else if(item->varData->type == var_double && parameterTypes[counter] == Integer)
					output_new("FLOAT2R2EINTS\n");
			}
			counter++;
		}
		else if((*temp)->tokenID == valueOfInteger)
		{
                        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "PUSHS int@%s\n", (*temp)->token->myString) + 1));
                        sprintf(oHelp, "PUSHS int@%s\n", (*temp)->token->myString);
                        output_new(oHelp);
                        if(oHelp != NULL)free(oHelp);

			if(parameterTypes[counter] == Double)
				output_new("INT2FLOATS\n");
			counter++;
                }
		else if((*temp)->tokenID == valueOfDouble || (*temp)->tokenID == valueOfDoubleWithExp)
                {
                        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "PUSHS float@%s\n", (*temp)->token->myString) + 1));
                        sprintf(oHelp, "PUSHS float@%s\n", (*temp)->token->myString);
                        output_new(oHelp);
			if(oHelp != NULL)free(oHelp);

                	if(parameterTypes[counter] == Integer)
				output_new("FLOAT2R2EINTS\n");
			counter++;
		}
		else if((*temp)->tokenID == valueOfString)
                {
		        //printf("PUSHS string@%s\n", (*temp)->token->myString);
			output_new("PUSHS ");
			IFJcode17_writeString((*temp)->token->myString);
			output_new("\n");
			counter++;
		}


		*temp = (*temp)->next;
	}
}



// generovani prirazeni do promenne
void IFJcode17_varAssign(tLinePtr *temp, BTNodePtr BTree)
{
	char *varNameHolder = (*temp)->token->myString;
	while((*temp)->tokenID != Equal) // zbaveni se vsech tokenu az po Equal
		*temp = (*temp)->next;
	*temp = (*temp)->next;	// zbaveni se tokenu Equal (=) printf ("TU\n");


	BTItemPtr *item = fStackSearch(BTree, varNameHolder); // pokus o nelezeni id v aktualni funkci
	int assignType = 0;

	if(item != NULL)
	{
		if(item->varData->type == var_integer)
			assignType = valueOfInteger;
		else if(item->varData->type == var_double)
       			assignType = valueOfDouble;
	}
	if(*temp != NULL)
	{
		if((*temp)->tokenID == ID)
		{
			item = BTSearch(BTree, (*temp)->token->myString);	// vyhledani prvniho operandu ve vyrazu (jestli je to funkce nebo promenna)
			if(item != NULL)
			{
				if(item->itemType == item_type_function)
				{
					char *funcNameHolder = (*temp)->token->myString;
					IFJcode17_funcAssign(temp, item->parameterTypes, BTree);

		                        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "CALL %s\n", funcNameHolder) + 1));
					sprintf(oHelp, "CALL %s\n", funcNameHolder);
                        		output_new(oHelp);
                        		if(oHelp != NULL)free(oHelp);

					output_new("POPFRAME\n");
					if(assignType == valueOfInteger && item->returnType == var_double)
						output_new("FLOAT2R2EINTS\n");
					else if(assignType == valueOfDouble && item->returnType == var_integer)
						output_new("INT2FLOATS\n");

                                        char *pHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "POPS LF@%s ", varNameHolder) + 1));
                                        sprintf(pHelp, "POPS LF@%s ", varNameHolder);
                                        output_new(pHelp);
                                        if(pHelp!=NULL)free(pHelp);


					return;
				}
			}
		}
	}

	IFJcode17_evalExpr(temp, BTree, assignType);

        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "POPS LF@%s ", varNameHolder) + 1));
        sprintf(oHelp, "POPS LF@%s ", varNameHolder);
        output_new(oHelp);
        if(oHelp != NULL)free(oHelp);
}


// generovani prikazu Input
void IFJcode17_input(tLinePtr *temp, BTNodePtr BTree)
{
	if((*temp)->next != NULL)
		*temp = (*temp)->next;

	char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "READ LF@%s ", (*temp)->token->myString) + 1));
        sprintf(oHelp, "READ LF@%s ", (*temp)->token->myString);
        output_new(oHelp);
        if(oHelp != NULL)free(oHelp);

	//BTItemPtr *var_btPtr = BTSearch(BTree, (*temp)->token->myString);
        BTItemPtr *var_btPtr = fStackSearch(BTree, (*temp)->token->myString);

	if(var_btPtr->itemType == item_type_variable)
	{
		switch(var_btPtr->varData->type)
		{
			case var_integer: output_new("int"); break;
			case var_double: output_new("float"); break;
			case var_string: output_new("string"); break;
			default: output_err("Chyba generatoru - neznamy typ promenne\n");
		}
		output_new("\n");
	}
}

// Zpracovani parametru funkce pri definici
void IFJcode17_functionParams(tLinePtr *temp)
{
	if((*temp)->next != NULL)
		*temp = (*temp)->next;

	struct fStack paramStackS;
	fStackPtr paramStack = &paramStackS;
	paramStack->top = 0;
	while((*temp) != NULL)
	{
		if((*temp)->tokenID == ID)
		{
        		char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "DEFVAR LF@%s\n", (*temp)->token->myString) + 1));
        		sprintf(oHelp, "DEFVAR LF@%s\n", (*temp)->token->myString);
        		output_new(oHelp);
        		if(oHelp != NULL)free(oHelp);

			fStackPush(paramStack, (*temp)->token->myString);
		//	printf("POPS LF@%s\n", (*temp)->token->myString);
		}
		*temp = (*temp)->next;
	}
	while(paramStack->top != 0)
	{
                char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "POPS LF@%s\n", fStackTop(paramStack)) + 1));
                sprintf(oHelp, "POPS LF@%s\n", fStackTop(paramStack));
                output_new(oHelp);
                if(oHelp != NULL)free(oHelp);

		fStackPop(paramStack);
	}
}


// Zpracovani definice funkce
void IFJcode17_function(tLinePtr *temp)
{
	if((*temp)->next != NULL)
		*temp = (*temp)->next;

	fStackPush(funcStack, (*temp)->token->myString);
        char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "LABEL %s\n", (*temp)->token->myString) + 1));
        sprintf(oHelp, "LABEL %s\n", (*temp)->token->myString);
        output_new(oHelp);
        if(oHelp != NULL)free(oHelp);

	output_new("CREATEFRAME\n");
	output_new("PUSHFRAME\n");
	IFJcode17_functionParams(temp);
}



// Generovani jednoho radku vstupniho kodu
int generateLine(tLinePtr tInput, BTNodePtr BTree)
{
	if(hVarOnce == 0) // probehne jednou na zacatku
	{
		hVarOnce = 1;
		output_init();
		output_new(".IFJcode17\n");
		output_new("CALL $Scope\n");
		char *tempScope = (char *) malloc (sizeof(char) * strlen ("@Scope"));
		// TODO: malloc fail + uvolnit
		strcpy(tempScope, "@Scope");
		fStackPush(funcStack, tempScope);
		IFJcode17_embeddedFunctions();	// vypise na zacatek 4 vestavene funkce ze zadani
	}

	tLinePtr temp = tInput;
	do
	{
		//TString *tempToken = (TString *) malloc (sizeof(TString));
		switch(temp->tokenID)
		{
			case Exclamation:
			    break;
			case Semicolon:
			    break;
		 	case End:
			    if(temp->next != NULL)
			    {
				if(temp->next->tokenID == Scope)
			    	{
					output_new("CLEARS\n");
			    		output_new("POPFRAME");
				}
				else if(temp->next->tokenID == If)
				{
					IFJcode_ifend(&temp);
				}
				else if(temp->next->tokenID == Function)
				{
                                        BTItemPtr *item = BTSearch(BTree, fStackTop(funcStack));
                                        if(item != NULL)
                                        {
                                                if(item->returnType == var_integer)
                                                        output_new("PUSHS int@0\n");
                                                if(item->returnType == var_double)
							output_new("PUSHS float@0\n");
                                                if(item->returnType == var_string)
							output_new("PUSHS string@\n");
					}
					output_new("RETURN");
					fStackPop(funcStack);
				}
			    }
			    if(temp->next != NULL)
				temp = temp->next;
			    break;
			case Scope:
			    output_new("\n\nLABEL $Scope\n");
			    output_new("CREATEFRAME\n");
			    output_new("PUSHFRAME\n");
			    output_new("CREATEFRAME");
    			    break;
			case Dim:
			    IFJcode17_varInit(&temp, BTree);
			    break;
			case As:
				// TODO: inicializace "Dim promenna as Integer = 3"
			    break;
			case ID:
			    if(temp->next != NULL)
			    {
				    if(temp->next->tokenID == Equal)
			    		IFJcode17_varAssign(&temp, BTree);
				    else if(temp->next->tokenID == As)
				    {
				        if(temp->next->next != NULL)
					{
						if(temp->next->next->next != NULL)
						 if(temp->next->next->next->tokenID == Equal)
							IFJcode17_varAssign(&temp, BTree);
					}
				    }
			    }
			    else
			    {
				char *oHelp = (char *) malloc (sizeof(char) * (snprintf(NULL, 0, "LF@%s", temp->token->myString) + 1));
        			sprintf(oHelp, "LF@%s", temp->token->myString);
        			output_new(oHelp);
        			if(oHelp != NULL)free(oHelp);
			    }break;
			case Function:
			    IFJcode17_function(&temp);
			    break;
			case Declare:
			    while(temp->next != NULL)
			    {
				temp = temp->next;
			    }
			    break;
			case Return:
			    if(temp->next != NULL)
			    {
				temp = temp->next;
				if(temp != NULL)
				{
					BTItemPtr *item = BTSearch(BTree, fStackTop(funcStack));
					if(item != NULL)
					{
						if(item->returnType == var_integer)
							IFJcode17_evalExpr(&temp, BTree, valueOfInteger);
						if(item->returnType == var_double)
                                                	IFJcode17_evalExpr(&temp, BTree, valueOfDouble);
						if(item->returnType == var_string)
                                                	IFJcode17_evalExpr(&temp, BTree, valueOfString);
					}
				}
				output_new("RETURN\n");
			    }
			    break;
			case Equal:
			    break;
			case Print:
			    IFJcode17_write(&temp, BTree);
			    break;
			case valueOfString:
			    break;
			case Input:
			    output_new("WRITE string@?\\032\n");
			    IFJcode17_input(&temp, BTree);
			    break;
			case If:
			    IFJcode17_if(&temp, BTree);
			    break;
			case Else:
			    IFJcode17_else(&temp);
			    break;
			case Do:
			    IFJcode17_dowhile(&temp, BTree);
			    break;
			case Loop:
			    IFJcode17_loop(&temp);
			    break;
		}
		if(temp != NULL)
			temp = temp->next;
	}while(temp != NULL && (temp->tokenID >= ID && temp->tokenID <= EndOfLine) );
	// TODO: upravit podminku cyklu (problem s duplicitnim "end scope", temp->next nebyl NULL)
	output_new("\n");
	return 1;
}


// vypsani vestavenych funkci prekladace
void IFJcode17_embeddedFunctions()
{
output_new("##################### Length ####################\n");
output_new("\n");
output_new("LABEL length\n");
output_new("CREATEFRAME\n");
output_new("PUSHFRAME\n");
output_new("DEFVAR LF@Assist\n");
output_new("DEFVAR LF@AString\n");
output_new("POPS LF@AString\n");
output_new("STRLEN LF@Assist LF@AString\n");
output_new("PUSHS LF@Assist\n");
output_new("RETURN\n");
output_new("\n");
output_new("\n");
output_new("\n");
output_new("###################### Asc ######################\n");
output_new("\n");
output_new("LABEL asc\n");
output_new("CREATEFRAME\n");
output_new("PUSHFRAME\n");
output_new("DEFVAR LF@AscS\n");
output_new("DEFVAR LF@AscI\n");
output_new("POPS LF@AscI\n");
output_new("POPS LF@AscS\n");
output_new("SUB LF@AscI LF@AscI int@1\n");
output_new("\n");
output_new("DEFVAR LF@AscHelp\n");
output_new("LT LF@AscHelp LF@AscI int@0\n");
output_new("JUMPIFEQ AscZero LF@AscHelp bool@true\n");
output_new("\n");
output_new("PUSHS LF@AscI\n");
output_new("PUSHS LF@AscS\n");
output_new("CALL length\n");
output_new("POPFRAME\n");
output_new("\n");
output_new("GTS\n");
output_new("PUSHS bool@true\n");
output_new("JUMPIFEQS AscZero\n");
output_new("\n");
output_new("DEFVAR LF@AscResult\n");
output_new("STRI2INT LF@AscResult LF@AscS LF@AscI\n");
output_new("PUSHS LF@AscResult\n");
output_new("RETURN\n");
output_new("\n");
output_new("\n");
output_new("LABEL AscZero\n");
output_new("PUSHS int@0\n");
output_new("RETURN\n");
output_new("\n");
output_new("\n");
output_new("#################### Chr ########################\n");
output_new("\n");
output_new("LABEL chr\n");
output_new("CREATEFRAME\n");
output_new("PUSHFRAME\n");
output_new("DEFVAR LF@ChrHelp\n");
output_new("DEFVAR LF@ChrResult\n");
output_new("POPS LF@ChrHelp\n");
output_new("\n");
output_new("INT2CHAR LF@ChrResult LF@ChrHelp\n");
output_new("PUSHS LF@ChrResult\n");
output_new("\n");
output_new("RETURN\n");
output_new("\n");
output_new("\n");
output_new("#################### SubStr #####################\n");
output_new("\n");
output_new("\n");
output_new("LABEL substr\n");
output_new("CREATEFRAME\n");
output_new("PUSHFRAME\n");
output_new("DEFVAR LF@SubStrN\n");
output_new("DEFVAR LF@SubStrI\n");
output_new("DEFVAR LF@SubStrS\n");
output_new("POPS LF@SubStrN\n");
output_new("POPS LF@SubStrI\n");
output_new("POPS LF@SubStrS\n");
output_new("\n");
output_new("DEFVAR LF@SubStrHelp\n");
output_new("EQ LF@SubStrHelp LF@SubStrS string@\n");
output_new("JUMPIFEQ SubStrZero LF@SubStrHelp bool@true\n");
output_new("LT LF@SubStrHelp LF@SubStrI int@1\n");
output_new("JUMPIFEQ SubStrZero LF@SubStrHelp bool@true\n");
output_new("EQ LF@SubStrHelp LF@SubStrN int@0\n");
output_new("JUMPIFEQ SubStrZero LF@SubStrHelp bool@true\n");
output_new("\n");
output_new("\n");
output_new("SUB LF@SubStrI LF@SubStrI int@1\n");
output_new("DEFVAR LF@SubStrResult\n");
output_new("MOVE LF@SubStrResult string@\n");
output_new("DEFVAR LF@SubStrResultChar\n");
output_new("DEFVAR LF@SubStrLen\n");
output_new("PUSHS LF@SubStrS\n");
output_new("CALL length\n");
output_new("POPFRAME\n");
output_new("POPS LF@SubStrLen\n");
output_new("SUB LF@SubStrLen LF@SubStrLen int@1\n");
output_new("\n");
output_new("LABEL SubStrLoop\n");
output_new("GETCHAR LF@SubStrResultChar LF@SubStrS LF@SubStrI\n");
output_new("CONCAT LF@SubStrResult LF@SubStrResult LF@SubStrResultChar\n");
output_new("ADD LF@SubStrI LF@SubStrI int@1\n");
output_new("SUB LF@SubStrN LF@SubStrN int@1\n");
output_new("GT LF@SubStrHelp LF@SubStrI LF@SubStrLen\n");
output_new("JUMPIFEQ SubStrEnd LF@SubStrHelp bool@true\n");
output_new("EQ LF@SubStrHelp LF@SubStrN int@0\n");
output_new("JUMPIFEQ SubStrEnd LF@SubStrHelp bool@true\n");
output_new("JUMP SubStrLoop\n");
output_new("\n");
output_new("LABEL SubStrEnd\n");
output_new("PUSHS LF@SubStrResult\n");
output_new("RETURN\n");
output_new("\n");
output_new("LABEL SubStrZero\n");
output_new("PUSHS string@\n");
output_new("RETURN\n");

output_new("\n\n\n");
}


/*
// pomocna funkce pro main
void copyString(char *str, char *dest)
{
        int i;
        for (i = 0; i < strlen(str); i++)
        {
                dest[i] = str[i];
        }
}


// struktura pro simulaci vstupniho programu
#define stringMaxSize 40
typedef struct gTest{
	char text[stringMaxSize];
	int id;
	int gEOL;	// 1 = novy radek, 0 = bez noveho radku
}gTest;
*/


/*
int main()
{
	tCodeList tInput;
	tCodeInit(&tInput);

	// symtable beginning

	//funcDataPtr func = (funcDataPtr) malloc (sizeof(struct funcData)); // nahrazeno globalni promennou

	BTInsertVarInt(func, "vysl", 2);
	BTInsertVarDouble(func, "a", 12.5);
	BTInsertVarInt(func, "b", 6);
	BTInsertVarInt(func, "c", 3);


	BTItemPtr *result = (BTItemPtr *) malloc (sizeof(struct BTItem)); // pouziti vyhledavani
//	result = BTSearch(func->ParamRootPtr, "a");			  // pouziti vyhledavani

	// symtable end


	int const numOfTokens = 100;
	TString token[numOfTokens];

//		Simulace vstupu			//
// format:	"token", tokenID, novyRadek 	//

	gTest test[] = {
	{"scope", Scope, 1},

	{"dim", Dim ,0},
        {"a", ID,0},
        {"as", As ,0},
        {"integer", Integer,1},

        {"dim", Dim ,0},
        {"vysl", ID,0},
        {"as", As ,0},
        {"integer", Integer,1},

	{"print", Print, 0},
        {"!", Exclamation, 0},
        {"Zadejte cislo pro vypocet faktorialu", valueOfString, 0},
	{";", Semicolon, 1},

	{"input", Input ,0},
        {"a", ID,1},

	{"if", If,0},
	{"a", ID,0},
	{"<", Lower,0},
	{"0", valueOfInteger,0},
	{"then", Then,1},

        {"print", Print, 0},
        {"!", Exclamation, 0},
        {"\nFaktorial nelze spocitat\n", valueOfString, 0},
        {";", Semicolon, 1},

	{"else", Else, 1},

        {"vysl", ID ,0},
        {"=", Equal,0},
        {"1", valueOfInteger,1},

        {"do", Do,0},
        {"while", While,0},
        {"a", ID,0},
        {">", Greater,0},
        {"0", valueOfInteger,1},

        {"vysl", ID ,0},
        {"=", Equal,0},
        {"vysl", ID ,0},
        {"*", Mul,0},
        {"a", ID,1},

        {"a", ID ,0},
        {"=", Equal,0},
        {"a", ID ,0},
        {"-", Minus,0},
        {"1", valueOfInteger,1},

        {"loop", Loop,1},

        {"print", Print, 0},
        {"!", Exclamation, 0},
        {"\nVysledek je:", valueOfString, 0},
        {";", Semicolon, 0},
        {"vysl", ID ,0},
	{";", Semicolon, 0},
        {"!", Exclamation, 0},
	{"\n", valueOfString,0},
        {";", Semicolon, 1},


        {"end", End, 0},
        {"if", If, 1},



	{"dim", Dim ,0},
        {"promenna1", ID,1},

        {"dim", Dim ,0},
        {"a", ID,1},
        {"a", ID,0},
        {"=", Equal,0},
	{"12.5", valueOfDouble, 0},
	{"+", Plus, 0},
	{"0", valueOfInteger, 1},

        {"dim", Dim ,0},
        {"b", ID,1},
        {"b", ID,0},
        {"=", Equal,0},
        {"6", valueOfInteger, 1},

        {"dim", Dim ,0},
        {"c", ID,1},
        {"c", ID,0},
        {"=", Equal,0},
        {"3", valueOfInteger, 1},


        {"10", valueOfInteger,0},
        {"+", Plus,0},
        {"a", ID,0},
        {"*", Mul,0},
        {"(", LeftParenthes,0},
        {"5", valueOfInteger,0},
        {"-", Minus,0},
        {"1", valueOfInteger,0},
        {"/", Div,0},
        {"2", valueOfInteger,0},
	{")", RightParenthes, 1},


        {"promenna1", ID,0},
        {"=", Equal,0},
        {"8", valueOfInteger,0},
        {"+", Plus,0},
        {"(", LeftParenthes,0},
        {"a", ID,0},
        {"\\", IntDiv,0},
        {"3.4", valueOfDouble,0},
        {")", RightParenthes,0},
        {"/", Div,0},
        {"(", LeftParenthes,0},
        {"8", valueOfInteger,0},
	{"-", Minus,0},
        {"b", ID,0},
        {")", RightParenthes,0},
        {"*", Mul,0},
        {"(", LeftParenthes,0},
        {"c", ID,0},
        {"+", Plus,0},
	{"c", ID, 0},
	{"-", Minus,0},
	{"c", ID, 0},
	{"-", Minus, 0},
        {"1", valueOfInteger,0},
        {")", RightParenthes,1},

        {"print", Print,0},
        {"!", Exclamation,0},
        {"mujString", valueOfString,0},
        {";", Semicolon,0},
        {"promenna1", ID,0},
        {";", Semicolon,0},
        {"!", Exclamation,0},
        {"druhyString\n", valueOfString,1},
        {"if", If,0},
	{"(", LeftParenthes,0},
	{"3", valueOfInteger, 0},
	{"<=", LowerOrEqual, 0},
	{"(", LeftParenthes,0},
	{"5", valueOfInteger, 0},
	{"-", Minus, 0},
	{"3.5", valueOfDouble, 0},
	{"(", LeftParenthes,0},
	{")", RightParenthes,0},
	{")", RightParenthes,0},
	{")", RightParenthes,0},
        {"then", Then,1},
	{"scope", Scope, 1},
        {"else", Else,1},
        {"end", End,0},
        {"if", If,1},

        {"do", Do,0},
        {"while", While,0},
        {"(", LeftParenthes,0},
        {"4", valueOfInteger, 0},
        {"=", Equal, 0},
        {"(", LeftParenthes,0},
        {"4", valueOfInteger, 0},
        {"*", Mul, 0},
        {"2", valueOfInteger, 0},
        {")", RightParenthes,0},
        {")", RightParenthes,1},
	{"scope", Scope, 1},
        {"LOOP", Loop,1},

	{"end", End,0},
	{"scope", Scope, 0}
	};

	tCodeCreateNewLine(&tInput);


	for(int i = 0; i < numOfTokens; i++)
	{
		token[i].myString = (char *) malloc (sizeof(char) * stringMaxSize);
		copyString(test[i].text ,token[i].myString);

		tCodeInsertToken(&tInput, &token[i], test[i].id);
		if(test[i].gEOL == 1)
		{
			tCodeCreateNewLine(&tInput);
		}
	}


	//	Generovani vysledneho kodu IFJcode17	//

	printf(".IFJcode17\n"); // hlavicka jazyka IFJcode17

	// inicializace globalnich zasobniku navesti (label if a label loop)
	sLabelInit(sLabelIf);
	sLabelInit(sLabelLoop);


result = BTSearch(func->ParamRootPtr, "a");

	tCodePtr tempInput = tInput.first;
	while(tempInput != NULL)
	{
		if(generateLine(tempInput->lineData, result));	// vygeneruje jeden radek
		else
		{
			fprintf(stderr, "Chyba generatoru.\n");
			return 99;
		}
	tempInput = tempInput->next;
	}
        tCodeDispose(&tInput);

        for(int i = 0; i<numOfTokens; i++)
                free(token[i].myString);


	return 0;
}

*/
