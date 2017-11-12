#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prec.h"

#define T_OK 0		//navratova hodnota, kdyz je vse v poradku
#define T_SYN 2		//navr. hodnota chyby v syntakticke analyze
#define T_ALLOCATION 1	//chyba alokace

/*dvojrozmerne pole se znamenky pro praci se zasobnikem*/
char prec [15] [15] = {
		/*stack   +    -    *    /    \    <    >    <=    >=    =    <>    (    )    i    $*/
		/*index   0    1    2    3    4    5    6    7     8     9    10    11   12   13   14*/

	/* + */ /* 0 */  '>'  '>'  '<'  '<'  '<'  '>'  '>'   '>'   '>'  '>'   '>'  '<'  '>'  '<'  '>'
	/* - */ /* 1 */  '>'  '>'  '<'  '<'  '<'  '>'  '>'   '>'   '>'  '>'   '>'  '<'  '>'  '<'  '>'
	/* * */ /* 2 */  '>'  '>'  '>'  '>'  '>'  '>'  '>'   '>'   '>'  '>'   '>'  '<'  '>'  '<'  '>'
	/* / */ /* 3 */  '>'  '>'  '>'  '>'  '>'  '>'  '>'   '>'   '>'  '>'   '>'  '<'  '>'  '<'  '>'
	/* \ */ /* 4 */  '>'  '>'  '<'  '<'  '>'  '>'  '>'   '>'   '>'  '>'   '>'  '<'  '>'  '<'  '>'
	/* < */ /* 5 */  '<'  '<'  '<'  '<'  '<'  '='  '='   '='   '='  '='   '='  '<'  '>'  '<'  '>'
	/* > */ /* 6 */  '<'  '<'  '<'  '<'  '<'  '='  '='   '='   '='  '='   '='  '<'  '>'  '<'  '>'
	/* <=*/ /* 7 */  '<'  '<'  '<'  '<'  '<'  '='  '='   '='   '='  '='   '='  '<'  '>'  '<'  '>'
	/* >=*/ /* 8 */  '<'  '<'  '<'  '<'  '<'  '='  '='   '='   '='  '='   '='  '<'  '>'  '<'  '>'
	/* = */ /* 9 */  '<'  '<'  '<'  '<'  '<'  '='  '='   '='   '='  '='   '='  '<'  '>'  '<'  '>'
	/* <>*/ /* 10*/  '<'  '<'  '<'  '<'  '<'  '='  '='   '='   '='  '='   '='  '<'  '>'  '<'  '>'
	/* ( */ /* 11*/  '<'  '<'  '<'  '<'  '<'  '<'  '<'   '<'   '<'  '<'   '<'  '<'  '='  '>'  ' '
	/* ) */ /* 12*/  '>'  '>'  '>'  '>'  '>'  '>'  '>'   '>'   '>'  '>'   '>'  ' '  '>'  ' '  '>'
	/* i */ /* 13*/  '>'  '>'  '>'  '>'  '>'  '>'  '>'   '>'   '>'  '>'   '>'  ' '  '>'  ' '  '>'
	/* $ */ /* 14*/  '<'  '<'  '<'  '<'  '<'  '<'  '<'   '<'   '<'  '<'   '<'  '<'  ' '  '<'  ' '
};

char prec_pom [15] [15] = {
		/*stack   +    -    *    /    \    <    >    <=    >=    =    <>    (    )    i    $*/
		/*index   0    1    2    3    4    5    6    7     8     9    10    11   12   13   14*/

	/* + */ /* 0 */  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* - */ /* 1 */  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* * */ /* 2 */  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* / */ /* 3 */  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* \ */ /* 4 */  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* < */ /* 5 */  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* > */ /* 6 */  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* <=*/ /* 7 */  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* >=*/ /* 8 */  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* = */ /* 9 */  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* <>*/ /* 10*/  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* ( */ /* 11*/  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
	/* ) */ /* 12*/  '>'  '>'  '>'  '>'  '>'  '>'  '>'   '>'   '>'  '>'   '>'  ' '  '>'  ' '  '>'
	/* i */ /* 13*/  '>'  '>'  '>'  '>'  '>'  '>'  '>'   '>'   '>'  '>'   '>'  ' '  '>'  ' '  '>'
	/* $ */ /* 14*/  ' '  ' '  ' '  ' '  ' '  ' '  ' '   ' '   ' '  ' '   ' '  '<'  ' '  '<'  ' '
};

void push(TStack* s, char c)
{
		s->stBody[(s->top)] = c;	
		s->top++;
}

void pop(TStack* s)
{
		s->top--;
}

int t_type(char c)
{
	switch(c)
    	{
    		case '+':	return 0;
    		case '-':	return 1;
    		case '*':	return 2;
    		case '/':	return 3;
    		case '\':	return 4;
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
	if((compare = strcmp(string, '<i>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<E+E>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<E-E>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<E*E>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<E/E>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<E\E>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<E<E>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<E>E>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<ElE>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<EmE>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<E!E>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<E=E>')) == 0)
	{
		c = 'E';
	}
	else if((compare = strcmp(string, '<(E)>')) == 0)
	{
		c = 'E';
	}
	else
	{
		c = 'X';
	}

	
	return c;
}


int prec(char* input)
{
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



	input = getInput();
	input = strcat(input,"$");

	int len = strlen(input);

	TStack* stack = (TStack*) malloc (sizeof(TStack));
	if(stack == NULL)
	{
		ret = T_ALLOCATION;
		return ret;
	}

	stack -> stBody = (char*) malloc (sizeof(char)*len);
	if(stack -> stackBody == NULL)
	{
		ret = T_ALLOCATION;
		return ret;
	}
/**************************************************************************************************************************/


	stack -> top = 0;
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

	push(stack, '$');

	for(int k = 0; k < len; k++)
	{
		if(ret == 0 || ret == 2)
			break;
		if(prec_pom [t_type(stack -> stBody[top])] [t_type(input[k])] == '<')
		{
			if(stack -> stBody[(stack->top)-1] == 'E')
			{
				pop(stack);
				push(stack, '<');
				pomst[pom] = (stack -> top)-1;
				pom++;
				push(stack, 'E');
			}
			else
			{
				push(stack, '<');
				pomst[pom] = (stack -> top)-1;
				pom++;
			}
			push(stack, input[k]);
		}
	
		else if(prec_pom [t_type(stack -> stBody[top])] [t_type(input[k])] == '>')
		{
			push(stack, '>'); 
			int j = pomst[pom-1];
			int k = stack->top;
			int l = 0;
			int m = 0;
			for(int x = j; x <= (k-1); x++)
			{
				pomch [i] = stack -> stBody[x];
				i++;
			}
			pomch[i] = '\0'
			for(int y = k; y > j; y--)
			{
				pop(stack);
			}
			push(stack, cmp_string(pomch));
			i = 0;
			push(stack, input[k]);
			
			if((stack -> stBody[(stack->top)-1]) == '$')
			{
				stack -> stBody[stack->top] = '\0';
				if(pomcmp = strcmp(stack -> stBody, '$E$')
				{
					ret = T_OK;
				}
				else
				{
					ret = T_SYN;
				}
			}
		}

		else if(prec_pom [t_type(stack -> stBody[top])] [t_type(input[k])] == ' ')
		{
			ret = T_SYN;
		}

		/*else if(prec_pom [t_type(stack -> stBody[top])] [t_type(input[k])] == '=')
		{
			push(stack, input[k]);
			stack -> stBody[stack->top] = '\0';
			if(pomcmp = strcmp(stack -> stBody, '$E$')
			{
				ret = T_OK;
			}
			else
			{
				ret = T_SYN;
			}
			break;
		}*/

		else
		{
			ret = T_SYN;
		}
	}
	


	free(stack -> stackBody);
	free(stack);
	free(token -> myString);
	free(token);
	free(input);

	return ret;

}


