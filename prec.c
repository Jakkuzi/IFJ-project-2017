#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prec.h"

#define T_OK 0		//navratova hodnota, kdyz je vse v poradku
#define T_SYN 2		//navr. hodnota chyby v syntakticke analyze
#define T_ALLOCATION 1	//chyba alokace

/*dvojrozmerne pole se znamenky pro praci s prioritami znamenek*/
char prec [11] [11] = {
		/*stack   +     -     *     /     \     <     >     <=     >=     =     <>*/
		/*index   0     1     2     3     4     5     6     7      8      9     10*/

	/* + */ /* 0 */  '>',  '>',  '<',  '<',  '<',  '>',  '>',   '>',   '>',  '>',   '>',
	/* - */ /* 1 */  '>',  '>',  '<',  '<',  '<',  '>',  '>',   '>',   '>',  '>',   '>',
	/* * */ /* 2 */  '>',  '>',  '>',  '>',  '>',  '>',  '>',   '>',   '>',  '>',   '>',
	/* / */ /* 3 */  '>',  '>',  '>',  '>',  '>',  '>',  '>',   '>',   '>',  '>',   '>',
	/* \ */ /* 4 */  '>',  '>',  '<',  '<',  '>',  '>',  '>',   '>',   '>',  '>',   '>',
	/* < */ /* 5 */  '<',  '<',  '<',  '<',  '<',  '=',  '=',   '=',   '=',  '=',   '=',
	/* > */ /* 6 */  '<',  '<',  '<',  '<',  '<',  '=',  '=',   '=',   '=',  '=',   '=',
	/* <=*/ /* 7 */  '<',  '<',  '<',  '<',  '<',  '=',  '=',   '=',   '=',  '=',   '=',
	/* >=*/ /* 8 */  '<',  '<',  '<',  '<',  '<',  '=',  '=',   '=',   '=',  '=',   '=', 
	/* = */ /* 9 */  '<',  '<',  '<',  '<',  '<',  '=',  '=',   '=',   '=',  '=',   '=',
	/* <>*/ /* 10*/  '<',  '<',  '<',  '<',  '<',  '=',  '=',   '=',   '=',  '=',   '='
};

/*dvojrozmerne pole se znamenky pro praci se zasobnikem*/
char prec_pom [16] [15] = { 
		/*stack   +     -     *     /     \     <     >     <=     >=     =     <>     (     )     i     $ */
		/*index   0     1     2     3     4     5     6     7      8      9     10     11    12    13    14*/

	/* + */ /* 0 */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* - */ /* 1 */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* * */ /* 2 */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* / */ /* 3 */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* \ */ /* 4 */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* < */ /* 5 */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* > */ /* 6 */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* <=*/ /* 7 */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* >=*/ /* 8 */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* = */ /* 9 */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* <>*/ /* 10*/  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* ( */ /* 11*/  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* ) */ /* 12*/  '>',  '>',  '>',  '>',  '>',  '>',  '>',   '>',   '>',  '>',   '>',  ' ',  '>',  ' ',  '>',
	/* i */ /* 13*/  '>',  '>',  '>',  '>',  '>',  '>',  '>',   '>',   '>',  '>',   '>',  ' ',  '>',  ' ',  '>',
	/* $ */ /* 14*/  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',   ' ',   ' ',  ' ',   ' ',  '<',  ' ',  '<',  ' ',
	/* E */ /* 15*/  '<',  '<',  '<',  '<',  '<',  '<',  '<',   '<',   '<',  '<',   '<',  ' ',  '>',  ' ',  '>',
};

void push(TStack* s, char c)
{
	s->stBody[(s->top)] = c;	
	s->top++;
}

void pop(TStack* s)
{
	s->top--;
	s->stBody[(s->top)] = '\0';
}

int t_type(char c)
{
	switch(c)
    	{
    		case '+':	return 0;
    		case '-':	return 1;
    		case '*':	return 2;
    		case '/':	return 3;
    		case '\'':	return 4;
    		case '<':	return 5;
    		case '>':	return 6;
    		case 'l':	return 7;
    		case 'm':	return 8;
    		case '=':	return 9;
    		case '!':	return 10;
    		case '(':	return 11;
    		case ')':	return 12;
    		case 'i':	return 13;
    		case '$':	return 14;
		case 'E':	return 15;
		default:	return 100;
    	}
}

char convert(char c, char d)
{
	if(c == '<' && d == '=')
		return 'l';
	else if(c == '>' && d == '=')
		return 'm';
	else if(c == '<' && d == '>')
		return '!';
	else
		return 'X';
}

char cmp_string(char* string)
{
	int compare;
	char c;
	if((compare = strcmp(string, "[i]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[E+E]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[E-E]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[E*E]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[E/E]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[E\'E]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[E<E]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[E>E]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[ElE]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[EmE]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[E!E]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[E=E]")) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, "[(E)]")) == 0)
	{
		c = 'E';
	}
	else
	{
		c = 'X';
	}

	
	return c;
}


int precedencni(char* input)
{
/*inicializace potrebnych promennych*/
	int ret = -1;		//navratova hodnota
	int pom = 0;		//pomocna promenna pro pole
	int pomst [15] = {-1};	//pomocne pole pro indexy
	char pomch [6];		//pomocny retezec pro praci se zasobnikem
	int pomcmp;		//pomocna promenna pro strcmp

/*	input = (char*) malloc (sizeof(char)*len);
	if(input == NULL)
	{
		ret = T_ALLOCATION;
		return ret;
	}

	TString* token = (TString *) malloc (sizeof(TString));
	if(token == NULL)
	{
		ret = T_ALLOCATION;
		return ret;
	}

	token -> myString = (char*) malloc (sizeof(char) * (token -> lenght));
	if(token -> myString == NULL)
	{
		ret = T_ALLOCATION;
		return ret;
	}*/

	input = strcat(input,"$");

	int len = strlen(input);

/*alokace*/
	TStack* stack = (TStack*) malloc (sizeof(TStack));
	if(stack == NULL)
	{
		ret = T_ALLOCATION;
		return ret;
	}

	stack -> stBody = (char*) malloc (sizeof(char)*len);
	if(stack -> stBody == NULL)
	{
		ret = T_ALLOCATION;
		return ret;
	}
/**************************************************************************************************************************/

/*inicializace zasobniku a vlozeni '$' na zacatek*/
	stack -> top = 0;
	push(stack, '$');

/*konverze nekterych znaku v retezci, pro lepsi praci s nimi*/
	for(int i = 0; i < len; i++)
	{
		if(convert(input[i], input[i+1]) != 'X')
		{
			input[i] = convert(input[i], input[i+1]);
			for(int j = i+1; j < len-1; j++)
			{
				input[j] = input[j+1];
			}
			len--;
		}
	}

/*inicializace promennych potrebnych v cyklu for*/
	int j0, j1, j2, j3, k0, k1, k2, k3, l0, l1, l2, l3;
	char a, b;
	for(int k = 0; k < len; k++)
	{
printf("%d\n", k);
		if(ret == 0 || ret == 2)
			break;

/*podle znamenka z tabulky prec_pom zjistime, co dale*/
		if(prec_pom [t_type(stack -> stBody[(stack->top)-1])] [t_type(input[k])] == '<')
		{

			push(stack, input[k]);

			a = stack -> stBody[(stack->top)-1];
			b = stack -> stBody[(stack->top)-3];
			if(a == '+' || a == '-' || a == '*' || a == '/' || a == '\'' || a == '<' || a == '>' || a == 'l' || a == 'm' || a == '=' || a == '!') //porovnani priority znamenek
			{
				if(b == '+' || b == '-' || b == '*' || b == '/' || b == '\'' || b == '<' || b == '>' || b == 'l' || b == 'm' || b == '=' || b == '!')
				{
					if(prec [t_type(b)] [t_type(a)] == '<')	//mensi priorita
					{
						pop(stack);
						pop(stack);
						push(stack, '[');
						pomst[pom] = (stack -> top)-1;
						pom++;
						push(stack, 'E');
						k--;
printf("%s\n", stack->stBody);						
					}

					else					//vetsi nebo shodna priorita
					{
						pop(stack);
						push(stack, ']');
printf("%s\n", stack->stBody);
						j0 = pomst[pom-1];
						k0 = stack->top;
						l0 = 0;
						for(int x0 = j0; x0 <= (k0-1); x0++)
						{
							pomch [l0] = stack -> stBody[x0];
							l0++;
						}
						pomch[l0] = '\0';
						for(int y0 = k0; y0 > j0; y0--)
						{
							pop(stack);
						}
						push(stack, cmp_string(pomch));
printf("%s\n", stack->stBody);						
						pom--;
						k--;	
					}
				}

				
				else if(b != '[')	//b = '('
				{
					pop(stack);
					pop(stack);
					push(stack, '[');					
					pomst[pom] = (stack -> top)-1;
					pom++;
					push(stack, 'E');
					k--;
printf("%s\n", stack->stBody);					
				}
			}

			else		//na vstupu je '(' nebo 'i'
			{
				pop(stack);
				push(stack, '[');				
				pomst[pom] = (stack -> top)-1;
				pom++;
				push(stack, input[k]);
printf("%s\n", stack->stBody);				
			}
		}
	
		else if(prec_pom [t_type(stack -> stBody[(stack->top)-1])] [t_type(input[k])] == '>')
		{
			push(stack, input[k]);
			
			if((stack -> stBody[(stack->top)-1]) == '$')	//test na konec vstupniho retezce
			{
				if((stack -> stBody[(stack->top)-3]) == '$')
				{
					stack -> stBody[stack->top] = '\0';
					if(pomcmp = strcmp(stack -> stBody, "$E$") == 0)
					{
						ret = T_OK;
					}
					else
					{
						ret = T_SYN;
					}
				}

				else	//pokud neni retezec jeste uplne zredukovany, redukujeme dale
				{
					pop(stack);
					push(stack, ']');
printf("%s\n", stack->stBody);					
					j1 = pomst[pom-1];
					k1 = stack->top;
					l1 = 0;
					for(int x1 = j1; x1 <= (k1-1); x1++)
					{
						pomch [l1] = stack -> stBody[x1];
						l1++;
					}
					pomch[l1] = '\0';
					for(int y1 = k1; y1 > j1; y1--)
					{
						pop(stack);
					}
					push(stack, cmp_string(pomch));
printf("%s\n", stack->stBody);					
					pom--;
					k--;
				}
			}

			else if((stack -> stBody[(stack->top)-1]) == ')')	//test na pravidlo se zavorkami
			{

				if((stack -> stBody[(stack->top)-3]) != '(')	//opet, pokud v zavorkach lze jeste neco redukovat, redukujeme
				{
					pop(stack);
					push(stack, ']');
printf("%s\n", stack->stBody);				       	
					j2 = pomst[pom-1];
					k2 = stack->top;
					l2 = 0;
					for(int x2 = j2; x2 <= (k2-1); x2++)
					{
						pomch [l2] = stack -> stBody[x2];
						l2++;
					}
					pomch[l2] = '\0';
					for(int y2 = k2; y2 > j2; y2--)
					{
						pop(stack);
					}
					push(stack, cmp_string(pomch));
printf("%s\n", stack->stBody);					
					pom--;
					k--;
				}
			}

			else	//ostatni znaky, krome '$' a ')'
			{
				pop(stack);
				push(stack, ']');
printf("%s\n", stack->stBody);				
				j3 = pomst[pom-1];
				k3 = stack->top;
				l3 = 0;
				for(int x3 = j3; x3 <= (k3-1); x3++)
				{
					pomch [l3] = stack -> stBody[x3];
					l3++;
				}
				pomch[l3] = '\0';
				for(int y3 = k3; y3 > j3; y3--)
				{
					pop(stack);
				}				

				push(stack, cmp_string(pomch));
printf("%s\n", stack->stBody);				
				pom--;
				k--;
			}

		}

		else if(prec_pom [t_type(stack -> stBody[(stack->top)-1])] [t_type(input[k])] == ' ')	//chybny vstup
		{
			ret = T_SYN;
		}

		else
		{
			ret = T_SYN;
		}
	}

/*uvolneni alokaci*/
	free(stack -> stBody);
	free(stack);

	return ret;
}


/****Testovaci funkce*****************************************************/
int main()
{
	char input [50] = "i=i+i\'i";
	int ret;
	ret = precedencni(input);
	printf("%d\n", ret);
	return 0;
}
/*************************************************************************/
