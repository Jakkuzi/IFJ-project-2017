/*
 * Implementace prekladace imperativního jazyka IFJ17.
 * xzichj00 - Jakub Zich
 */


#ifndef prec_h
#define prec_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {		//struktura zasobniku
	char* stBody;
	int top;
} TStack;

void push(TStack* s, char c);	//vlozi znak na vrchol zasobniku
void pop(TStack* s);		//odstrani znak z vrcholu zasobniku
int t_type(char c);		//funkce pro rozpoznani typu znaku
char convert(char c, char d);	//pomocna funkce pro prevod znaku
char cmp_string(char* string);	//funkce pro porovnavani retezcu

int precedencni(char* input);		//hlavni funkce precedencni analyzy

#endif
