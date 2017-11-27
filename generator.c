#include "generator.h"

#define IFJcode17_ID 0

// globaclni zasobniky navesti pro label if a label loop
struct sLabel sStrLabelIf;
sLabelPtr sLabelIf = &sStrLabelIf;
struct sLabel sStrLabelLoop;
sLabelPtr sLabelLoop = &sStrLabelLoop;



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
			result = oFirst + oSecond; break;
		case Minus:
			result = oFirst - oSecond; break;
		case Mul:
			result = oFirst * oSecond; break;
		case Div:
			result = oFirst / oSecond; break;

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
				case Plus: printf("ADDS\n"); break;
                                case Minus: printf("SUBS\n"); break;
                                case Mul: printf("MULS\n"); break;
                                case Div: printf("DIVS\n"); break;
                                case IntDiv: 	printf("DIVS\n");
						printf("FLOAT2INT\n"); break;
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


/*

int main()
{
	tCodeList tInput;
	tCodeInit(&tInput);

	// symtable beginning

	funcDataPtr func = (funcDataPtr) malloc (sizeof(struct funcData));

	BTInsertVarInt(func, "vysl", 2);
	BTInsertVarInt(func, "a", 3);

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
