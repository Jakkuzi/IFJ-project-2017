#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"

#define  StringError  1
#define  StringOk      0
#define  StringLength  8


int stringInit (TString *s){

 if ((s->myString = (char*) malloc(StringLength)) == NULL)
    return StringError;

 s->myString[0] = '\0';
 s->length = 0;
 s->sizeOfAllocation = StringLength;
 return StringOk;

}

void stringFree (TString *s){

 free(s->myString);

}

void stringClean (TString *s){

 s->myString[0]= '\0';
 s->length=0;

}


int addToString (TString *s, char c ){

 if (s->length + 1 >= s->sizeOfAllocation){

   if ((s->myString = (char*) realloc(s->myString, s->length + StringLength)) == NULL)
     return StringError;
   s->sizeOfAllocation = s->length + StringLength ;
   }
 s->myString[s->length] = c;
 s->length++;
 s->myString[s->length] = '\0';
 return StringOk;


}

int compareStringAndString (TString *s1, char *s2){

 return strcmp(s1->myString, s2);

}
