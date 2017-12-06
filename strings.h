/*
 * Implementace prekladace imperativního jazyka IFJ17.
 * xhudec30 - Patricia Hudedova
 */

#ifndef strings_h
#define strings_h

typedef struct {
char* myString;
int length;
int sizeOfAllocation;
} TString;

int stringInit (TString *s);
void stringFree (TString *s);
void stringClean (TString *s);
int addToString (TString *s, char c );
int compareStringAndString (TString *s1, char *s2);


#endif
