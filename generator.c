#include "generator.h"

#define IFJcode17_ID 0


// globaclni zasobniky navesti pro label if a label loop
struct sLabel sStrLabelIf;
sLabelPtr sLabelIf = &sStrLabelIf;
struct sLabel sStrLabelLoop;
sLabelPtr sLabelLoop = &sStrLabelLoop;

// data prave aktualni funkce (navratovy typ, binarni strom parametru a lokalnich promennych)
struct funcData sFunc;
funcDataPtr func = &sFunc;

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

	double oFirst;
	double oSecond;
	double result;
	sscanf(first->data, "%lf", &oFirst);
	sscanf(second->data, "%lf", &oSecond);

	switch(op)
	{
		case Plus:
			result = oFirst + oSecond;
			if(first->tokenID == valueOfDouble || second->tokenID == valueOfDouble)
				first->tokenID = valueOfDouble;
			break;
		case Minus:
			result = oFirst - oSecond;
                        if(first->tokenID == valueOfDouble || second->tokenID == valueOfDouble)
                                first->tokenID = valueOfDouble;
			break;
		case Mul:
			result = oFirst * oSecond;
                        if(first->tokenID == valueOfDouble || second->tokenID == valueOfDouble)
                                first->tokenID = valueOfDouble;
			break;
		case Div:
			result = oFirst / oSecond;
			first->tokenID = valueOfDouble;
			break;
		case IntDiv:
			result = IFJcode17_exprDouble2Int(oFirst) / IFJcode17_exprDouble2Int(oSecond);
			result = (int) result;
			first->tokenID = valueOfInteger;
			break;

		default: fprintf(stderr, "Optimalizator - neznamy operand\n");
	}
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



void IFJcode17_exprConvTypesExecute(gListPtr *list, int *array, int index)
{
	gListPtr listAssist = *list;

	unsigned int counter = 0;
        while(listAssist != NULL)
        {
		if(array[counter] != 0)
		{
			if(listAssist->tokenID == ID || (listAssist->tokenID >= Plus && listAssist->tokenID <= IntDiv)) // ID nebo operand
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
		listAssist = listAssist->next;
	}
}


void IFJcode17_exprConvTypes(gListPtr *list)
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
			searchResult = BTSearch(func->ParamRootPtr, listAssist->data);
			switch(searchResult->varData->type)
			{
				case var_integer: convListAssist->dataType = valueOfInteger; break;
				case var_double: convListAssist->dataType = valueOfDouble; break;
				default: fprintf(stderr, "Chyba generatoru. Konverze datovych typu se nepovedla.\n");
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

	for (int y = 0; y < index; y++)
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


		if((first == valueOfInteger || first == valueOfDouble) &&
		(second == valueOfInteger || second == valueOfDouble) &&
		(op >= Plus && op <= IntDiv))	// lze vypocitat
		{
			converted = 1;
			switch(op)
			{
				case Plus: case Minus: case Mul:
					if(first == valueOfInteger && second == valueOfDouble)
					{
						convListAssist4->dataType = valueOfDouble;
						array[convListAssist4->order] = Int2Float;
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

			}
			convListAssist4->order = convListAssist->order;
			convListAssist4->next = convListAssist->next; // preklenuti
			// TODO: preklenuti (ztrata 2 ukazatelu)
			break;	// TODO
		}

		convListAssist = convListAssist->next;
	}
	}

	IFJcode17_exprConvTypesExecute(list, array, index);
}


// prevede vyraz na postfixovou (polskou) notaci
gListPtr IFJcode17_toPostfix(tLinePtr *temp)
{
	struct gStack st;
	gStackPtr stack = &st;
	gStackInit(stack);

	//struct gList list;
	gListPtr gListFirst = (gListPtr) malloc (sizeof(struct gList));
	gListInit(gListFirst);

	while((*temp) != NULL)
	{
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
	printf("\n# -------- TEST VYRAZU --------\n# Prevedeny na postfix: ");
	gListPrint(gListFirst);
	while(IFJcode17_exprOptimalize(&gListFirst) == 1);	// optimalizace vyrazu
	printf("\n# Vyraz po optimalizaci (predpocitani konstant): ");
	gListPrint(gListFirst);
	printf("\n# Vyraz po implicitni konverzi datovych typu: ");
	IFJcode17_exprConvTypes(&gListFirst);
	gListPrint(gListFirst);
	printf("\n# -------- KONEC TESTU --------\n\n");

	return gListFirst;
}


// vyhodnoceni vyrazu bez pravdivostnich hodnot a relacnich opteratoru (true, false)
void IFJcode17_evalExpr(tLinePtr *temp)
{
        gListPtr list, assistList;
	list = IFJcode17_toPostfix(temp);	// ulozeni vytupniho seznamu vyrazu v postfixu
	assistList = list;
	while(assistList != NULL)
	{
		if(assistList->isOp == 1)
		{
			switch(assistList->tokenID)
			{
				case Float2Int2Float: 	printf("FLOAT2R2EINTS\n");
							printf("INT2FLOATS\n"); break;
				case Int2Float: printf("INT2FLOATS\n"); break;
				case Float2Int:	printf("FLOAT2R2EINTS\n"); break;
				case Plus: printf("ADDS\n"); break;
                                case Minus: printf("SUBS\n"); break;
                                case Mul: printf("MULS\n"); break;
                                case Div: printf("DIVS\n"); break;
                                case IntDiv: 	printf("DIVS\n");
						printf("FLOAT2INTS\n"); break;
				case Equal:	printf("EQS\n"); break;
                                case Lower:     printf("LTS\n"); break;
                                case Greater:     printf("GTS\n"); break;
                                case LowerGreater:     printf("EQS\n");
							printf("NOTS\n"); break;
                                case LowerOrEqual: case GreaterOrEqual:
						printf("\n# BEGINNING: <= or >= operator\n"); // navic kvuli prehlednosti
						printf("CREATEFRAME\n");
						printf("DEFVAR TF@assist1\n");
						printf("DEFVAR TF@assist2\n");
						printf("DEFVAR TF@result1\n");
						printf("DEFVAR TF@result2\n");
						printf("POPS TF@assist1\n");
						printf("POPS TF@assist2\n");
						if(assistList->tokenID == LowerOrEqual)
							printf("LT ");
						else
							printf("GT ");
						printf("TF@result1 TF@assist2 TF@assist1\n");
						printf("EQ TF@result2 TF@assist2 TF@assist1\n");
						printf("PUSHS TF@result1\n");
						printf("PUSHS TF@result2\n");
						printf("ORS\n");
						printf("CREATEFRAME\n");
						printf("# END: <= or >= operator\n\n"); // navic kvuli prehlednosti
							break;

				// TODO: relacni operatory
			}
		}
		else
		{
			printf("PUSHS ");
			if(assistList->isConst == 1)
			{
				switch(assistList->tokenID)
                        	{
                                	case valueOfInteger: printf("int@"); break;
                               		case valueOfDouble: printf("float@"); break;
                        		// TODO: relacni operatory
				}
			}
			else
			{
				printf("LF@");
			}
			printf("%s\n", assistList->data);
		}
		assistList = assistList->next;
	}
}



// zpracovani prikazu IF
void IFJcode17_if(tLinePtr *temp)
{
	labelIf += 1;
	sLabelPush(sLabelIf, labelIf);
	if((*temp)->next != NULL)
		*temp = (*temp)->next;
	if((*temp)->tokenID != Then)
		IFJcode17_evalExpr(temp);
	printf("PUSHS bool@true\n");
	printf("JUMPIFNEQS LABELIF%d", sLabelTop(sLabelIf));
	while((*temp)->tokenID != Then)
		*temp = (*temp)->next;

	*temp = (*temp)->next;	// zbaveni se tokenu Then
}

// zpracovani prikazu ELSE (pouziva navesti z predchoziho IF)
void IFJcode17_else(tLinePtr *temp)
{
	printf("JUMP LABELIFEND%d\n", sLabelTop(sLabelIf));
	printf("LABEL LABELIF%d", sLabelTop(sLabelIf));

	if((*temp)->next != NULL)
		*temp = (*temp)->next;	// zbaveni se tokenu Else
}

// zpracovani sekvence prikazu end if
void IFJcode_ifend(tLinePtr *temp)
{
	printf("LABEL LABELIFEND%d\n", sLabelTop(sLabelIf));
	sLabelPop(sLabelIf);

	while((*temp)->next != NULL)
		*temp = (*temp)->next;  // zbaveni se tokenu (end) if
}


// zpracovani sekvence prikazu DO WHILE
void IFJcode17_dowhile(tLinePtr *temp)
{

	labelLoop += 1;
	sLabelPush(sLabelLoop, labelLoop);

        if((*temp)->next != NULL)
                *temp = (*temp)->next;  // zbaveni se tokenu DO
        if((*temp)->next != NULL)
                *temp = (*temp)->next;  // zbaveni se tokenu WHILE


	printf("LABEL LABELLOOP%d\n", sLabelTop(sLabelLoop));

	if((*temp)->tokenID != While)
		IFJcode17_evalExpr(temp);

	printf("PUSHS bool@true\n");
	printf("JUMPIFNEQS LABELLOOPEND%d", sLabelTop(sLabelLoop));
}

// zpracovani ukoncovaciho prikazu cyklu LOOP
void IFJcode17_loop(tLinePtr *temp)
{
	printf("JUMP LABELLOOP%d\n", sLabelTop(sLabelLoop));
	printf("LABEL LABELLOOPEND%d", sLabelTop(sLabelLoop));
	sLabelPop(sLabelLoop);

	while((*temp)->next != NULL)
		*temp = (*temp)->next;  // zbaveni se tokenu LOOP
}


// pomocna funkce pro generovani instrukce WRITE
// vytiskne na stdout lomitko a za nim tricifernou ascii hodnotu jednoho znaku
void IFJcode17_writechar(char character, int special)
{
	printf("\\");
	if(special == 0) // zpracovani normalniho znaku
	{
		if(character < 100)
		{
			printf("0");
		}
		printf("%d", character);
	}
	else	// zpracovani specialniho znaku (\n, \t,...)
	{
		printf("0");
		switch(character)
		{
			case 'n': printf("%d", '\n');	break;
			case 't': printf("%d", '\t');	break;
			case '\\': printf("%d", '\\');	break;
			case 34: printf("%d", 34);	break; // uvozovky "
			default: fprintf(stderr, "Chyba generatoru, neznama backslash sekvence v prikazu Print\n");
		}
	}
}

// vytisknuti retezce na stdout podle pravidel IFJcode17
void IFJcode17_writeString(tLinePtr temp)
{
	int special = 0; // udava jestli je znak specialni (\n, \t,...)

	printf("string@");
        for(int i = 0; temp->token->myString[i] != '\0'; i++)
        {
		if(temp->token->myString[i] == '\\')
		{
			special = 1;
			continue;
		}
      		if(temp->token->myString[i] == '"') // ignorovani " na konci
        	{
 			if(temp->token->myString[i+1] != '\0')
        		{
        			IFJcode17_writechar(temp->token->myString[i], special);
        		}
        	}
        	else
        		IFJcode17_writechar(temp->token->myString[i], special);
		special = 0;
	}
}

// generovani instrukce WRITE
void IFJcode17_write(tLinePtr *temp)
{
        *temp = (*temp)->next;
        while(*temp != NULL)
        {
                if((*temp)->tokenID != Semicolon)
                        printf("WRITE ");

                if((*temp)->tokenID == Semicolon)
                        *temp = (*temp)->next;
                else if((*temp)->tokenID == Exclamation)
                {
                        *temp = (*temp)->next;
			IFJcode17_writeString(*temp);
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
        }
}

// generovani prirazeni do promenne
void IFJcode17_varAssign(tLinePtr *temp)
{
	char *varNameHolder = (*temp)->token->myString;
	*temp = (*temp)->next->next;
	/*switch((*temp)->tokenID)
	{
		case valueOfInteger:
			IFJcode17_evalExpr(temp);
			//printf("int@%s", (*temp)->token->myString);
			break;
		case valueOfDouble:
			printf("float@%s", (*temp)->token->myString);
			break;
		case valueOfDoubleWithExp:
			// TODO: uprava Exp ?
			break;
		case valueOfString:
			IFJcode17_writeString(*temp);
			break;
	}*/
	IFJcode17_evalExpr(temp);

	printf("POPS LF@%s ", varNameHolder);
}


// generovani prikazu Input
void IFJcode17_input(tLinePtr *temp, BTItemPtr *BTree)
{
	if((*temp)->next != NULL)
		*temp = (*temp)->next;
	printf("READ LF@%s ", (*temp)->token->myString);


	if(BTree->itemType == item_type_variable);
	{
		switch(BTree->varData->type)
		{
			case var_integer: printf("int"); break;
			case var_double: printf("float"); break;
			case var_string: printf("string"); break;
			default: fprintf(stderr, "Chyba generatoru - neznamy typ promenne\n");
		}
		printf("\n");
	}
}


int generateLine(tLinePtr tInput, BTItemPtr *BTree)
{
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
					printf("CLEARS\n");
			    		printf("POPFRAME");
				}
				else if(temp->next->tokenID == If)
				{
					IFJcode_ifend(&temp);
				}
			    }
			    if(temp->next != NULL)
				temp = temp->next;
			    break;
			case Scope:
			    printf("CREATEFRAME\n");
			    printf("PUSHFRAME");
			    break;
			case Dim:
			    printf("DEFVAR ");
			    break;
			case As:
				// TODO: inicializace "Dim promenna as Integer = 3"
			    break;
			case ID:
			    if(temp->next != NULL)
			    {
				    if(temp->next->tokenID == Equal)
			    		IFJcode17_varAssign(&temp);
				    else
					printf("LF@%s", temp->token->myString);
			    }
			    else
				printf("LF@%s", temp->token->myString);
			    break;
			case Equal:
			    break;
			case Print:
			    IFJcode17_write(&temp);
			    break;
			case valueOfString:
			    //IFJcode17_write(temp);
			    break;
			case Input:
			    IFJcode17_input(&temp, BTree);
			    break;
			case If:
			    IFJcode17_if(&temp);
			    break;
			case Else:
			    IFJcode17_else(&temp);
			    break;
			case Do:
			    IFJcode17_dowhile(&temp);
			    break;
			case Loop:
			    IFJcode17_loop(&temp);
			    break;
		}
		if(temp != NULL)
			temp = temp->next;
	}while(temp != NULL && (temp->tokenID >= ID && temp->tokenID <= EndOfLine) );
	// TODO: upravit podminku cyklu (problem s duplicitnim "end scope", temp->next nebyl NULL)
	printf("\n");
	return 1;
}

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
/*
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
*/


	{"dim", Dim ,0},
        {"promenna1", ID,1},
        {"promenna1", ID,0},
        {"=", Equal,0},
/*
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
*/


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
        {"1", valueOfInteger,0},
        {")", RightParenthes,1},

        {"print", Print,0},
        {"!", Exclamation,0},
        {"\"mujString\"", valueOfString,0},
        {";", Semicolon,0},
        {"promenna1", ID,0},
        {";", Semicolon,0},
        {"!", Exclamation,0},
        {"\"druhyString\n\"", valueOfString,1},
        {"if", If,0},
	{"(", LeftParenthes,0},
	{"3", valueOfInteger, 0},
	{"<=", LowerOrEqual, 0},
	{"(", LeftParenthes,0},
	{"5", valueOfInteger, 0},
	{"-", Minus, 0},
	{"4", valueOfInteger, 0},
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

