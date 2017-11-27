#include "generator.h"

#define IFJcode17_ID 0

// globaclni zasobniky navesti pro label if a label loop
struct sLabel sStrLabelIf;
sLabelPtr sLabelIf = &sStrLabelIf;
struct sLabel sStrLabelLoop;
sLabelPtr sLabelLoop = &sStrLabelLoop;



void IFJcode17_evalExprBool(tLinePtr *temp)
{
	// TODO: everything
}


// kalkulace pro optimalizaci vyrazu (predpocitani konstant)
void IFJcode17_exprOptimalizeExecute(gListPtr first, gListPtr second, int op)
{
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
			if(first->isConst == 1 && second->isConst == 1 && (act->tokenID >= Plus && act->tokenID <= GreaterOrEqual))
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
		else	// zpracovava se operator
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
		*temp = (*temp)->next;
	}
	while(stack->top != -1)
	{
		struct tLine newLine = gListConv(stack);
		gListAdd(&gListFirst, &newLine); // operator ze zas. do sezn.
	}

	// TODO: pak smazat vypisy
	printf("\n# -------- TEST VYRAZU --------\n# Vyraz: 10+4*(5-1/2)\n# Prevedeny na postfix: ");
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
	IFJcode17_evalExprBool(temp);
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
	printf("LABEL LABELIFEND%d", sLabelTop(sLabelIf));
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

	printf("LABEL LABELLOOP%d\n", sLabelTop(sLabelLoop));
	IFJcode17_evalExprBool(temp);
	printf("PUSHS bool@true\n");
	printf("JUMPIFNEQS LABELLOOPEND%d", sLabelTop(sLabelLoop));

	while((*temp)->next != NULL)
		*temp = (*temp)->next;  // zbaveni se tokenu WHILE
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
void IFJcode17_writechar(char character)
{
	printf("\\");
	if(character < 100)
		printf("0");

	printf("%d", character);
}

// vytisknuti retezce na stdout podle pravidel IFJcode17
void IFJcode17_writeString(tLinePtr temp)
{
	printf("string@");
        for(int i = 1; temp->token->myString[i] != '\0'; i++)
        {
        	if(temp->token->myString[i] == '"') // ignorovani " na konci
        	{
 		if(temp->token->myString[i+1] != '\0')
        	{
        		IFJcode17_writechar(temp->token->myString[i]);
        	}
        }
        else
        	IFJcode17_writechar(temp->token->myString[i]);
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
	switch((*temp)->tokenID)
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
	}
	printf("POPS LF@%s ", varNameHolder);
}


int generateLine(tLinePtr tInput)
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
			    		printf("READ ");
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
typedef struct gTest{
	char text[20];
	int id;
	int gEOL;	// 1 = novy radek, 0 = bez noveho radku
}gTest;


/*

int main()
{
	tCodeList tInput;
	tCodeInit(&tInput);

	int const numOfTokens = 100;
	TString token[numOfTokens];

//		Simulace vstupu			//
// format:	"token", tokenID, novyRadek 	//

	gTest test[] = {
	{"scope", Scope, 1},
	{"dim", Dim ,0},
        {"promenna1", ID,1},
        {"promenna1", ID,0},
        {"=", Equal,0},
        {"10", valueOfInteger,0},
        {"+", Plus,0},
        {"A", ID,0},
        {"*", Mul,0},
        {"(", LeftParenthes,0},
        {"5", valueOfInteger,0},
        {"-", Minus,0},
        {"1", valueOfInteger,0},
        {"/", Div,0},
        {"2", valueOfInteger,0},
	{")", RightParenthes, 1},
        {"print", Print,0},
        {"!", Exclamation,0},
        {"\"mujString\"", valueOfString,0},
        {";", Semicolon,0},
        {"promenna1", ID,0},
        {";", Semicolon,0},
        {"!", Exclamation,0},
        {"\"druhyString\n\"", valueOfString,1},

	{"if", If,0},
	{"then", Then,1},
	{"else", Else,1},
	{"end", End,0},
	{"if", If,1},

        {"if", If,0},
        {"then", Then,1},
	        {"if", If,0},
		{"then", Then,1},
	      	{"else", Else,1},
	     	{"end", End,0},
		{"if", If,1},
        {"else", Else,1},
        {"end", End,0},
        {"if", If,1},

        {"do", Do,0},
        {"while", While,1},
		{"do", Do,0},
        	{"while", While,1},
        	{"LOOP", Loop,1},
        {"LOOP", Loop,1},

	{"end", End,0},
	{"scope", Scope, 0}
	};

	tCodeCreateNewLine(&tInput);

	for(int i = 0; i < numOfTokens; i++)
	{
		token[i].myString = (char *) malloc (sizeof(char) * 20);
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


	tCodePtr tempInput = tInput.first;
	while(tempInput != NULL)
	{
		if(generateLine(tempInput->lineData));	// vygeneruje jeden radek
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
